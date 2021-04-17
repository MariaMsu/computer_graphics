#define _USE_MATH_DEFINES

#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "geometry.h"
#include "shapes.h"
#include "config.h"
#include "material.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Lib/stb_image_write.h"

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

int screen_width = 512, screen_height = 512;

struct Light {
    Light(const Vec3f &p, const float i) : position(p), intensity(i) {}

    Vec3f position;
    float intensity;
};


Vec3f reflect(const Vec3f &I, const Vec3f &N) {
    return I - N * 2.f * (I * N);
}

Vec3f refract(const Vec3f &I, const Vec3f &N, const float eta_t, const float eta_i = 1.f) { // Snell's law
    float cosi = -std::max(-1.f, std::min(1.f, I * N));
    if (cosi < 0)
        return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the object, swap the air and the media
    float eta = eta_i / eta_t;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
    return k < 0 ? Vec3f(1, 0, 0) : I * eta + N * (eta * cosi - sqrtf(k));
}

bool scene_intersect(const Vec3f &orig, const Vec3f &dir, const std::vector<Shape *> &shapes, Vec3f &hit, Vec3f &N,
                     Material &material) {
    float shapes_dist = std::numeric_limits<float>::max();
    for (auto shape : shapes) {
        float dist_i;
        if (shape->ray_intersect(orig, dir, dist_i, material) && dist_i < shapes_dist) {
            shapes_dist = dist_i;
            hit = orig + dir * dist_i;
            // todo make reflection not as a sphere
            N = (hit - shape->get_center()).normalize();
        }
    }

    float checkerboard_dist = std::numeric_limits<float>::max();
    if (fabs(dir.y) > 1e-3) {
        float d = -(orig.y + 6) / dir.y; // the checkerboard plane has equation y = -6
        Vec3f pt = orig + dir * d;
        if (d > 0 && fabs(pt.x) < 15 && pt.z < -10 && pt.z > -60 && d < shapes_dist) {
            checkerboard_dist = d;
            hit = pt;
            N = Vec3f(0, 1, 0);
            material.diffuse_color = (int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1 ?
                                     Vec3f(0.05, 0.2, 0.05) :
                                     Vec3f(0.05, 0.05, 0.2);
        }
    }
    return std::min(shapes_dist, checkerboard_dist) < 1000;
}

Vec3f
cast_ray(const Vec3f &orig, const Vec3f &dir, const std::vector<Shape *> &shapes, const std::vector<Light> &lights,
         size_t depth = 0) {
    Vec3f point, N;
    Material material;

    if (depth > H_RECURSION_DEPTH || !scene_intersect(orig, dir, shapes, point, N, material)) {
        return background_colour;
    }

    Vec3f reflect_dir = reflect(dir, N).normalize();
    Vec3f refract_dir = refract(dir, N, material.refractive_index).normalize();
    // offset the original point to avoid occlusion by the object itself
    Vec3f reflect_orig = reflect_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
    Vec3f refract_orig = refract_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
    Vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, shapes, lights, depth + 1);
    Vec3f refract_color = cast_ray(refract_orig, refract_dir, shapes, lights, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (auto light : lights) {
        Vec3f light_dir = (light.position - point).normalize();
        float light_distance = (light.position - point).norm();

        // checking if the point lies in the shadow of the lights[i]
        Vec3f shadow_orig = light_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
        Vec3f shadow_pt, shadow_N;
        Material tmpmaterial;
        if (scene_intersect(shadow_orig, light_dir, shapes, shadow_pt, shadow_N, tmpmaterial) &&
            (shadow_pt - shadow_orig).norm() < light_distance)
            continue;

        diffuse_light_intensity += light.intensity * std::max(0.f, light_dir * N);
        specular_light_intensity +=
                powf(std::max(0.f, -reflect(-light_dir, N) * dir), material.specular_exponent) * light.intensity;
    }
    return material.diffuse_color * diffuse_light_intensity * material.albedo[0]
           + Vec3f(1., 1., 1.) * specular_light_intensity * material.albedo[1]
           + reflect_color * material.albedo[2]
           + refract_color * material.albedo[3];
}

void render(const std::vector<Shape *> &shapes, const std::vector<Light> &lights, const char *file_name) {
    auto *framebuffer = new unsigned char[screen_width * screen_height * 3];

    for (size_t j = 0; j < screen_height; j++) { // rendering loop
        for (size_t i = 0; i < screen_width; i++) {
            float dir_x = (i + 0.5) - screen_width / 2.;
            float dir_y = -(j + 0.5) + screen_height / 2.;    // this flips the image at the same time
            float dir_z = -screen_height / (2. * tan(H_FIELD_OF_VIEW / 2.));
            Vec3f pixel =
                    cast_ray(Vec3f(0, 0, 0), Vec3f(dir_x, dir_y, dir_z).normalize(), shapes, lights);
            framebuffer[(i + j * screen_width) * 3 + 0] = (unsigned char) MAX(0, MIN(pixel.x * 256, 256));
            framebuffer[(i + j * screen_width) * 3 + 1] = (unsigned char) MAX(0, MIN(pixel.y * 256, 256));
            framebuffer[(i + j * screen_width) * 3 + 2] = (unsigned char) MAX(0, MIN(pixel.z * 256, 256));
        }
    }

    stbi_write_png(file_name, screen_width, screen_height, 3, framebuffer, 0);
    delete[] framebuffer;
}

int main(int argc, char **argv) {
    if ((argc >= 2) && strcmp(argv[1], "-w") == 0){
        screen_width = 1024;
        screen_height = 1024;
    }

    std::vector<Shape *> shapes;
    shapes.push_back(new Sphere(Vec3f(0, 2, -17), 6, glass));

    shapes.push_back(new TexturedParallelepiped(Vec3f(2.5, 2.8, -19), 3, 3, 3, TEXTURE_WATER));
    shapes.push_back(new TexturedParallelepiped(Vec3f(-0.5, -0.2, -19), 3, 3, 3, TEXTURE_GROUND));
    shapes.push_back(new TexturedParallelepiped(Vec3f(2.5, -0.2, -16), 3, 3, 3, TEXTURE_SAND));

    shapes.push_back(new Parallelepiped(Vec3f(0, -5, -18), 1, 12, 12, gold));

    std::vector<Light> lights;
    lights.emplace_back(Vec3f(-20, 20, 20), 1.5);
    lights.emplace_back(Vec3f(30, 70, -25), 1.8);
    lights.emplace_back(Vec3f(40, 20, 30), 1.7);

    time_t start_time = time(nullptr);
    std::string file_name = "./render_" + std::to_string(start_time) + ".png";
    render(shapes, lights, file_name.c_str());
    time_t stop_time = time(nullptr);
    std::cout << "render time is " << stop_time - start_time << " seconds \n";

    return 0;
}

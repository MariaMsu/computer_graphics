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
    return k < 0 ? Vec3f(1, 0, 0) : I * eta + N * (eta * cosi -
                                                   sqrtf(k)); // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
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
//            material = shape->get_material();
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
            material.diffuse_color =
                    (int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1 ? Vec3f(.3, .3, .3) : Vec3f(.3, .2, .1);
        }
    }
    return std::min(shapes_dist, checkerboard_dist) < 1000;
    return shapes_dist < 1000;
}

Vec3f
cast_ray(const Vec3f &orig, const Vec3f &dir, const std::vector<Shape *> &shapes, const std::vector<Light> &lights,
         size_t depth = 0) {
    Vec3f point, N;
    Material material;

    if (depth > H_RECURSION_DEPTH || !scene_intersect(orig, dir, shapes, point, N, material)) {
        return Vec3f(0.2, 0.7, 0.8); // background color
    }

    Vec3f reflect_dir = reflect(dir, N).normalize();
    Vec3f refract_dir = refract(dir, N, material.refractive_index).normalize();
    Vec3f reflect_orig = reflect_dir * N < 0 ? point - N * 1e-3 : point + N *  1e-3; // offset the original point to avoid occlusion by the object itself
    Vec3f refract_orig = refract_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
    Vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, shapes, lights, depth + 1);
    Vec3f refract_color = cast_ray(refract_orig, refract_dir, shapes, lights, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (auto light : lights) {
        Vec3f light_dir = (light.position - point).normalize();
        float light_distance = (light.position - point).norm();

        Vec3f shadow_orig = light_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3; // checking if the point lies in the shadow of the lights[i]
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

void render(const std::vector<Shape *> &shapes, const std::vector<Light> &lights, const std::string& file_name) {
    std::vector<Vec3f> framebuffer(H_SCREEN_WIDTH * H_SCREEN_HEIGHT);

#pragma omp parallel for
    for (size_t j = 0; j < H_SCREEN_HEIGHT; j++) { // actual rendering loop
        for (size_t i = 0; i < H_SCREEN_WIDTH; i++) {
            float dir_x = (i + 0.5) - H_SCREEN_WIDTH / 2.;
            float dir_y = -(j + 0.5) + H_SCREEN_HEIGHT / 2.;    // this flips the image at the same time
            float dir_z = -H_SCREEN_HEIGHT / (2. * tan(H_FIELD_OF_VIEW / 2.));
            framebuffer[i + j * H_SCREEN_WIDTH] =
                    cast_ray(Vec3f(0, 0, 0), Vec3f(dir_x, dir_y, dir_z).normalize(), shapes, lights);
        }
    }

    std::ofstream ofs; // save the framebuffer to file
    ofs.open(file_name, std::ios::binary);
    ofs << "P6\n" << H_SCREEN_WIDTH << " " << H_SCREEN_HEIGHT << "\n255\n";
    for (size_t i = 0; i < H_SCREEN_HEIGHT * H_SCREEN_WIDTH; ++i) {
        Vec3f &c = framebuffer[i];
        float max = std::max(c[0], std::max(c[1], c[2]));
        if (max > 1) c = c * (1. / max);
        for (size_t j = 0; j < 3; j++) {
            ofs << (char) (255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
}

int main() {
    std::vector<Shape *> shapes;
//    shapes.push_back(new Sphere(Vec3f(3, 1, -18), 6, glass));

    shapes.push_back(new TexturedParallelepiped(Vec3f(-3, 1, -12), 4, 4, 4, material_for_texture));
    shapes.push_back(new Parallelepiped(Vec3f(0, -5, -18), 1, 12, 12, red_rubber));

    std::vector<Light> lights;
    lights.emplace_back(Vec3f(-20, 20, 20), 1.5);
    lights.emplace_back(Vec3f(30, 50, -25), 1.8);
    lights.emplace_back(Vec3f(30, 20, 30), 1.7);


    time_t start_time = time(nullptr);
    std::string file_name = "./render_" + std::to_string(start_time) + ".ppm";
    render(shapes, lights, file_name);
    time_t stop_time = time(nullptr);
    std::cout << "render time is " << stop_time - start_time << " seconds \n";

    return 0;
}

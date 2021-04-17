#ifndef RAY_TRACING_SHAPES_H
#define RAY_TRACING_SHAPES_H

#include "material.h"
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Lib/stb_image.h"

struct Shape {
public:
    /* orig - camera position
     * dir - direction of gaze from the camera
     * t0 - distance to nearest intersection */
    virtual bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0, Material &point_material) const = 0;

    virtual Vec3f get_center() const = 0;
};

struct Sphere: public Shape {
    Vec3f center;
    float radius;
    Material material;

    Vec3f get_center() const override { return center; };

    Sphere(const Vec3f &c, const float r, const Material &m) : center(c), radius(r), material(m) {}

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0, Material &point_material) const override {
        Vec3f L = center - orig;  // the vector from camera to the center of the sphere
        float tca = L*dir;  // projection L to the ray
        float d2 = L*L - tca*tca;  // square of the distance between the ray and the center of the sphere
        if (d2 > radius*radius) return false;  // the ray does not intersect the sphere
        float thc = sqrtf(radius*radius - d2);  // distance between the center's projection & 1th ray's intersection
        t0       = tca - thc;  // 1st intersection
        float t1 = tca + thc;  // 2nd intersection
        if (t0 < 0) t0 = t1;  // the camera is inside the sphere & does not see the 1st intersection
        if (t0 < 0) return false;
        point_material = this->material;
        return true;
    }
};

struct Parallelepiped: public Shape {
    Vec3f center;
    float height, width, depth;
    Material material;

    Vec3f get_center() const override { return center; };


    Parallelepiped(const Vec3f &c, const float h, const float w, const float d, const Material &m) :
        center(c), height(h), width(w), depth(d), material(m) {
        this->bounds[0] = Vec3f(center.x - width/2, center.y - height/2, center.z - depth/2);
        this->bounds[1] = Vec3f(center.x + width/2, center.y + height/2, center.z + depth/2);
    }

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0, Material &point_material) const override {
        float txmin, txmax, tymin, tymax, tzmin, tzmax;
        // todo zero division
        Vec3f invdir = Vec3f {1/dir.x, 1/dir.y, 1/dir.z};
        int sign[3];
        sign[0] = (invdir.x < 0);
        sign[1] = (invdir.y < 0);
        sign[2] = (invdir.z < 0);

        // t = time = distance
        txmin = (bounds[sign[0]].x - orig.x) * invdir.x;
        txmax = (bounds[1 - sign[0]].x - orig.x) * invdir.x;
        tymin = (bounds[sign[1]].y - orig.y) * invdir.y;
        tymax = (bounds[1-sign[1]].y - orig.y) * invdir.y;

        if ((txmin > tymax) || (tymin > txmax)) return false;
        if (tymin > txmin)  txmin = tymin;
        if (tymax < txmax) txmax = tymax;

        tzmin = (bounds[sign[2]].z - orig.z) * invdir.z;
        tzmax = (bounds[1-sign[2]].z - orig.z) * invdir.z;

        if ((txmin > tzmax) || (tzmin > txmax)) return false;
        if (tzmin > txmin) txmin = tzmin;
        if (tzmax < txmax) txmax = tzmax;

        t0 = txmin;
        if (t0 < 0) {
            t0 = txmax;
            if (t0 < 0) return false;
        }
        point_material = this->material;
        return true;
    }

private:
    Vec3f bounds[2];
};


struct TexturedParallelepiped: public Shape {
    Vec3f center;
    float height, width, depth;
    unsigned char *texture;
    int picture_width, picture_height;

    Vec3f get_center() const override { return center; };

    TexturedParallelepiped(const Vec3f &c, const float h, const float w, const float d, const char *texture_path):
            center(c), height(h), width(w), depth(d) {
        this->bounds[0] = Vec3f(center.x - width/2, center.y - height/2, center.z - depth/2);
        this->bounds[1] = Vec3f(center.x + width/2, center.y + height/2, center.z + depth/2);
        texture = stbi_load(texture_path, &picture_width, &picture_height, NULL, 3);
        if (texture == NULL){
            std::cout<<"texture file '"<<texture_path<<"' not found";
            exit(1);
        }
        //todo call stbi_write_png(texture) in destructor
    }

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0, Material &point_material) const override {
        float txmin, txmax, tymin, tymax, tzmin, tzmax;
        float side = 0, side_max = 0;  // параллельно x
        // todo zero division
        Vec3f invdir = Vec3f {1/dir.x, 1/dir.y, 1/dir.z};
        int sign[3];
        sign[0] = (invdir.x < 0);
        sign[1] = (invdir.y < 0);
        sign[2] = (invdir.z < 0);

        // t = time: t * camera_direction = cube_surface
        txmin = (bounds[sign[0]].x - orig.x) * invdir.x;
        txmax = (bounds[1 - sign[0]].x - orig.x) * invdir.x;
        tymin = (bounds[sign[1]].y - orig.y) * invdir.y;
        tymax = (bounds[1-sign[1]].y - orig.y) * invdir.y;

        if ((txmin > tymax) || (tymin > txmax)) return false;
        if (tymin > txmin)  {txmin = tymin; side = 1; }  // параллельно y
        if (tymax < txmax) {txmax = tymax; side_max = 1; }

        tzmin = (bounds[sign[2]].z - orig.z) * invdir.z;
        tzmax = (bounds[1-sign[2]].z - orig.z) * invdir.z;

        if ((txmin > tzmax) || (tzmin > txmax)) return false;
        if (tzmin > txmin) {txmin = tzmin; side = 2; }  // параллельно z
        if (tzmax < txmax) {txmax = tzmax; side_max = 2; }

        t0 = txmin;
        if (t0 < 0) {
            t0 = txmax;
            if (t0 < 0) return false;
            side = side_max;
        }

        int texture_x=0, texture_y=0;
        if (side == 0){
            texture_x = (int) (((orig.z + t0 * dir.z - center.z) +  width/2) / width * picture_width);
            texture_y = picture_height - 1 - (int) (((orig.y + t0 * dir.y - center.y) + height/2) / height * picture_height);
        }
        else if (side == 1) {
            texture_x = (int) (((orig.x + t0 * dir.x - center.x) +  width/2) / width * picture_width);
            texture_y = picture_height - 1 - (int) (((orig.z + t0 * dir.z - center.z) + height/2) / height * picture_height);
        }
        else if (side == 2) {
            texture_x = (int) (((orig.x + t0 * dir.x - center.x) +  width/2) / width * picture_width);
            texture_y = picture_height - 1 - (int) (((orig.y + t0 * dir.y - center.y) + height/2) / height * picture_height);
        }

        if ((texture_x < 0) || (texture_y < 0) || (texture_x >= picture_width) || (texture_y >= picture_height)) {
            point_material = get_texture_material(Vec3f (1,1,1));
            return true;
        }

        Vec3f diffuse_color = Vec3f (texture[(texture_x + texture_y * picture_width) * 3 + 0] / 256.,
                                 texture[(texture_x + texture_y * picture_width) * 3 + 1] / 256.,
                                 texture[(texture_x + texture_y * picture_width) * 3 + 2] / 256.);
        point_material = get_texture_material(diffuse_color);
        return true;
    }

private:
    Vec3f bounds[2];
};

#endif //RAY_TRACING_SHAPES_H

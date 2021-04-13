#ifndef RAY_TRACING_SHAPES_H
#define RAY_TRACING_SHAPES_H

#include "material.h"

struct Shape {
public:
    /* orig - camera position
     * dir - direction of gaze from the camera
     * t0 - distance to nearest intersection */
    virtual bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0, Material &material) const = 0;

    virtual Vec3f get_center() const = 0;
};

struct Sphere: public Shape {
    Vec3f center;
    float radius;
    Material material;

    Vec3f get_center() const override { return center; };

    Sphere(const Vec3f &c, const float r, const Material &m) : center(c), radius(r), material(m) {}

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0, Material &material) const override {
        Vec3f L = center - orig;  // the vector from camera to the center of the sphere
        float tca = L*dir;  // projection L to the ray
        float d2 = L*L - tca*tca;  // square of the distance between the ray and the center of the sphere
        if (d2 > radius*radius) return false;  // the ray does not intersect the sphere
        float thc = sqrtf(radius*radius - d2);  // distance between the center's projection & 1th ray's intersection
        t0       = tca - thc;  // 1st intersection
        float t1 = tca + thc;  // 2nd intersection
        if (t0 < 0) t0 = t1;  // the camera is inside the sphere & does not see the 1st intersection
        if (t0 < 0) return false;
        material = this->material;
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

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0, Material &material) const override {
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
        material = this->material;
        return true;
    }

private:
    Vec3f bounds[2];
};

#endif //RAY_TRACING_SHAPES_H

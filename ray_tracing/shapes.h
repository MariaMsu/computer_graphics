#ifndef RAY_TRACING_SHAPES_H
#define RAY_TRACING_SHAPES_H

struct Material {
    Material(const float r, const Vec4f &a, const Vec3f &color, const float spec) : refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
    Material() : refractive_index(1), albedo(1,0,0,0), diffuse_color(), specular_exponent() {}
    float refractive_index;
    Vec4f albedo;
    Vec3f diffuse_color;
    float specular_exponent;
};

struct Shape {
public:
    virtual bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const = 0;
    virtual Material get_material() const = 0;
    virtual Vec3f get_center() const = 0;
};

struct Sphere: public Shape {
    Vec3f center;
    float radius;
    Material material;

    Material get_material() const override { return material; };
    Vec3f get_center() const override { return center; };

    Sphere(const Vec3f &c, const float r, const Material &m) : center(c), radius(r), material(m) {}

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const override {
        /* orig - camera position
         * dir - direction of gaze from the camera
         */
        Vec3f L = center - orig;  // the vector from camera to the center
        float tca = L*dir;
        float d2 = L*L - tca*tca;
        if (d2 > radius*radius) return false;
        float thc = sqrtf(radius*radius - d2);
        t0       = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return false;
        return true;
    }
};

struct Cylinder {
    Vec3f center;
    float radius;
    Material material;


    Cylinder(const Vec3f &c, const float r, const Material &m) : center(c), radius(r), material(m) {}

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const {
        /* orig - camera position
         * dir - direction of gaze from the camera
         */
        Vec3f L = center - orig;  // the vector from camera to the center
        float tca = L*dir;
        float d2 = L*L - tca*tca;
        if (d2 > radius*radius) return false;
        float thc = sqrtf(radius*radius - d2);
        t0       = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return false;
        return true;
    }
};

#endif //RAY_TRACING_SHAPES_H
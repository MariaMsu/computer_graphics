#ifndef RAY_TRACING_MATERIAL_H
#define RAY_TRACING_MATERIAL_H

struct Material {
    Material(const float r, const Vec4f &a, const Vec3f &color, const float spec) :
            refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
    Material() : refractive_index(1), albedo(1,0,0,0), diffuse_color(), specular_exponent() {}
    float refractive_index;  // преломление
    // albedo[0]: интенсивность цвета
    // albedo[1]: интенсивность отраженного света (бликов)
    // albedo[2]: интенсивность пропускаемого цвета (прозрачность)
    // albedo[3]: интенсивность пропускаемого света (прозрачность)
    Vec4f albedo{};
    Vec3f diffuse_color{};  // material color {red, green, blue}; {1, 1, 1} is white
    float specular_exponent;  // яркость отраженного света
};

const Material ivory(1.0, Vec4f(0.6, 0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3), 50.);
const Material glass(1.5, Vec4f(0.0, 0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8), 125.);
const Material red_rubber(1.0, Vec4f(0.9, 0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1), 10.);
const Material mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

Material get_texture_material(Vec3f colour){
    return Material{1.0, Vec4f(0.4, 0.0, 0.0, 0.0), colour, 0.};
}
//const Material material_for_texture
//    (1.0, Vec4f(0.4, 0.0, 0.0, 0.0), Vec3f(0.6, 0.7, 0.8), 0.);

#endif //RAY_TRACING_MATERIAL_H

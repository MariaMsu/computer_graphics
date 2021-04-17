#ifndef RAY_TRACING_CONFIG_H
#define RAY_TRACING_CONFIG_H

#define H_SCREEN_WIDTH 512
#define H_SCREEN_HEIGHT 512
#define H_FIELD_OF_VIEW M_PI / 3.  // angle
#define H_RECURSION_DEPTH 1

#define TEXTURE_SAND "../resources/sand.jpg"
#define TEXTURE_GROUND "../resources/ground.jpg"
#define TEXTURE_WATER "../resources/water.jpg"

const Vec3f background_colour = Vec3f(240. / 256., 170. / 256., 250. / 256.);

#endif //RAY_TRACING_CONFIG_H

#ifndef MAIN_UTILSSTRUCTURES_H
#define MAIN_UTILSSTRUCTURES_H

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct Point {
    // координаты в пикселях
    int x;
    int y;
};

struct PointT {
    // координаты в тайтлах
    int x;
    int y;
};

#endif //MAIN_UTILSSTRUCTURES_H

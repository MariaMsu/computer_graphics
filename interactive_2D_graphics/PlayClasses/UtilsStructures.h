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

struct ObjectBorders {
    explicit ObjectBorders(int x_left, int x_right, int y_low, int y_heigh) :
            x_left(x_left), x_right(x_right), y_low(y_low), y_heigh(y_heigh) {};
    int x_left;
    int x_right;
    int y_low;
    int y_heigh;
};

#endif //MAIN_UTILSSTRUCTURES_H

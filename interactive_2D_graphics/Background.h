#ifndef MAIN_BACKGROUND_H
#define MAIN_BACKGROUND_H

#include <utility>
#include <vector>
#include <fstream>
#include <array>
#include <iostream>
#include "Image.h"
#include "Player.h"
#include "Constants.h"


struct Background {
    explicit Background(std::string background_path);

    void Draw(Image &screen);

private:
//    Point coords{.x = 10, .y = 10};
//    Point old_coords{.x = 10, .y = 10};
//    Pixel color{.r = 255, .g = 255, .b = 0, .a = 255};
//    int move_speed = 4;
    std::string background_path;
    std::vector<std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT>> background_vector;

};

#endif // MAIN_BACKGROUND_H

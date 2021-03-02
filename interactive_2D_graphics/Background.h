#ifndef MAIN_BACKGROUND_H
#define MAIN_BACKGROUND_H

#include <utility>
#include <vector>
#include <fstream>
#include <array>
#include <iostream>
#include <memory>
#include "Image.h"
#include "Player.h"
#include "Constants.h"


struct Background {
    explicit Background(std::string map_path, std::string  titles_path);

    void Draw(Image &screen);

private:
    std::string map_path;
    std::string titles_path;
    std::vector<std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT>> background_vector;
    std::vector<std::shared_ptr<Image>> titles_vector;
    Pixel *background_state = nullptr;
};

#endif // MAIN_BACKGROUND_H

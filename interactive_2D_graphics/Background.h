#ifndef MAIN_BACKGROUND_H
#define MAIN_BACKGROUND_H

#include <utility>
#include <vector>
#include <fstream>
#include <array>
#include <iostream>
#include <memory>
#include <cstring>
#include "Image.h"
#include "Player.h"
#include "Constants.h"
#include "ScreanState.h"

typedef std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT> BackgroundMap;

struct Background {
    explicit Background(std::string map_path, std::string  titles_path);

    void DrawRoom(Image &screen, ScreenState &screen_state, int room_number);

private:
    std::string map_path;
    std::string titles_path;
    std::vector<BackgroundMap> map_vector;
    std::vector<std::shared_ptr<Image>> titles_vector;  // todo remove shared?
    int room_number = -1;
};

#endif // MAIN_BACKGROUND_H

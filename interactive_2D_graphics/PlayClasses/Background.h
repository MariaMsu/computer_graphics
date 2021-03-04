#ifndef MAIN_BACKGROUND_H
#define MAIN_BACKGROUND_H

#include <utility>
#include <map>
#include <fstream>
#include <iostream>
#include <memory>
#include <cstring>
#include "Image.h"
#include "Player.h"
#include "../Constants.h"
#include "GlobalState.h"

struct Background {
    explicit Background(std::string  titles_path);

    void DrawRoom(Image &screen, GlobalState &screen_state);

private:
    std::string map_path;
    std::string titles_path;
    std::map<int, std::shared_ptr<Image>> titles_map;
};

#endif // MAIN_BACKGROUND_H

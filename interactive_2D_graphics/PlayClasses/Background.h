#ifndef MAIN_BACKGROUND_H
#define MAIN_BACKGROUND_H

#include <map>
#include <fstream>
#include "Image.h"
#include "../Constants.h"
#include "GlobalState.h"

struct Background {
    explicit Background(std::string  titles_path);

    void DrawRoom(Image &screen, std::shared_ptr<TitleMap> &background_map);

private:
    std::string map_path;
    std::string titles_path;
    std::map<int, std::shared_ptr<Image>> titles_map;
};

#endif // MAIN_BACKGROUND_H

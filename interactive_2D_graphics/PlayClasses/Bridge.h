#ifndef MAIN_BRIDGE_H
#define MAIN_BRIDGE_H

#include <string>
#include <vector>
#include <array>
#include <memory>
#include "Image.h"
#include "GlobalState.h"

struct Bridge {
    explicit Bridge(const std::string &titles_path);
    void DrawBridge(Image &screen, PointT bridge_point);


private:
    static const int n_bridges = 4;
    std::array<std::shared_ptr<Image>, n_bridges> bridges_array;
};

#endif //MAIN_BRIDGE_H

#include <fstream>
#include "Bridge.h"
#include "Utils.h"

Bridge::Bridge(const std::string &titles_path) {
    std::string path;
    std::ifstream title_file(titles_path);
    if (!title_file.is_open()) {
        std::cerr << "Unable to open title's path file " << titles_path << "\n";
        exit(6);
    }
    int i;
    for (i = 0; std::getline(title_file, path), i < n_bridges; ++i) {
        bridges_array[i] = std::make_shared<Image>(path);
    }
    if (i < n_bridges) {
        std::cerr << "Found less bridges then " << n_bridges << "\n";
        exit(6);
    }
    title_file.close();
}

void Bridge::DrawBridge(Image &screen, PointT bridge_point) {
    int direction = getTransitionDirection(bridge_point);
    assert((1<=direction) && (direction<=4));
    const std::shared_ptr<Image> &title = this->bridges_array[direction-1];
    int global_x, global_y ;
    switch (direction) {  // todo replace macros
        case 1:
            global_x = bridge_point.x * h_TEXTURE_SIZE;
            global_y = bridge_point.y * h_TEXTURE_SIZE;
            break;
        case 2:
            global_x = bridge_point.x * h_TEXTURE_SIZE;
            global_y = bridge_point.y * h_TEXTURE_SIZE;
            break;
        case 3:
            global_x = bridge_point.x * h_TEXTURE_SIZE;
            global_y = bridge_point.y * h_TEXTURE_SIZE;
            break;
        case 4:
            global_x = bridge_point.x * h_TEXTURE_SIZE;
            global_y = bridge_point.y * h_TEXTURE_SIZE;
            break;
        default:
            global_x = 0, global_y = 0;
    }
    std::cout<<"global_x "<<global_x<<", global_y "<<global_y<<", direction "<<direction<<"\n";
    drawTrSaveAsset(screen, title, global_x, global_y);
}

#include <fstream>
#include <iostream>
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

void Bridge::DrawBridge(Image &screen, GlobalState &screen_state, int direction, int position) {
    const std::shared_ptr<Image> &title = this->bridges_array[direction-1];
    drawAsset(screen, title, 200, 200);
}

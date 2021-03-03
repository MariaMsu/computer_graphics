#ifndef MAIN_GLOBALSTATE_H
#define MAIN_GLOBALSTATE_H

#include <vector>
#include <iostream>
#include <fstream>
#include "../Constants.h"
#include "Image.h"
#include <filesystem>
namespace fs = std::filesystem;

struct GlobalState {

    GlobalState(const std::string& rooms_data_path);

    Pixel *background_state = new Pixel[h_WINDOW_HEIGHT * h_WINDOW_WIDTH];
    BackgroundMap* background_map = nullptr;


    ~GlobalState() {
        delete[] background_state;
    }

private:
    std::vector<std::shared_ptr<BackgroundMap>> background_map_vector;
};

#endif //MAIN_GLOBALSTATE_H

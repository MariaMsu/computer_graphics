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

    explicit GlobalState(const std::string& rooms_data_path);

    void SetRoom(int room_number);
    // todo there must to be getter
    std::shared_ptr<TitleMap> room_background_map;
    std::shared_ptr<TitleMap> room_objects_map;
    std::shared_ptr<TransitionsData> room_transitions_data;

    Pixel *background_state = new Pixel[h_WINDOW_HEIGHT * h_WINDOW_WIDTH];
    TitleMap* background_map = nullptr;


    ~GlobalState() {
        delete[] background_state;
    }

private:
    std::vector<std::shared_ptr<TitleMap>> background_map_vector;
    std::vector<std::shared_ptr<TitleMap>> objects_map_vector;
    std::vector<std::shared_ptr<TransitionsData>> transitions_data_vector;
    int n_rooms;
};

#endif //MAIN_GLOBALSTATE_H

#ifndef MAIN_GLOBALSTATE_H
#define MAIN_GLOBALSTATE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "../Constants.h"
#include "Image.h"

struct GlobalState {

    explicit GlobalState(const std::string &rooms_data_path);
    void SetRoom(int room_number);
    void PutBridge(int direction) { bridges_state[direction - 1] = true; };
    void SetTransitionDirection(int direction) {
        if (bridges_state[direction - 1]) { this->transition_direction = direction; }
    };

    int GetTransitionDirection(){return transition_direction;}

    // todo there must to be getter?
    std::shared_ptr<TitleMap> room_background_map;

    Pixel *background_state = new Pixel[h_WINDOW_HEIGHT * h_WINDOW_WIDTH];


    int GetNewRoomNumber();

    ~GlobalState() {
        delete[] background_state;
    }

private:
    std::shared_ptr<TransitionsData> room_transitions_data;
    TransitionsData bridges_state;
    std::shared_ptr<TitleMap> room_objects_map; // todo remove?

    //    void _CheckTransitions();
    std::vector<std::shared_ptr<TitleMap>> background_map_vector;
    std::vector<std::shared_ptr<TitleMap>> objects_map_vector;
    std::vector<std::shared_ptr<TransitionsData>> transitions_data_vector;
    int current_room;
    int n_rooms;
    int transition_direction = 0;
};

#endif //MAIN_GLOBALSTATE_H

#ifndef MAIN_GLOBALSTATE_H
#define MAIN_GLOBALSTATE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "../Constants.h"
#include "Utils.h"

struct GlobalState {

    explicit GlobalState(const std::string &rooms_data_path);
    void PutBridge(int direction) { bridges_state[direction - 1] = true; };
    void SetTransitionDirection(int direction) {
        if (bridges_state[direction - 1]) { this->transition_direction = direction; }
    };
    int GetTransitionDirection() const { return transition_direction; }
    bool SwitchRoom();

    Point GetInitPlayerPosition() const { return init_player_position; };

    // todo there must to be getter?
    std::shared_ptr<TitleMap> room_background_map;

    Pixel *background_state = new Pixel[h_WINDOW_HEIGHT * h_WINDOW_WIDTH];


    ~GlobalState() {
        delete[] background_state;
    }

private:
    void _ReassigneState(int room_number);

    std::shared_ptr<TransitionsData> room_transitions_data;
    TransitionsData bridges_state{false, false, false, false};
    std::shared_ptr<TitleMap> room_objects_map; // todo remove?

    //    void _CheckTransitions();
    std::vector<std::shared_ptr<TitleMap>> background_map_vector;
    std::vector<std::shared_ptr<TitleMap>> objects_map_vector;
    std::vector<std::shared_ptr<TransitionsData>> transitions_data_vector;
    int current_room = 0;
    int n_rooms;
    int transition_direction = 0;
    Point init_player_position{h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
};

#endif //MAIN_GLOBALSTATE_H

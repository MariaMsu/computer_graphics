#ifndef MAIN_GLOBALSTATE_H
#define MAIN_GLOBALSTATE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "../Constants.h"
#include "UtilsStructures.h"

struct GlobalState {

    explicit GlobalState(const std::string &rooms_data_path);
    void PutBridge(int direction) {
        bridges_state[direction - 1] = true;
        draw_bridge = true;
    };
    void SetTransitionDirection(int direction) {
        tmp_transition_direction = direction;
        if (bridges_state[direction - 1]) {this->transition_direction = direction; }
    };
    int GetTransitionDirection() const { return tmp_transition_direction; }
    int GetTransitionPosition() const {
        if (transition_direction <= 0){
            return 10;
        }
        return (*transitions_pos_vector[current_room])[transition_direction-1];
    }
    bool SwitchRoom();

    Point GetInitPlayerPosition() const { return init_player_position; };

    // todo there must to be getter?
    std::shared_ptr<TitleMap> room_background_map;
    bool draw_bridge = false;

private:
    int tmp_transition_direction = 0;
    void _ReassigneState(int room_number);

    std::shared_ptr<TransitionsData> room_transitions_data;
    TransitionsData bridges_state{false, false, false, false};
    std::shared_ptr<TitleMap> room_objects_map; // todo remove?

    //    void _CheckTransitions();
    std::vector<std::shared_ptr<TitleMap>> background_map_vector;
    std::vector<std::shared_ptr<TitleMap>> objects_map_vector;
    std::vector<std::shared_ptr<TransitionsData>> transitions_data_vector;
    std::vector<std::shared_ptr<TransitionsData>> transitions_pos_vector;
    int current_room = 0;
    int n_rooms;
    int transition_direction = 0;
    Point init_player_position{h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
};

#endif //MAIN_GLOBALSTATE_H

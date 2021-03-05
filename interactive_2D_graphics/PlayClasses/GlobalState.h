#ifndef MAIN_GLOBALSTATE_H
#define MAIN_GLOBALSTATE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>
#include "../Constants.h"
#include "UtilsStructures.h"


struct GlobalState {

    explicit GlobalState(const std::string &rooms_data_path);

    void PushStateBridge(int transition_num);
    bool PopStateBridge(PointT& p);

    bool SetTransitionDirection(int direction);
    int GetTransitionDirection() const { return transition_direction; }

    bool SwitchRoom();

    Point GetInitPlayerPosition() const { return init_player_position; };

    // todo there must to be getter?
    std::shared_ptr<TitleMap> room_background_map;
    std::shared_ptr<std::vector<PointT>> room_transitions_points;

private:
    void reassigneState(int room_number);

    // in state
    int transition_direction = 0;  // [1..4]

    // out state
    PointT bridge_point = {};

    // in & out sate

    // inner
    bool switch_room = false;
    bool draw_bridge = false;
    std::shared_ptr<TransitionsData> room_transitions_data;
    TransitionsData bridges_state{false, false, false, false};
    std::shared_ptr<TitleMap> room_objects_map; // todo remove?

    // all rooms data
    std::vector<std::shared_ptr<TitleMap>> background_map_vector;
    std::vector<std::shared_ptr<TitleMap>> objects_map_vector;
    std::vector<std::shared_ptr<TransitionsData>> transitions_data_vector;
    std::vector<std::shared_ptr<std::vector<PointT>>> transitions_points_vector;
//    std::vector<std::shared_ptr<TransitionsData>> transitions_pos_vector;
    int current_room = 0;
    int n_rooms;
    Point init_player_position{h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
};

#endif //MAIN_GLOBALSTATE_H

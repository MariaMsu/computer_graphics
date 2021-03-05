#ifndef MAIN_GLOBALSTATE_H
#define MAIN_GLOBALSTATE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>
#include "../Constants.h"
#include "UtilsStructures.h"
#include "Utils.h"


struct GlobalState {
    explicit GlobalState(const std::string &rooms_data_path);

    void PushStateBridge(int transition_num);
    bool PopStateBridge(PointT& p);

    void PushStateRoom(Point player);
    bool PopStateRoom(Point &player_position);

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
    bool update_room = false;
    int room_new_ind = 0;
    bool update_bridge = false;
    std::shared_ptr<std::vector<int>> room_transitions_data;
    std::vector<bool> bridges_state;
    std::shared_ptr<TitleMap> room_objects_map; // todo remove?

    // all rooms data
    std::vector<std::shared_ptr<TitleMap>> background_map_vector;
    std::vector<std::shared_ptr<TitleMap>> objects_map_vector;
    std::vector<std::shared_ptr<std::vector<int>>> transitions_data_vector;
    std::vector<std::shared_ptr<std::vector<PointT>>> transitions_points_vector;
//    std::vector<std::shared_ptr<TransitionsData>> transitions_pos_vector;
    int room_ind = 0;
    int n_rooms;
    Point init_player_position{h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
};

#endif //MAIN_GLOBALSTATE_H

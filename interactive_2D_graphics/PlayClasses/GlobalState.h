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

struct RoomInfo{
    int logs_number;
    std::vector<int> transition_rooms;
    std::vector<PointT> transition_points;

    std::array<int, h_N_ROOM_SIDES> _last_i;  // todo rename
};

struct GlobalState {
    explicit GlobalState(const std::string &rooms_data_path, int init_room_ind=0);

    void PushStateBridge(int transition_num);
    bool PopStateBridge(PointT& p);

    void PushStateRoom(Point player);
    bool PopStateRoom(Point &player_position);

    void PushStateLogs(int nearest_point_ind);
    bool PopStateLogs(int & removing_ind);

    void PushStateEnd(int end_state);
    bool PopStateEnd(int& end_state);

    // todo there must to be getter?
    std::shared_ptr<TitleMap> room_background_map;
    std::vector<PointT> log_points;
    // todo да, нужно выпилить &
    std::vector<PointT> &GetTransitionsPoints(){ return room_info->transition_points; }
    int GetRoomInd(){ return room_ind; }
    int GetLogsNumber(){ return room_info->logs_number; }

private:
    void reassigneState(int room_number);
    Point getNewPlayerPosition(int old_room_ind);

    // in state

    // out state
    PointT bridge_point = {};

    // in & out sate

    // inner
    int logs_counter;
    int transition_direction = 0;  // [1..4]
    int room_new_ind = 0;
    bool update_room = false;
    bool update_bridge = false;
    int remove_logs_by_ind = -1;
    int end_game = 0;
    std::shared_ptr<RoomInfo> room_info;
    std::vector<bool> bridges_state;

    // all rooms data
    std::vector<std::shared_ptr<TitleMap>> background_map_vector;
    std::vector<std::shared_ptr<RoomInfo>> common_info_vector;
    int room_ind = 0;
};

#endif //MAIN_GLOBALSTATE_H

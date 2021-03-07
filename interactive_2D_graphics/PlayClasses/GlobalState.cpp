#include "GlobalState.h"

std::shared_ptr<TitleMap> readTitleMap(const std::string &title_map_path) {
    std::ifstream input_stream(title_map_path);
    if (!input_stream.is_open()) {
        std::cout << "Unable to open room file " << title_map_path << "\n";
        exit(1);
    }

    std::shared_ptr<TitleMap> title_map = std::make_shared<TitleMap>();
    std::string line;
    int i;
    for (i=0; getline(input_stream, line); ++i) {
        if (line.length() != h_WINDOW_T_WIDTH * h_MAP_CODE_SIZE) {
            std::cout << title_map_path << " incorrect h_WINDOW_WIDTH "
                      << line.length() << " != " << h_WINDOW_T_WIDTH * h_MAP_CODE_SIZE << "\n";
            exit(1);
        }
        for (int j = 0; j < h_WINDOW_T_WIDTH; ++j) {
            // title_map's items belong to [0,...N]
            (*title_map)[h_WINDOW_T_HEIGHT - i - 1][j] =
                    std::stoi(line.substr(j * h_MAP_CODE_SIZE, h_MAP_CODE_SIZE));
        }
    }
    if (i != h_WINDOW_T_HEIGHT) {
        std::cout << title_map_path << " incorrect h_WINDOW_T_HEIGHT "
                  << i << " != " << h_WINDOW_T_HEIGHT << "\n";
        exit(1);
    }
    return title_map;
}

std::shared_ptr<RoomInfo> readRoomInfo(const std::string &transitions_path) {
    std::ifstream input_stream(transitions_path);
    if (!input_stream.is_open()) {
        std::cerr << "Unable to open room file " << transitions_path << "\n";
        exit(2);
    }
    std::shared_ptr<RoomInfo> room_info = std::make_shared<RoomInfo>();
    std::string line;
    if(!getline(input_stream, line) || line!="logs_number:"){
        std::cerr << "Incorrect room info format: 'logs_number:'. " << transitions_path << "\n";
        exit(2);
    }
    getline(input_stream, line);
    room_info->logs_number = std::stoi(line);
    if(0 > room_info->logs_number){
        std::cerr << "Incorrect room info format: logs_number. " << transitions_path << "\n";
        exit(2);
    }
    if(!getline(input_stream, line) || line!="transitions:"){
        std::cerr << "Incorrect room info format: 'transitions:'. " << transitions_path << "\n";
        exit(2);
    }
    for (int i = 0; getline(input_stream, line), i < h_N_ROOM_SIDES; ++i) {
        if (i != 0) { room_info->_last_i[i-1] = room_info->transition_rooms.size();}
        if (line == "-") { continue; }  // empty wall
        if (line.length() % h_MAP_CODE_SIZE != 0) {
            std::cout << transitions_path << " incorrect h_N_TRANSITIONS "<< line.length() << " not multiple 3\n";
            exit(2);
        }
        for (int j = 0; j < line.length() / h_MAP_CODE_SIZE; ++j) {
            int room = std::stoi(line.substr(j * h_MAP_CODE_SIZE, h_MAP_CODE_SIZE));
            if(0 > room || room >= h_N_ROOMS) {
                std::cerr << "Incorrect room info format: room_transition. " << transitions_path << "\n";
                exit(2);
            }
            room_info->transition_rooms.push_back(room);
        }
    }
    room_info->_last_i[h_N_ROOM_SIDES-1] = room_info->transition_rooms.size();
    getline(input_stream, line); // end of file can change the return value
    if (line!="end.") {
        std::cerr << "Incorrect room info format: 'end.'. " << transitions_path << "\n";
        exit(2);
    }
    input_stream.close();
    return room_info;
}


void setTransitionPoints(std::shared_ptr<RoomInfo> &room_info, std::shared_ptr<TitleMap> &background_map) {
    int previous_n_transition = 0;
    for (int j = 0; j < h_WINDOW_T_WIDTH; ++j) {
        if ((*background_map)[h_WINDOW_T_HEIGHT - 1][j] == h_LAVA_CENTRE) {
            room_info->transition_points.push_back(PointT{j, h_WINDOW_T_HEIGHT - 1});
        }
    }
    assert(room_info->transition_points.size() - previous_n_transition == room_info->_last_i[0]);
    previous_n_transition == room_info->transition_points.size();
    for (int i = 0; i < h_WINDOW_T_HEIGHT; ++i) {
        if ((*background_map)[i][h_WINDOW_T_WIDTH - 1] == h_LAVA_CENTRE) {
            room_info->transition_points.push_back(PointT{h_WINDOW_T_WIDTH - 1, i});
        }
    }
    assert(room_info->transition_points.size() - previous_n_transition == room_info->_last_i[1]);
    previous_n_transition == room_info->transition_points.size();
    for (int j = 0; j < h_WINDOW_T_WIDTH; ++j) {
        if ((*background_map)[0][j] == h_LAVA_CENTRE) {
            room_info->transition_points.push_back(PointT{j, 0});
        }
    }
    assert(room_info->transition_points.size() - previous_n_transition == room_info->_last_i[2]);
    previous_n_transition == room_info->transition_points.size();
    for (int i = 0; i < h_WINDOW_T_HEIGHT; ++i) {
        if ((*background_map)[i][0] == h_LAVA_CENTRE) {
            room_info->transition_points.push_back(PointT{0, i});
        }
    }
    assert(room_info->transition_points.size() - previous_n_transition == room_info->_last_i[3]);
}


GlobalState::GlobalState(const std::string &rooms_data_path) {
    for (int i = 0; i < h_N_ROOMS; ++i) {
        std::string single_room_path = rooms_data_path + "/r" + std::to_string(i);
        std::clog<<"load room " << single_room_path << "\n";
        this->background_map_vector.push_back(
                readTitleMap(single_room_path + "/background_map.txt"));
        std::shared_ptr<RoomInfo> cur_room_info = readRoomInfo(single_room_path + "/info.txt");
        setTransitionPoints(cur_room_info, background_map_vector.back());
        this->common_info_vector.push_back(cur_room_info);
    }
    this->logs_counter = h_INIT_LOGS_NUMBER;
    this->reassigneState(0);
}

void GlobalState::reassigneState(int room_number) {
    if ((room_number < 0) || (room_number >= h_N_ROOMS)) {
        std::cerr << "room_number must to be: 0 <" << room_number << " <= " << h_N_ROOMS<< "\n";
        exit(3);
    }
    this->room_background_map = this->background_map_vector[room_number];
    this->room_info = this->common_info_vector[room_number];
    this->bridges_state.clear();
    for (int i = 0; i < room_info->transition_points.size(); ++i) {
        bridges_state.push_back(false);
    }
    this->log_points.clear();
    this->room_ind = room_number;
    this->transition_direction = 0;
    this->update_room = false;
    this->update_bridge = false;
    this->remove_logs_by_ind = -1;
}

Point GlobalState::getNewPlayerPosition(int old_room_ind) {
    // should be called after 'room_transitions_data' updating
    for(int i = 0; i < room_info->_last_i.back(); ++i){
        // ищем переход из новой комнаты в старую
        if (room_info->transition_rooms[i] != old_room_ind) {continue;}
        PointT point = room_info->transition_points[i];
//        std::cout<<"transition point x="<<point.x<<", y="<<point.y<<"\n";
        if (point.y == h_WINDOW_T_HEIGHT-1) { return PointT2Point(
                PointT{point.x, point.y - h_PLAYER_NEW_POINT_SHIFT}); }
        if (point.x == h_WINDOW_T_WIDTH-1) { return PointT2Point(
                PointT{point.x - h_PLAYER_NEW_POINT_SHIFT, point.y}); }
        if (point.y == 0) { return PointT2Point(
                PointT{point.x, point.y+h_PLAYER_NEW_POINT_SHIFT}); }
        if (point.x == 0) { return PointT2Point(
                PointT{point.x+h_PLAYER_NEW_POINT_SHIFT, point.y}); }
        std::clog<<"Incorrect transition point x="<<point.x<<", y="<<point.y<<"\n";
        return Point{h_WINDOW_WIDTH / 2, h_WINDOW_HEIGHT / 2};
    }
    std::clog<<"Transition point not found\n";
    return Point{h_WINDOW_WIDTH / 2, h_WINDOW_HEIGHT / 2};
}

void GlobalState::PushStateBridge(int transition_num) {
    assert((0 <= transition_num) && (transition_num < bridges_state.size()));
    if (bridges_state[transition_num]){return;} // bridge already set
    if (logs_counter <= 0){
        std::cout<<"USER: no logs left\n";
        return;
    }
    update_bridge = true;
    bridges_state[transition_num] = true;
    bridge_point = room_info->transition_points[transition_num];
}

bool GlobalState::PopStateBridge(PointT &p) {
    if (!update_bridge) { return false; }
    p = bridge_point;
    update_bridge = false;
    logs_counter -= 1;
    std::cout<<"USER: "<<logs_counter<<" logs left\n";
    return true;
}

void GlobalState::PushStateRoom(Point player) {
    int nearest_transition;
    double distance = detNearestPointT( // todo make easier
            player, this->room_info->transition_points, nearest_transition);
    assert(distance >= 0); // < 0 if something with arguments is wrong
    if (! this->bridges_state[nearest_transition]) {return;}
    this->transition_direction = getTransitionDirection(room_info->transition_points[nearest_transition]);
    assert((1 <= transition_direction) && (transition_direction <= h_N_ROOM_SIDES));
    update_room = true;
    room_new_ind = this->room_info->transition_rooms[nearest_transition];
    std::clog<<"Transition to the room " << room_new_ind << "\n";
}

bool GlobalState::PopStateRoom(Point &player_position) {
    if (!update_room) { return false; }
    update_room = false;
    int old_room_ind = this->room_ind;
    this->room_ind = room_new_ind;
    reassigneState(room_new_ind);
    // не умариает, если наступил на лаву и сменил комнату
    if (end_game == -1) {end_game = 0;}
    player_position = getNewPlayerPosition(old_room_ind); // todo move
    return true;
}

void GlobalState::PushStateLogs(int nearest_point_ind) { this->remove_logs_by_ind = nearest_point_ind; }

bool GlobalState::PopStateLogs(int &removing_ind) {
    if (this->remove_logs_by_ind < 0){ return false;}
    removing_ind = remove_logs_by_ind;
    logs_counter += 1;
    this->remove_logs_by_ind = -1;
    return true;
}

//void GlobalState::_CheckTransitions() {
//    for(std::shared_ptr<TransitionsData> transitions: this->transitions_data_vector){
//        for (int room: *transitions){
//            TransitionsData target_room_t = *(this->transitions_data_vector[room]);
//            if (std::find(target_room_t.begin(), target_room_t.end(), room) == target_room_t.end()){
//                std::cout<<"room "<<room<<"'s transition data has not transition to "<<
//            };
//        }
//    }
//};

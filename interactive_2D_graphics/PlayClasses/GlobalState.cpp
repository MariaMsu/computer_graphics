
#include "GlobalState.h"

#include <memory>
#include <algorithm>


std::shared_ptr<TitleMap> readTitleMap(const std::string &title_map_path) {
    std::ifstream input_stream(title_map_path);
    if (!input_stream.is_open()) {
        std::cout << "Unable to open room file " << title_map_path << "\n";
        exit(1);
    }

    std::shared_ptr<TitleMap> title_map = std::make_shared<TitleMap>();
    std::string line;
    int i = 0;
    while (getline(input_stream, line)) { // todo i++
        if (line.length() != h_WINDOW_T_WIDTH * h_MAP_CODE_SIZE) {
            std::cout << title_map_path << " incorrect h_WINDOW_WIDTH "
                      << line.length() << " != " << h_WINDOW_T_WIDTH * h_MAP_CODE_SIZE << "\n";
            exit(1);
        }
        for (int j = 0; j < h_WINDOW_T_WIDTH; ++j) {
            // title_map's items belong to [0,...N]
            (*title_map)[h_WINDOW_T_HEIGHT - i - 1][j] =
                    std::stoi(line.substr(j * h_MAP_CODE_SIZE, h_MAP_CODE_SIZE));
        };
        ++i;
    }
    if (i != h_WINDOW_T_HEIGHT) {
        std::cout << title_map_path << " incorrect h_WINDOW_T_HEIGHT "
                  << i << " != " << h_WINDOW_T_HEIGHT << "\n";
        exit(1);
    }
    return title_map;
};

std::shared_ptr<TransitionsData> readTransitions(const std::string &transitions_path) {
    std::ifstream input_stream(transitions_path);
    if (!input_stream.is_open()) {
        std::cout << "Unable to open room file " << transitions_path << "\n";
        exit(2);
    }

    std::shared_ptr<TransitionsData> transitions_data = std::make_shared<TransitionsData>();
    std::string line;
    if (!getline(input_stream, line)) {
        std::cout << "Transitions incorrect " << transitions_path << "\n";
        exit(2);
    }
    input_stream.close();
    if (line.length() != h_N_TRANSITIONS * h_MAP_CODE_SIZE) {
        std::cout << transitions_path << " incorrect h_N_TRANSITIONS "
                  << line.length() << " != " << h_N_TRANSITIONS * h_MAP_CODE_SIZE << "\n";
        exit(2);
    }
    for (int j = 0; j < h_N_TRANSITIONS; ++j) {
        (*transitions_data)[j] =
                std::stoi(line.substr(j * h_MAP_CODE_SIZE, h_MAP_CODE_SIZE));
    };
    return transitions_data;
};

std::shared_ptr<std::vector<PointT>> getTransitionPoints(
        std::shared_ptr<TransitionsData> &transitions_data,
        std::shared_ptr<TitleMap>& background_map) {
    std::shared_ptr<std::vector<PointT>> transition_points = std::make_shared<std::vector<PointT>>();
        for (int j = 0; j < h_WINDOW_T_WIDTH; ++j) {
            if ((*background_map)[h_WINDOW_T_HEIGHT - 1][j] == h_LAVA_CENTRE) {
                transition_points->push_back(PointT{j, h_WINDOW_T_HEIGHT - 1});
            }};
        for (int i = 0; i < h_WINDOW_T_HEIGHT; ++i) {
            if ((*background_map)[i][h_WINDOW_T_WIDTH - 1] == h_LAVA_CENTRE) {
                transition_points->push_back(PointT{h_WINDOW_T_WIDTH - 1, i});
            }};
        for (int j = 0; j < h_WINDOW_T_WIDTH; ++j) {
            if ((*background_map)[0][j] == h_LAVA_CENTRE) {
                transition_points->push_back(PointT{j, 0});
            }};
        for (int i = 0; i < h_WINDOW_T_HEIGHT; ++i) {
            if ((*background_map)[i][0] == h_LAVA_CENTRE) {
                transition_points->push_back(PointT{0, i});
            }};
    return transition_points;
}


GlobalState::GlobalState(const std::string &rooms_data_path) {
    for (int i = 0; i < h_N_ROOMS; ++i) {
        std::string single_room_path = rooms_data_path + "/r" + std::to_string(i);
        this->background_map_vector.push_back(
                readTitleMap(single_room_path + "/background_map.txt"));
        this->objects_map_vector.push_back(
                readTitleMap(single_room_path + "/objects_map.txt"));
        this->transitions_data_vector.push_back(
                readTransitions(single_room_path + "/transitions.txt"));
        this->transitions_points_vector.push_back(getTransitionPoints(
                transitions_data_vector.back(),background_map_vector.back()));
    }
    this->n_rooms = this->background_map_vector.size();
    this->reassigneState(0);
}

void GlobalState::reassigneState(int room_number) {
    if ((room_number < 0) || (room_number >= this->n_rooms)) {
        std::cerr << "room_number must to be: 0 <" << room_number << " <= " << this->n_rooms << "\n";
        exit(3);
    }
    this->room_background_map = this->background_map_vector[room_number];
    this->room_objects_map = this->objects_map_vector[room_number];
    this->room_transitions_data = this->transitions_data_vector[room_number];
    this->room_transitions_points = this->transitions_points_vector[room_number];
    this->current_room = room_number;
    this->transition_direction = 0;
    this->bridges_state = {false, false, false, false};
    this->switch_room = false;
    this->draw_bridge = false;
}

Point getNewPlayerPosition(int transition_direction) {
    switch (transition_direction) {
        case 1: return Point{h_WINDOW_WIDTH / 2, h_TEXTURE_SIZE * 3};
        case 2: return Point{h_WINDOW_WIDTH - h_TEXTURE_SIZE * 3, h_WINDOW_HEIGHT / 2};
        case 3: return Point{h_WINDOW_WIDTH / 2, h_WINDOW_HEIGHT - h_TEXTURE_SIZE * 3};
        case 4: return Point{h_TEXTURE_SIZE * 3, h_WINDOW_HEIGHT / 2};
        default: return {h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
    }
};

bool GlobalState::SwitchRoom() {
    if (! this->switch_room) { return false; }
    int new_room_number = (*this->room_transitions_data)[transition_direction - 1];
    if (new_room_number < 0) {
        std::clog << "From room " << current_room << " address unset room direction " << transition_direction << "\n";
        return false;
    };
    this->init_player_position = getNewPlayerPosition(this->transition_direction);
    reassigneState(new_room_number);
    return true;
}

bool GlobalState::SetTransitionDirection(int direction)  {
    assert((0 < direction) && ( direction <= 4));
    transition_direction = direction;
    if (bridges_state[direction - 1]) {
        this->switch_room = true;
        return true;
    }
    return false;
}

void GlobalState::PushStateBridge(int transition_num) {
    assert((0<=transition_num) && (transition_num < bridges_state.size()));
    bridges_state[transition_num] = true;
    bridge_point = (*room_transitions_points)[transition_num];
    draw_bridge = true;
}

bool GlobalState::PopStateBridge(PointT &p) {
    if (! draw_bridge) {return false;}
    p = bridge_point;
    draw_bridge = false;
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

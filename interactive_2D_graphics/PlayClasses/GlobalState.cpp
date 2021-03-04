
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


GlobalState::GlobalState(const std::string &rooms_data_path) {
    for (int i = 0; i < h_N_ROOMS; ++i) {
        std::string single_room_path = rooms_data_path + "/r" + std::to_string(i);
        this->background_map_vector.push_back(
                readTitleMap(single_room_path + "/background_map.txt"));
        this->objects_map_vector.push_back(
                readTitleMap(single_room_path + "/objects_map.txt"));
        this->transitions_data_vector.push_back(
                readTransitions(single_room_path + "/transitions.txt"));
    }
    this->n_rooms = this->background_map_vector.size();
}

void GlobalState::SetRoom(int room_number) {
    if ((room_number < 0) || (room_number >= this->n_rooms)) {
        std::cout << "room_number must to be: 0 <" << room_number << " <= " << this->n_rooms << "\n";
        exit(3);
    }
    this->room_background_map = this->background_map_vector[room_number];
    this->room_objects_map = this->objects_map_vector[room_number];
    this->room_transitions_data = this->transitions_data_vector[room_number];
    this->current_room = room_number;
    this->transition_direction = 0;
    this->bridges_state = {false, false, false, false};
}

int GlobalState::GetNewRoomNumber() {
    if (this->transition_direction <= 0) {
        return current_room;
    }
    int new_room_number = (*this->room_transitions_data)[transition_direction - 1];
    if (new_room_number < 0) {
        std::clog << "From room " << current_room << " address unset room direction " << transition_direction << "\n";
        return current_room;
    };
    return new_room_number;
};

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

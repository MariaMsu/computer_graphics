
#include "GlobalState.h"

#include <memory>

std::shared_ptr<TitleMap> readTitleMap(const std::string &title_map_path) {
    std::ifstream input_stream(title_map_path);
    if (!input_stream.is_open()) {
        std::cout << "Unable to open room file " << title_map_path << "\n";
        exit(1);
    }

    std::shared_ptr<TitleMap> title_map = std::make_shared<TitleMap>();
    std::string line;
    int i = 0;
    while (getline(input_stream, line)) {
        if (line.length() != h_WINDOW_T_WIDTH) {
            std::cout << title_map_path << " incorrect h_WINDOW_WIDTH "
                      << line.length() << " != " << h_WINDOW_T_WIDTH << "\n";
            exit(1);
        }
        for (int j = 0; j < line.length(); ++j) {
            // title_map's items belong to [0,...N]
            (*title_map)[h_WINDOW_T_HEIGHT - i - 1][j] = line[j] - 'A';  // todo возможно, поменяю маппинг
        };
        ++i;
    }
    if (i != h_WINDOW_T_HEIGHT) {
        std::cout << title_map_path << " incorrect h_WINDOW_T_HEIGHT "
                  << i << " != " << h_WINDOW_T_HEIGHT << "\n";
        exit(1);
    }
    input_stream.close();
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
    if (line.length() != h_N_TRANSITIONS) {
        std::cout << transitions_path << " incorrect h_N_TRANSITIONS "
                  << line.length() << " != " << h_N_TRANSITIONS << "\n";
        exit(2);
    }
    for (int j = 0; j < line.length(); ++j) {
        (*transitions_data)[j] = int(line[j]);
    };
    return transitions_data;
};


GlobalState::GlobalState(const std::string &rooms_data_path) {
    for (const std::filesystem::directory_entry &single_room_dir : fs::directory_iterator(rooms_data_path)) {
        std::string single_room_path = single_room_dir.path();
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
    if (room_number >= this->n_rooms) {
        std::cout << "incorrect room_number " << room_number << " >= " << this->n_rooms << "\n";
        exit(3);
    }
    this->room_background_map = this->background_map_vector[room_number];
    this->room_objects_map = this->objects_map_vector[room_number];
    this->room_transitions_data = this->transitions_data_vector[room_number];
};

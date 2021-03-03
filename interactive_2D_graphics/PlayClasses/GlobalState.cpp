
#include "GlobalState.h"

#include <memory>

std::shared_ptr<BackgroundMap> readBackgroundMap1(const std::string& background_map_path){

    std::ifstream input_stream(background_map_path);
    if (!input_stream.is_open()){
        std::cout << "Unable to open file map\n" << background_map_path << "\n";
        exit(2);
    }

    std::shared_ptr<BackgroundMap> background_map = std::make_shared<BackgroundMap>();

    std::string line;
    int i = 0;
    while (getline(input_stream,line)){
        if (line.length() != h_WINDOW_T_WIDTH){
            std::cout << "incorrect h_WINDOW_WIDTH "<<line.length() <<" != " << h_WINDOW_T_WIDTH <<"\n";
            exit(1);
        }
        for (int j=0; j<line.length(); ++j){
            // background_map's items belong to [0,...N]
            (*background_map)[h_WINDOW_T_HEIGHT-i-1][j] = line[j] - 'A';  // todo возможно, поменяю маппинг
        };
        ++i;
    }
    input_stream.close();
    return background_map;
};


GlobalState::GlobalState(const std::string& rooms_data_path) {
    for (const std::filesystem::directory_entry& single_room_dir : fs::directory_iterator(rooms_data_path)){
        std::string single_room_path = single_room_dir.path();
        std::string background_map_path = single_room_path + "/background_map.txt";

        this->background_map_vector.push_back(readBackgroundMap1(background_map_path));


        std::cout<<background_map<<" background_map.txt\n";
    }

};

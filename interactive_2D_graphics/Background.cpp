#include "Background.h"


std::vector<std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT>>
    read_background_map(const std::string& background_path){

    std::ifstream input_stream(background_path);
    std::vector<std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT>> result;
    if (!input_stream.is_open()){
        std::cout << "Unable to open file";  // todo log error
        return result;
    }

    std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT> background_map = {};  // todo type

    std::string line;
    int i = 0;
    while ( getline (input_stream,line) ){
        if (line.length() != h_WINDOW_T_WIDTH){
            std::cout << "incorrect h_WINDOW_WIDTH";  // todo log error
            return result;
        }
        std::copy(line.begin(), line.end(), background_map[i].data());
        ++i;
    }
    input_stream.close();
    result.push_back(background_map);
    return result;
};

Background::Background(std::string background_path) : background_path(std::move(background_path)) {
    this->background_vector = read_background_map(background_path);
};

void Background::Draw(Image &screen) {
    Pixel my_pixel{100, 100, 100};
    for (int y=0; y<h_WINDOW_T_HEIGHT*10; ++y){
        for (int x=0; x<h_WINDOW_T_HEIGHT*10; ++x){
            screen.PutPixel(x, y, my_pixel);
        }
    }

//    if (Moved()) {
//        for (int y = old_coords.y; y <= old_coords.y + tileSize; ++y) {
//            for (int x = old_coords.x; x <= old_coords.x + tileSize; ++x) {
//                screen.PutPixel(x, y, backgroundColor);
//            }
//        }
//        old_coords = coords;
//    }
//    Image andrew("./resources/16.png");
//    for (int y = coords.y; y <= coords.y + tileSize; ++y) {
//        for (int x = coords.x; x <= coords.x + tileSize; ++x) {
//            Pixel masha = andrew.GetPixel(x-coords.x, y-coords.y);
//            screen.PutPixel(x, y, masha);
//        }
//    }
}
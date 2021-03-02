#include <cstring>
#include "Background.h"


std::vector<std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT>>
    readBackgroundMap(const std::string& background_path){

    std::ifstream input_stream(background_path);
    std::vector<std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT>> result;
    if (!input_stream.is_open()){
        std::cout << "Unable to open file map\n" << background_path << "\n";  // todo log error
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

std::vector<std::shared_ptr<Image>> readBackgroundTitles(const std::string&  titles_path){
    std::string path;
    std::vector<std::shared_ptr<Image>> titles_vector;
    std::ifstream title_file(titles_path);  // todo stream does not open error
    while (std::getline(title_file, path)) {
        std::cout<<"add tile path \n";
        titles_vector.push_back(std::make_shared<Image>(path));
    }
    title_file.close();
    return  titles_vector;
};


Background::Background(std::string map_path, std::string  titles_path) :
                        map_path(std::move(map_path)),
                        titles_path(std::move(titles_path)) {
    std::cout << "background_path\n" << this->map_path << "\n";
    this->background_vector = readBackgroundMap(this->map_path);
    this->titles_vector = readBackgroundTitles(this->titles_path);
    this->background_state = new Pixel[h_WINDOW_T_HEIGHT * h_WINDOW_T_HEIGHT];
};

void drawPixel(Image &screen, const std::shared_ptr<Image>& title, int global_x, int global_y){
    for (int y=0; y<h_TEXTURE_SIZE; ++y){
        for (int x=0; x<h_TEXTURE_SIZE; ++x){
            screen.PutPixel(global_x+x, global_y+y, title->GetPixel(x,y));
        }
    }
};

void Background::Draw(Image &screen) {
    for (int y=0; y<h_WINDOW_T_HEIGHT; ++y){
        for (int x=0; x<h_WINDOW_T_HEIGHT; ++x){
            drawPixel(screen, this->titles_vector[0], x*h_TEXTURE_SIZE, y*h_TEXTURE_SIZE);
        }
    }
    std::memcpy(background_state, screen.Data(), h_WINDOW_T_HEIGHT * h_WINDOW_T_HEIGHT);
}
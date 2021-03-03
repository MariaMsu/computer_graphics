#include "Background.h"


std::vector<std::shared_ptr<Image>> readBackgroundTitles(const std::string&  titles_path){
    std::string path;
    std::vector<std::shared_ptr<Image>> titles_vector;
    std::ifstream title_file(titles_path);  // todo stream does not open error
    while (std::getline(title_file, path)) {
        titles_vector.push_back(std::make_shared<Image>(path));
    }
    title_file.close();
    return  titles_vector;
};


Background::Background(std::string map_path, std::string  titles_path) :
                        map_path(std::move(map_path)),
                        titles_path(std::move(titles_path)) {
    this->titles_vector = readBackgroundTitles(this->titles_path);
};

void drawTitle(Image &screen, const std::shared_ptr<Image>& title, int global_x, int global_y){
    for (int y=0; y<h_TEXTURE_SIZE; ++y){
        for (int x=0; x<h_TEXTURE_SIZE; ++x){
            screen.PutPixel(global_x + x, global_y + (h_TEXTURE_SIZE - y - 1), title->GetPixel(x,y));
        }
    }
};

void Background::DrawRoom(Image &screen, GlobalState &screen_state) {
    TitleMap background_map = *screen_state.room_background_map;
    for (int y=0; y<h_WINDOW_T_HEIGHT; ++y){
        for (int x=0; x<h_WINDOW_T_WIDTH; ++x){
            const std::shared_ptr<Image>& title = this->titles_vector[background_map[y][x]];
            drawTitle(screen, title, x * h_TEXTURE_SIZE, y * h_TEXTURE_SIZE);
        }
    }
    // todo write method to copy
    std::memcpy(screen_state.background_state, screen.Data(), h_WINDOW_HEIGHT * h_WINDOW_WIDTH * sizeof(Pixel));
}

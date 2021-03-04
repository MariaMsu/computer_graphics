#include "Background.h"


std::map<int, std::shared_ptr<Image>> readBackgroundTitles(const std::string &titles_path) {
    std::string path;
    std::map<int, std::shared_ptr<Image>> titles_map;
    std::ifstream title_file(titles_path);
    if (!title_file.is_open()) {
        std::cout << "Unable to open title's path file " << titles_path << "\n";
        exit(4);
    }
    int i = 0;  // todo
    while (std::getline(title_file, path)) {
        if (path.length() != 0){
            std::pair<int, std::shared_ptr<Image>> new_item =
                    std::pair<int, std::shared_ptr<Image>>(i, std::make_shared<Image>(path));
            titles_map.insert(new_item);
        }
        ++i;
    }
    std::cout<<"\n";
    title_file.close();
    return titles_map;
};


Background::Background(std::string titles_path) : titles_path(std::move(titles_path)) {
    this->titles_map = readBackgroundTitles(this->titles_path);
};

void drawTitle(Image &screen, const std::shared_ptr<Image> &title, int global_x, int global_y) {
    for (int y = 0; y < h_TEXTURE_SIZE; ++y) {
        for (int x = 0; x < h_TEXTURE_SIZE; ++x) {
            screen.PutPixel(global_x + x, global_y + (h_TEXTURE_SIZE - y - 1), title->GetPixel(x, y));
        }
    }
};

void Background::DrawRoom(Image &screen, GlobalState &screen_state) {
    TitleMap background_map = *screen_state.room_background_map;
    for (int y = 0; y < h_WINDOW_T_HEIGHT; ++y) {
        for (int x = 0; x < h_WINDOW_T_WIDTH; ++x) {
            int key = background_map[y][x];
            if (titles_map.count(key) == 0){
                std::cerr<<"Title "<<key<<" did not loaded\n";
                exit(5);
            }
            const std::shared_ptr<Image> &title = this->titles_map[background_map[y][x]];
            drawTitle(screen, title, x * h_TEXTURE_SIZE, y * h_TEXTURE_SIZE);
        }
    }
    // todo write method to copy
    std::memcpy(screen_state.background_state, screen.Data(), h_WINDOW_HEIGHT * h_WINDOW_WIDTH * sizeof(Pixel));
}

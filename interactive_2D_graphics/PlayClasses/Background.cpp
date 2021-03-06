#include "Background.h"
#include "Utils.h"


std::map<int, std::shared_ptr<Image>> readBackgroundTitles(const std::string &titles_path) {
    std::string path;
    std::map<int, std::shared_ptr<Image>> titles_map;
    std::ifstream title_file(titles_path);
    if (!title_file.is_open()) {
        std::cerr << "Unable to open title's path file " << titles_path << "\n";
        exit(4);
    }
    for (int i = 0; std::getline(title_file, path); ++i) {
        if (path.length() != 0) {
            std::pair<int, std::shared_ptr<Image>> new_item =
                    std::pair<int, std::shared_ptr<Image>>(i, std::make_shared<Image>(path));
            titles_map.insert(new_item);
        }
    }
    title_file.close();
    return titles_map;
};


Background::Background(std::string titles_path) : titles_path(std::move(titles_path)) {
    this->titles_map = readBackgroundTitles(this->titles_path);
};

void Background::DrawRoom(Image &screen, std::shared_ptr<TitleMap> &background_map) {
    DrawArea(screen, background_map,
             ObjectBorders{0, h_WINDOW_T_WIDTH-1, 0, h_WINDOW_T_HEIGHT-1});
}

void Background::DrawArea(Image &screen, std::shared_ptr<TitleMap> &background_map, ObjectBorders borders) {
    for (int y = borders.y_low; y <= borders.y_heigh; ++y) {
        for (int x = borders.x_left; x <= borders.x_right; ++x) {
            int key = (*background_map)[y][x];
            if (titles_map.count(key) == 0) {
                std::cerr << "Title " << key << " did not loaded\n";
                exit(5);
            }
//            std::cout<<"DrawArea x="<<x<<", y="<<y<<"\n";
            const std::shared_ptr<Image> &title = this->titles_map[(*background_map)[y][x]];
            drawSaveAsset(screen, title, x * title->Width(), y * title->Height());
        }
    }
}

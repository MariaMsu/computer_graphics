#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Utils.h"
#include "GlobalState.h"

enum class MovementDir {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct PlayerBorders {
    explicit PlayerBorders(int x_left, int x_right, int y_low, int y_heigh) :
            x_left(x_left), x_right(x_right), y_low(y_low), y_heigh(y_heigh) {};
    int x_left;
    int x_right;
    int y_low;
    int y_heigh;
};

struct Player {
    explicit Player(const std::string &skins_path);

    void SetPosition(Point player_position);
    void ProcessInput(MovementDir dir, GlobalState &global_state);
    void ProcessBridge(GlobalState &global_state);
    void Draw(Image &screen, GlobalState &screen_state);
    PlayerBorders GetTitleBorders(Point coord, int x_add_space, int y_add_space);

private:
    void updateSkin();
    int skin_ind_right = 0;
    int skin_ind_left = 0;
    std::shared_ptr<Image> current_skin;

    Point movement = {};
    bool is_moved = false;

    int width;
    int height;
    Point coords{h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
    Point old_coords{h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
    Pixel color{.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 4;  // todo const
    std::shared_ptr<Image> static_skin;
    std::vector<std::shared_ptr<Image>> dynamic_skins_right;
    std::vector<std::shared_ptr<Image>> dynamic_skins_left;
};

#endif //MAIN_PLAYER_H

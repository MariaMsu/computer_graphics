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
            x_left(x_left), x_right(x_right), y_low(y_low), y_heigh(y_heigh) {
        x_center = (x_left + x_right + 1) / 2;
        y_center = (y_low + y_heigh + 1) / 2;
    };
    int x_left;
    int x_right;
    int x_center;
    int y_low;
    int y_heigh;
    int y_center;
};

struct Player {
    explicit Player(const std::string &asset_path){
        player_image = std::make_shared<Image>(asset_path);
    };
    void SetPosition(Point player_position);
    bool Moved() const;
    void ProcessInput(MovementDir dir, GlobalState &global_state);
    void ProcessBridge(GlobalState &global_state);
    void Draw(Image &screen, GlobalState &screen_state);
    PlayerBorders GetTitleBorders(Point coord, int x_add_space, int y_add_space);

private:
    Point coords{h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
    Point old_coords{h_WINDOW_HEIGHT / 2, h_WINDOW_WIDTH / 2};
    Pixel color{.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 4;
    std::shared_ptr<Image> player_image;
};

#endif //MAIN_PLAYER_H

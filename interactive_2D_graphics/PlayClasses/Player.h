#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "GlobalState.h"

struct Point {
    int x;
    int y;
};

enum class MovementDir {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct ObjectBorders {
    explicit ObjectBorders(int x_left, int x_right, int y_low, int y_heigh):
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
    explicit Player(const std::string& asset_path, Point pos = {.x = 10, .y = 10}):
            coords(pos), old_coords(coords), player_image(Image(asset_path)) {};

    bool Moved() const;
    void ProcessInput(MovementDir dir, GlobalState& global_state);
    void ProcessBridge(GlobalState& global_state);
    void Draw(Image &screen, GlobalState& screen_state);
    ObjectBorders GetTitleBorders(Point coord, int add_space);

private:
    Point coords{.x = 10, .y = 10};
    Point old_coords{.x = 10, .y = 10};
    Pixel color{.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 4;
    Image player_image;
};

#endif //MAIN_PLAYER_H

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

struct Player {
    explicit Player(const std::string& asset_path, Point pos = {.x = 10, .y = 10}):
            coords(pos), old_coords(coords), player_image(Image(asset_path)) {};

    bool Moved() const;
    void ProcessInput(MovementDir dir, GlobalState& global_state);
    void Draw(Image &screen, GlobalState& screen_state);

private:
    Point coords{.x = 10, .y = 10};
    Point old_coords{.x = 10, .y = 10};
    Pixel color{.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 4;
    Image player_image;
};

#endif //MAIN_PLAYER_H

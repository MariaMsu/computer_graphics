#include <iostream>
#include "Player.h"


bool Player::Moved() const {
    if (coords.x == old_coords.x && coords.y == old_coords.y)
        return false;
    else
        return true;
}

bool isWall(Point coord, ScreenState& screen_state){
    int x_title = coord.x / h_TEXTURE_SIZE;
    int y_title = coord.y / h_TEXTURE_SIZE;
    char map_element = (*screen_state.background_map)[y_title][x_title]+'A';
    if (h_walls.find(map_element) != h_walls.end()){
        return true;
    }
    return false;
}

void Player::ProcessInput(MovementDir dir, ScreenState& screen_state) {
    int move_dist = move_speed * 1;
    Point tmp_old_coords{this->old_coords};
    Point tmp_coords{this->coords};
    switch (dir) {
        case MovementDir::UP:
            tmp_old_coords.y = coords.y;
            tmp_coords.y += move_dist;
            break;
        case MovementDir::DOWN:
            tmp_old_coords.y = coords.y;
            tmp_coords.y -= move_dist;
            break;
        case MovementDir::LEFT:
            tmp_old_coords.x = coords.x;
            tmp_coords.x -= move_dist;
            break;
        case MovementDir::RIGHT:
            tmp_old_coords.x = coords.x;
            tmp_coords.x += move_dist;
            break;
        default:
            break;
    }
    if (!isWall(tmp_coords, screen_state)){
        // update coordinates only if player not in the wall
        this->old_coords = tmp_old_coords;
        this->coords = tmp_coords;
    }
}

void Player::Draw(Image &screen, ScreenState& screen_state) {
    if (Moved()) {
        for (int y = old_coords.y; y <= old_coords.y + tileSize; ++y) {
            for (int x = old_coords.x; x <= old_coords.x + tileSize; ++x) {
                screen.PutPixel(x, y, screen_state.background_state[y*h_WINDOW_WIDTH+x]);  // todo create function
            }
        }
        old_coords = coords;
    }
    for (int y = coords.y; y <= coords.y + tileSize; ++y) {
        for (int x = coords.x; x <= coords.x + tileSize; ++x) {

            Pixel oldPix = screen_state.background_state[y*h_WINDOW_WIDTH+x];
            Pixel newPix = this->player_image.GetPixel(x - coords.x, tileSize - y + coords.y);
            screen.PutPixel(x, y, blend(oldPix, newPix));
//            Pixel masha = andrew.GetPixel(x-coords.x, y-coords.y);
//            screen.PutPixel(x, y, masha);
        }
    }
}
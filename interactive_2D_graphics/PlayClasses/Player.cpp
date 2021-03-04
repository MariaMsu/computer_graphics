#include "Player.h"


bool Player::Moved() const {
    if (coords.x == old_coords.x && coords.y == old_coords.y)
        return false;
    else
        return true;
}

bool isWall(ObjectBorders borders, GlobalState &global_state) {
    for (int x : {borders.x_left, borders.x_right}) {
        for (int y : {borders.y_low, borders.y_heigh}) {
            short map_element = (*global_state.room_background_map)[y][x];
            if (h_walls.find(map_element) != h_walls.end()) { return true; }
        }
    }
    return false;
    // todo check field borders
}

ObjectBorders Player::GetTitleBorders(Point coord) {
    return ObjectBorders{
            (coord.x + h_P_PHIS_WIDTH_SHIFT) / h_TEXTURE_SIZE,
            (coord.x + player_image.Width() - h_P_PHIS_WIDTH_SHIFT) / h_TEXTURE_SIZE,
            (coord.y + h_P_PHIS_HEIGHT_SHIFT) / h_TEXTURE_SIZE,
            (coord.y + player_image.Height() - h_P_PHIS_HEIGHT_SHIFT) / h_TEXTURE_SIZE,
    };
};

void Player::ProcessInput(MovementDir dir, GlobalState &global_state) {
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
    ObjectBorders tmp_borders = GetTitleBorders(tmp_coords);
    if (!isWall(tmp_borders, global_state)) {
        // update coordinates only if player not in the wall
        this->old_coords = tmp_old_coords;
        this->coords = tmp_coords;
    }
}

void Player::Draw(Image &screen, GlobalState &screen_state) {
    if (Moved()) {
        for (int y = old_coords.y; y < old_coords.y + player_image.Height(); ++y) {
            for (int x = old_coords.x; x < old_coords.x + player_image.Width(); ++x) {
                screen.PutPixel(x, y, screen_state.background_state[y * h_WINDOW_WIDTH + x]);  // todo create function
            }
        }
        old_coords = coords;
    }
    for (int y = coords.y; y < coords.y + player_image.Height(); ++y) {
        for (int x = coords.x; x < coords.x + player_image.Width(); ++x) {

            Pixel oldPix = screen_state.background_state[y * h_WINDOW_WIDTH + x];
            Pixel newPix = this->player_image.GetPixel(x - coords.x, player_image.Height() - 1 - y + coords.y);
            screen.PutPixel(x, y, blend(oldPix, newPix));
        }
    }
}

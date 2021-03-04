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
    if ((borders.y_low >= h_WINDOW_T_HEIGHT - 1) ||
        (borders.x_right >= h_WINDOW_T_WIDTH - 1) ||
        (borders.y_low <= 0) ||
        (borders.x_left <= 0)) {
        return true;
    }
    return false;
}

const int screen_aspect = h_WINDOW_HEIGHT / h_WINDOW_WIDTH;

int isTransition(ObjectBorders borders, GlobalState &global_state) {
    // return room_direction [1..4] or 0
    short map_element = (*global_state.room_background_map)[borders.y_center][borders.x_center];
    if (h_lava.find(map_element) == h_lava.end()) { return 0; }
    if (borders.y_center > h_WINDOW_T_HEIGHT - screen_aspect * borders.x_center) {
        // над побочной диагональю
        if (borders.y_center > screen_aspect * borders.x_center) { return 1; }
        else { return 2; };
    } else {
        // под побочной диагональю
        if (borders.y_center > screen_aspect * borders.x_center) { return 4; }
        else { return 3; };
    }
};

ObjectBorders Player::GetTitleBorders(Point coord) {
    return ObjectBorders{
            (coord.x + h_PLAYER_PHIS_WIDTH_SHIFT) / h_TEXTURE_SIZE,
            (coord.x + player_image.Width() - h_PLAYER_PHIS_WIDTH_SHIFT) / h_TEXTURE_SIZE,
            (coord.y + h_PLAYER_PHIS_HEIGHT_SHIFT) / h_TEXTURE_SIZE,
            (coord.y + player_image.Height() - h_PLAYER_PHIS_HEIGHT_SHIFT) / h_TEXTURE_SIZE,
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
    if (int room_direction = isTransition(tmp_borders, global_state)){
        global_state.update_room = room_direction;
        return;
    }
    if (!isWall(tmp_borders, global_state)) {
        // update coordinates only if player not in the wall
        this->old_coords = tmp_old_coords;
        this->coords = tmp_coords;
        return;
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

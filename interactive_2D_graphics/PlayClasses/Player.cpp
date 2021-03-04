#include "Player.h"


bool Player::Moved() const {
    if (coords.x == old_coords.x && coords.y == old_coords.y)
        return false;
    else
        return true;
}

bool titleTypeIntersection(const ObjectBorders borders, const std::set<short> &title_types,
                           const std::shared_ptr<TitleMap> &room_background_map) {
    for (int x : {borders.x_left, borders.x_right}) {
        for (int y : {borders.y_low, borders.y_heigh}) {
            short map_element = (*room_background_map)[y][x];
            if (title_types.count(map_element) != 0) { return true; }
        }
    }
    return false;
};

bool isBeyondWindow(const ObjectBorders borders) {
    if ((borders.y_low >= h_WINDOW_T_HEIGHT - 1) ||
        (borders.x_right >= h_WINDOW_T_WIDTH - 1) ||
        (borders.y_low <= 0) ||
        (borders.x_left <= 0)) {
        return true;
    }
    return false;
}

const int screen_aspect = h_WINDOW_HEIGHT / h_WINDOW_WIDTH;

int getTransitionDirection(ObjectBorders borders, GlobalState &global_state) {
    // return room_direction [1..4]
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

ObjectBorders Player::GetTitleBorders(Point coord, int add_space=0) {
    return ObjectBorders{
            (coord.x - add_space + h_PLAYER_PHIS_WIDTH_SHIFT) / h_TEXTURE_SIZE,
            (coord.x + add_space + player_image.Width() - h_PLAYER_PHIS_WIDTH_SHIFT) / h_TEXTURE_SIZE,
            (coord.y - add_space + h_PLAYER_PHIS_HEIGHT_SHIFT) / h_TEXTURE_SIZE,
            (coord.y + add_space+ player_image.Height() - h_PLAYER_PHIS_HEIGHT_SHIFT) / h_TEXTURE_SIZE,
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
    if (titleTypeIntersection(tmp_borders, h_lava, global_state.room_background_map)){
        int room_direction = getTransitionDirection(tmp_borders, global_state);
        global_state.SetTransitionDirection(room_direction);
        return;
    }
    if (!(isBeyondWindow(tmp_borders) ||
        titleTypeIntersection(tmp_borders, h_walls, global_state.room_background_map))) {
        // update coordinates only if player not in the wall
        this->old_coords = tmp_old_coords;
        this->coords = tmp_coords;
    }
}

void Player::ProcessBridge(GlobalState &global_state) {
    ObjectBorders borders = GetTitleBorders(this->coords, h_TEXTURE_SIZE/2);
    if (!titleTypeIntersection(borders, h_lava, global_state.room_background_map)) { return; }
    int room_direction = getTransitionDirection(borders, global_state);
    global_state.PutBridge(room_direction);
    std::clog << "Put bridge " << room_direction << "\n";
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

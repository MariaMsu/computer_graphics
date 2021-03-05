#include "Player.h"


bool Player::Moved() const {
    if (coords.x == old_coords.x && coords.y == old_coords.y)
        return false;
    else
        return true;
}

bool titleTypeIntersection(const PlayerBorders borders, const std::set<short> &title_types,
                           const std::shared_ptr<TitleMap> &room_background_map, PointT& intersection) {
    // checking only corners is not enough
    for (int x = borders.x_left; x <= borders.x_right; ++x) {
        for (int y = borders.y_low; y <= borders.y_heigh; ++y) {
            short map_element = (*room_background_map)[y][x];
            if (title_types.count(map_element) != 0) {
                intersection = PointT{x, y};
                return true;
            }
        }
    }
    return false;
};

bool isBeyondWindow(const PlayerBorders borders) {
    if ((borders.y_heigh >= h_WINDOW_T_HEIGHT - 1) ||
        (borders.x_right >= h_WINDOW_T_WIDTH - 1) ||
        (borders.y_low <= 0) ||
        (borders.x_left <= 0)) {
        return true;
    }
    return false;
}

PlayerBorders Player::GetTitleBorders(Point coord, int x_add_space = 0, int y_add_space = 0) {
    return PlayerBorders{
            (coord.x - x_add_space) / h_TEXTURE_SIZE,
            (coord.x + player_image.Width() + x_add_space) / h_TEXTURE_SIZE,
            (coord.y - y_add_space) / h_TEXTURE_SIZE,
            (coord.y + player_image.Height() + y_add_space) / h_TEXTURE_SIZE,
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

    PlayerBorders tmp_borders = GetTitleBorders(
            tmp_coords, -h_PLAYER_PHIS_WIDTH_SHIFT, -h_PLAYER_PHIS_HEIGHT_SHIFT);
//    std::cout<<"x: "<<tmp_coords.x<<" res= "<<tmp_borders.x_left<<", "<<tmp_borders.x_right<<"\n";
//    std::cout<<"y: "<<tmp_coords.y<<" res= "<<tmp_borders.y_low<<", "<<tmp_borders.y_heigh<<"\n";
//    std::cout<<"\n";
    PointT intersection;
    if (titleTypeIntersection(tmp_borders, h_lava, global_state.room_background_map, intersection)) {
        global_state.PushStateRoom(coords);
        return;
    }
    if (!isBeyondWindow(tmp_borders) &&
        !titleTypeIntersection(tmp_borders, h_walls, global_state.room_background_map, intersection)) {
        // update coordinates only if player not in the wall
        this->old_coords = tmp_old_coords;
        this->coords = tmp_coords;
    }
}

void Player::ProcessBridge(GlobalState &global_state) {
    int nearest_transition;
    double distance = detNearestPointT(
            coords, global_state.room_transitions_points, nearest_transition);
    if (distance < h_BRIDGE_REQ_DISTANCE){ global_state.PushStateBridge(nearest_transition);
    std::clog<<"put bridge\n";
    }
}

void Player::Draw(Image &screen, GlobalState &screen_state) {
    if (Moved()) {
        for (int y = old_coords.y; y < old_coords.y + player_image.Height(); ++y) {
            for (int x = old_coords.x; x < old_coords.x + player_image.Width(); ++x) {
                screen.PutPixel(x, y, screen.GetPixel(x, y));
            }
        }
        old_coords = coords;
    }
//    drawAsset(screen, image_ptr, coords.x, coords.y);
    for (int y = coords.y; y < coords.y + player_image.Height(); ++y) {
        for (int x = coords.x; x < coords.x + player_image.Width(); ++x) {

            Pixel newPix = this->player_image.GetPixel(x - coords.x, player_image.Height() - 1 - y + coords.y);
            screen.PutPixel(x, y, blend(screen.GetPixel(x, y), newPix));
        }
    }
}

void Player::SetPosition(Point player_position) {
    coords.x = player_position.x - player_image.Width() / 2;
    coords.y = player_position.y - player_image.Height() / 2;
}

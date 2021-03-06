#include "Player.h"

bool isBeyondWindow(const ObjectBorders borders) {
    if ((borders.y_heigh >= h_WINDOW_T_HEIGHT - 1) ||
        (borders.x_right >= h_WINDOW_T_WIDTH - 1) ||
        (borders.y_low <= 0) ||
        (borders.x_left <= 0)) {
        return true;
    }
    return false;
}

Pixel *makeMirrorPixelData(Pixel *data, int weight, int height) {
    Pixel *new_data = new Pixel[weight * height]{};
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < weight; ++x) {
            new_data[y * weight + x] = data[y * weight + weight - x - 1];
        }
    }
    return new_data;
}

Player::Player(const std::string &skins_path) {
    std::string path;
    std::ifstream skin_file(skins_path);
    if (!skin_file.is_open()) {
        std::cerr << "Unable to open title's path file " << skins_path << "\n";
        exit(4);
    }
    if (std::getline(skin_file, path)) {
        this->static_skin = std::make_shared<Image>(path);
    } else {
        std::cerr << "Skins file is empty\n";
        exit(4);
    }
    int i_width, i_height;
    while (std::getline(skin_file, path)) {
        std::shared_ptr<Image> image = std::make_shared<Image>(path);
        this->dynamic_skins_right.push_back(image);
        i_width = image->Width(), i_height = image->Height();
        this->dynamic_skins_left.push_back(std::make_shared<Image>
                                                   (makeMirrorPixelData(image->Data(), i_width, i_height),
                                                    i_width,
                                                    i_height,
                                                    image->Channels())
        );
    }
    skin_file.close();
    assert(!dynamic_skins_right.empty());
    width = i_width;
    height = i_height;
    current_skin = static_skin;
};

ObjectBorders Player::GetTitleBorders(Point coord, int x_add_space = 0, int y_add_space = 0) {
    return ObjectBorders{
            (coord.x - x_add_space) / h_TEXTURE_SIZE,
            (coord.x + width + x_add_space) / h_TEXTURE_SIZE,
            (coord.y - y_add_space) / h_TEXTURE_SIZE,
            (coord.y + height + y_add_space) / h_TEXTURE_SIZE,
    };
};

void Player::ProcessInput(MovementDir dir, GlobalState &global_state) {
    Point tmp_old_coords{this->old_coords};
    Point tmp_coords{this->coords};
    this->is_moved = true;
    switch (dir) {
        case MovementDir::UP:
            tmp_old_coords.y = coords.y;
            tmp_coords.y += h_PLAYER_MOVE_SPEED;
            break;
        case MovementDir::DOWN:
            tmp_old_coords.y = coords.y;
            tmp_coords.y -= h_PLAYER_MOVE_SPEED;
            break;
        case MovementDir::LEFT:
            tmp_old_coords.x = coords.x;
            tmp_coords.x -= h_PLAYER_MOVE_SPEED;
            break;
        case MovementDir::RIGHT:
            tmp_old_coords.x = coords.x;
            tmp_coords.x += h_PLAYER_MOVE_SPEED;
            break;
        default:
            break;
    }
    movement.x += tmp_coords.x - tmp_old_coords.x;
    movement.y += tmp_coords.y - tmp_old_coords.y;

    ObjectBorders tmp_borders = GetTitleBorders(
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
            coords, global_state.GetTransitionsPoints(), nearest_transition);
    if (distance < h_BRIDGE_REQ_DISTANCE) {
        global_state.PushStateBridge(nearest_transition);
    }
}

void Player::ProcessLogs(GlobalState &global_state) {
    int nearest_transition;
    double distance = detNearestPointT(
            coords, global_state.log_points, nearest_transition);
    std::cout<<"\n";
    if (distance < h_LOGS_REQ_DISTANCE) {
        global_state.PushStateLogs(nearest_transition);
        std::cout<<"ProcessLogs "<<nearest_transition<<"\n";
    }
}

void Player::Draw(Image &screen, GlobalState &global_state) {
    updateSkin();
    if (is_moved) {
        for (int y = old_coords.y; y < old_coords.y + height; ++y) {
            for (int x = old_coords.x; x < old_coords.x + width; ++x) {
                screen.PutPixel(x, y, screen.GetPixel(x, y));
            }
        }
        old_coords = coords;
    }
    drawTrAsset(screen, current_skin, coords.x, coords.y);
    is_moved = false;
}

void Player::SetPosition(Point player_position) {
    coords.x = player_position.x - width / 2;
    coords.y = player_position.y - height / 2;
}

void Player::updateSkin() {
    if (!is_moved) {
        if (current_skin != static_skin){
            current_skin = static_skin;
            movement.x = 0,  movement.y = 0;
            skin_ind_right = 0, skin_ind_left = 0;
        }
        return;
    }
    if ((movement.x >= h_PLAYER_SKIN_SPEED) || (!movement.x && movement.y >= h_PLAYER_SKIN_SPEED)){
        current_skin = dynamic_skins_right[(skin_ind_right++) % dynamic_skins_right.size()];
        movement.x = 0, movement.y = 0;
        return;
    }
    if ((-movement.x >= h_PLAYER_SKIN_SPEED) || (!movement.x && (-movement.y) >= h_PLAYER_SKIN_SPEED)){
        current_skin = dynamic_skins_left[(skin_ind_left++) % dynamic_skins_left.size()];
        movement.x = 0, movement.y = 0;
        return;
    }
}



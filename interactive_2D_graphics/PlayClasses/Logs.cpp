#include <string>
#include "Logs.h"

Logs::Logs(const std::string &logs_asset, const std::string &attention_asset) {
    this->logs_image = std::make_shared<Image>(logs_asset);
    this->attention_image = std::make_shared<Image>(attention_asset);
    this->_height = logs_image->Height();
    this->_weight = logs_image->Width();
}

bool Logs::initLogPoint(PointT &point) {
    for (int i = 0; i < 3; ++i) { // трижды пробуем найти место
        point = getRandomPoint();
        ObjectBorders borders = ObjectBorders{
                point.x - 1, point.x + 1, point.y - 1, point.y + 1};
        PointT t;
        if (!titleTypeIntersection(borders, h_not_flore, _background_map, t)) { return true; }
    }
    return false; // не удалось найти подходящее место
}

void Logs::DrawRoom(Image &screen, GlobalState &global_state) {
    _room_ind = global_state.GetRoomInd();
    _logs_number = global_state.GetLogsNumber();
    _background_map = global_state.room_background_map;
    if (all_rooms_logs_positions.count(_room_ind) == 0) { drawNewRoom(screen); }
    else { drawOldRoom(screen, _room_ind); }

    all_rooms_logs_positions.insert(
            std::pair<int, std::vector<PointT>>(_room_ind, log_points));
    // todo нужно сделать по ссылке, но мне лень дебажить
    global_state.log_points = std::vector<PointT>(this->log_points);
}

void Logs::RemoveLog(int removing_ind, GlobalState &globalState, ObjectBorders &borders) {
    assert(removing_ind < log_points.size());
    PointT p = log_points[removing_ind];
    borders = ObjectBorders{p.x - 1, p.x + 1, p.y, p.y};
    log_points.erase(log_points.begin() + removing_ind);
    globalState.log_points = log_points;
    std::cout << "remove x=" << p.x << ", y=" << p.y << "\n";
}

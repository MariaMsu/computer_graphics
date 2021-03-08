#include <string>
#include "Logs.h"

Logs::Logs(const std::string &logs_asset, const std::string &attention_asset) {
    this->logs_image = std::make_shared<Image>(logs_asset);
    this->attention_image = std::make_shared<Image>(attention_asset);
}

bool Logs::initLogPoint(PointT &point) {
    for (int i = 0; i < 3; ++i) { // трижды пробуем найти место
        point = getRandomPoint();
        ObjectBorders borders = ObjectBorders{
                point.x - 1, point.x + 1, point.y, point.y};
        PointT t;
        if (!titleTypeIntersection(borders, h_not_flore, _background_map, t)) { return true; }
    }
    return false; // не удалось найти подходящее место
}

void Logs::DrawRoom(Image &screen, GlobalState &global_state) {
    this->log_points = std::make_shared<std::vector<PointT>>(); // очищаем старый log_points
    this->transition_counter++;
    _room_ind = global_state.GetRoomInd();
    _logs_number = global_state.GetLogsNumber();
    _background_map = global_state.room_background_map;

    if ((all_rooms_logs_positions.count(_room_ind) == 0) ||
        (transition_counter - all_rooms_logs_positions[_room_ind].dump_mark >= h_LOG_UPDATE_INTERVAL)){
        drawNewRoom(screen);
        all_rooms_logs_positions[_room_ind] = RoomDump{log_points, transition_counter};
    } else {
        drawOldRoom(screen, _room_ind);
        all_rooms_logs_positions[_room_ind].dump_mark = transition_counter;
    }
    attention_points.clear();
    for(PointT log_point: *log_points){
        attention_points.push_back(Point{log_point.x * h_TEXTURE_SIZE - h_TEXTURE_SIZE / 4,
                                         log_point.y * h_TEXTURE_SIZE + h_TEXTURE_SIZE / 4});
    }
    last_update_time = 0;
    last_direction_change_time = 0;
    global_state.log_points = *log_points;   // todo нужно сделать по ссылке, но мне лень дебажить
}

void Logs::drawNewRoom(Image &screen) {
    assert(log_points->size() == 0);
    for (int i = 0; i < _logs_number; ++i) {
        PointT point;
        if (!initLogPoint(point)) { continue;}
        this->log_points->push_back(point);
        drawTrSaveAsset(screen, logs_image,
                        point.x * h_TEXTURE_SIZE - h_TEXTURE_SIZE / 2,
                        point.y * h_TEXTURE_SIZE);
    }
}

void Logs::drawOldRoom(Image &screen, int room_ind) {
    this->log_points = all_rooms_logs_positions[room_ind].log_points;
    for(PointT point: *log_points){
        drawTrSaveAsset(screen, logs_image,
                        point.x * h_TEXTURE_SIZE - h_TEXTURE_SIZE / 2,
                        point.y * h_TEXTURE_SIZE);
    }
}

void Logs::RemoveLog(int removing_ind, GlobalState &globalState, ObjectBorders &borders) {
    assert(removing_ind < log_points->size());
    PointT p = (*log_points)[removing_ind];
    borders = ObjectBorders{p.x - 1, p.x + 1, p.y, p.y+1};
    log_points->erase(log_points->begin() + removing_ind);
    attention_points.erase(attention_points.begin() + removing_ind);
    globalState.log_points = *log_points; // todo сделать по ссылке
}

void Logs::DrawUpdate(Image &screen, GLfloat deltaTime) {
    last_update_time += deltaTime;
    float k = 2;
    last_direction_change_time = fmod((last_direction_change_time + deltaTime), k);
    if (last_update_time < 0.05){ return;}
    last_update_time = 0;
    for(int i = 0; i < attention_points.size(); ++i){
        Point p = attention_points[i];
        for (int y = p.y; y < p.y + attention_image->Height(); ++y) {
            for (int x = p.x; x < p.x + attention_image->Width(); ++x) {
                screen.PutPixel(x, y, screen.GetPixel(x, y));
            }
        }
        if (last_direction_change_time < (k / 2)) {  p.y += 1; }
        else { p.y -= 1;}
        drawTrAsset(screen, attention_image, p.x, p.y);
        attention_points[i] = p;
    }
}
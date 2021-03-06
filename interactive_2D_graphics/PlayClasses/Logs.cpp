#include <string>
#include "Logs.h"

Logs::Logs(const std::string &logs_asset, const std::string &attention_asset) {
    this->logs_image = std::make_shared<Image>(logs_asset);
    this->attention_image = std::make_shared<Image>(attention_asset);
    this->_height = logs_image->Height();
    this->_weight = logs_image->Width();
}

bool Logs::initLogPoint(PointT & point){
    for (int i = 0; i < 3; ++i){
        point =  getRandomPoint();
        ObjectBorders borders = ObjectBorders{point.x-1,point.x+1, point.y-1, point.y+1 };
        PointT t;
        if (!titleTypeIntersection(borders, h_walls, _background_map, t)){ return true; }
    }
    return false; // не удалось найти подходящее место
}

void Logs::DrawRoom(Image &screen, GlobalState &global_state) {
    _room_ind = global_state.GetRoomInd();
    _logs_number = global_state.GetLogsNumber();
    _background_map = global_state.room_background_map;
    for (int i = 0; i < _logs_number; ++i){
        PointT point;
//        if (initLogPoint(point)){
            point = PointT{2,2}; //todo
            this->log_points.push_back(point);
            std::cout<<"Draw log x="<<point.x<<", y="<<point.y<<"\n";
            drawSaveAsset(screen, logs_image,
                            point.x*h_TEXTURE_SIZE - h_TEXTURE_SIZE/2,
                            point.y*h_TEXTURE_SIZE);
        break;

//        }
    }  // todo нужно сделать по ссылке, но мне лень дебажить
    global_state.log_points = std::vector<PointT>(this->log_points);
}

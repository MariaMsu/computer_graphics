#ifndef MAIN_LOGS_H
#define MAIN_LOGS_H


#include <vector>
#include <memory>
#include <set>
#include <map>
#include "UtilsStructures.h"
#include "Image.h"
#include "Utils.h"
#include "GlobalState.h"



struct Logs {
    Logs(const std::string &logs_asset, const std::string &attention_asset);

    void DrawRoom(Image &screen, GlobalState& globalState);
    void RemoveLog(int removing_ind, GlobalState& globalState, ObjectBorders& borders);

private:

    bool initLogPoint(PointT & point);
    void drawNewRoom(Image &screen){
        for (int i = 0; i < _logs_number; ++i) {
            PointT point;
            if (!initLogPoint(point)) { continue;}
            this->log_points.push_back(point);
            std::cout << "Draw log x=" << point.x << ", y=" << point.y << "\n";
            drawTrSaveAsset(screen, logs_image,
                            point.x * h_TEXTURE_SIZE - h_TEXTURE_SIZE / 2,
                            point.y * h_TEXTURE_SIZE);
        }
    }
    void drawOldRoom(Image &screen, int room_ind){
        this->log_points = all_rooms_logs_positions[room_ind];
        for(PointT point: log_points){
            drawTrSaveAsset(screen, logs_image,
                            point.x * h_TEXTURE_SIZE - h_TEXTURE_SIZE / 2,
                            point.y * h_TEXTURE_SIZE);
        }
    }

    int _weight;
    int _height;
    int _room_ind;
    int _logs_number;
    std::shared_ptr<TitleMap> _background_map;
    std::vector<PointT> log_points;
    std::map<int, std::vector<PointT>> all_rooms_logs_positions;
    std::shared_ptr<Image> logs_image;
    std::shared_ptr<Image> attention_image;
};


#endif //MAIN_LOGS_H

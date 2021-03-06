#ifndef MAIN_LOGS_H
#define MAIN_LOGS_H


#include <vector>
#include <memory>
#include <set>
#include "UtilsStructures.h"
#include "Image.h"
#include "Utils.h"
#include "GlobalState.h"



struct Logs {
    Logs(const std::string &logs_asset, const std::string &attention_asset);

    void DrawRoom(Image &screen, GlobalState& globalState);
    void RemoveLog(int removing_ind, GlobalState& globalState, ObjectBorders& borders){
        assert(removing_ind < log_points.size());
        PointT p = log_points[removing_ind];
        borders = ObjectBorders{p.x-1, p.x+1, p.y, p.y};
        log_points.erase(log_points.begin() + removing_ind);
        globalState.log_points= log_points;
        std::cout<<"remove x="<<p.x<<", y="<<p.y<<"\n";
    }

private:

    bool initLogPoint(PointT & point);

    int _weight;
    int _height;
    int _room_ind;
    int _logs_number;
    std::shared_ptr<TitleMap> _background_map;
    std::vector<PointT> log_points;
    std::vector<std::vector<PointT>> all_rooms_log_positions;
    std::shared_ptr<Image> logs_image;
    std::shared_ptr<Image> attention_image;
};


#endif //MAIN_LOGS_H

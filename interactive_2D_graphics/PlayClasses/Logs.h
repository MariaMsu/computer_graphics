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
    void RemoveLog(int removing_ind, GlobalState& globalState, ObjectBorders& borders);

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

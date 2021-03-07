#ifndef MAIN_LOGS_H
#define MAIN_LOGS_H


#include <vector>
#include <memory>
#include <set>
#include <map>
#include <glad/glad.h>
#include "UtilsStructures.h"
#include "Image.h"
#include "Utils.h"
#include "GlobalState.h"



struct Logs {
    Logs(const std::string &logs_asset, const std::string &attention_asset);

    void DrawRoom(Image &screen, GlobalState& globalState);
    void DrawUpdate(Image &screen, GLfloat deltaTime);
    void RemoveLog(int removing_ind, GlobalState& globalState, ObjectBorders& borders);

private:

    bool initLogPoint(PointT & point);
    void drawNewRoom(Image &screen);
    void drawOldRoom(Image &screen, int room_ind);

    int _weight;
    int _height;
    int _room_ind;
    int _logs_number;
    GLfloat last_update_time = 0;
    GLfloat last_direction_change_time = 0;
    std::shared_ptr<TitleMap> _background_map;
    std::vector<PointT> log_points;
    std::map<int, std::vector<PointT>> all_rooms_logs_positions;
    std::shared_ptr<Image> logs_image;
    std::shared_ptr<Image> attention_image;
    std::vector<Point> attention_points;
};


#endif //MAIN_LOGS_H

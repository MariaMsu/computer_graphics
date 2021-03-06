#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H
#include "../Constants.h"
#include "Image.h"

#include <memory>
#include <cmath>
#include <vector>
#include <iostream>

Point PointT2Point(PointT p);
double getPointsDistance(Point p1, Point p2);
double detNearestPointT(Point main_point, std::vector<PointT>& points, int & nearest_index);
bool titleTypeIntersection(ObjectBorders borders, const std::set<short> &title_types,
                           const std::shared_ptr<TitleMap> &room_background_map, PointT &intersection);

Pixel blend(Pixel oldPixel, Pixel newPixel);

void drawAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawTrAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawTrSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);


int getTransitionDirection(PointT p);


#endif //MAIN_UTILS_H

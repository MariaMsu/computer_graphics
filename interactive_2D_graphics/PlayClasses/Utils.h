#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H
#include "../Constants.h"

#include <cmath>
double getPointsDistance(Point p1, Point p2);
Point PointT2Point(PointT p);

Pixel blend(Pixel oldPixel, Pixel newPixel);

void drawAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawTrSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);


int getTransitionDirection(PointT p);


#endif //MAIN_UTILS_H

#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H
#include "../Constants.h"

#include <cmath>

//Point PointT2Point(PointT p){
//    return Point{p.x * h_TEXTURE_SIZE + h_TEXTURE_SIZE / 2, p.y* h_TEXTURE_SIZE + h_TEXTURE_SIZE / 2};
//};

Pixel blend(Pixel oldPixel, Pixel newPixel);

void drawAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawTrSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);


int getTransitionDirection(PointT p);


#endif //MAIN_UTILS_H

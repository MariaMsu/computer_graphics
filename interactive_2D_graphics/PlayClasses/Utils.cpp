#include <memory>
#include "Image.h"
#include "Utils.h"

double getPointsDistance(Point p1, Point p2){
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

double detNearestPointT(Point main_point, std::shared_ptr<std::vector<PointT>>& points, int & nearest_index){
    // return distance
    double smallest_distance = h_WINDOW_WIDTH * h_WINDOW_HEIGHT * 100;  // 100 just fore clam
    int local_nearest_index = -1;
    for (int i = 0; i < points->size(); ++i){
        double distance = getPointsDistance(main_point, PointT2Point((*points)[i]));
        std::clog<<"distance: "<<distance<<" ("<<(*points)[i].x<<", "<<(*points)[i].y<<")\n";
        if (distance < smallest_distance){
            smallest_distance = distance;
            local_nearest_index = i;
        }
    }
    if (local_nearest_index < 0){        return -1;    }
    nearest_index = local_nearest_index;
    return smallest_distance;
};

Point PointT2Point(PointT p){
    return Point{p.x * h_TEXTURE_SIZE + h_TEXTURE_SIZE / 2, p.y* h_TEXTURE_SIZE + h_TEXTURE_SIZE / 2};
};

Pixel blend(Pixel oldPixel, Pixel newPixel) {
    newPixel.r = newPixel.a / 255.0 * (newPixel.r - oldPixel.r) + oldPixel.r;
    newPixel.g = newPixel.a / 255.0 * (newPixel.g - oldPixel.g) + oldPixel.g;
    newPixel.b = newPixel.a / 255.0 * (newPixel.b - oldPixel.b) + oldPixel.b;
    newPixel.a = 255;
    return newPixel;
}

void drawAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y) {
    for (int y = 0; y < asset->Height(); ++y) {
        for (int x = 0; x < asset->Width(); ++x) {
            screen.PutPixel(global_x + x, global_y + (asset->Height() - y - 1), asset->GetPixel(x, y));
        }
    }
};

void drawSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y) {
    for (int y = 0; y < asset->Height(); ++y) {
        for (int x = 0; x < asset->Width(); ++x) {
            screen.PutSavePixel(global_x + x, global_y + (asset->Height() - y - 1), asset->GetPixel(x, y));
        }
    }
};

void drawTrSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y) {
    for (int y = 0; y < asset->Height(); ++y) {
        for (int x = 0; x < asset->Width(); ++x) {
            Pixel old_pixel = screen.GetPixel(global_x + x, global_y + (asset->Height() - y - 1));
            Pixel blended = blend(old_pixel, asset->GetPixel(x, y));
            screen.PutSavePixel(global_x + x, global_y + (asset->Height() - y - 1), blended);
        }
    }
};

int getTransitionDirection(PointT p){
if (p.y == h_WINDOW_T_HEIGHT - 1){return 1;}
if (p.x == h_WINDOW_T_WIDTH - 1){return 2;}
if (p.y == 0){return 3;}
if (p.x == 0){return 4;}
return -1;
}

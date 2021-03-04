#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

Pixel blend(Pixel oldPixel, Pixel newPixel);

void drawAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);
void drawTrSaveAsset(Image &screen, const std::shared_ptr<Image> &asset, int global_x, int global_y);


#endif //MAIN_UTILS_H

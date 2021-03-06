#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>
#include "UtilsStructures.h"

Pixel blend(Pixel oldPixel, Pixel newPixel);

struct Image {
    explicit Image(const std::string &a_path);
    Image(int a_width, int a_height, int a_channels);
    Image(Pixel* in_data, int a_width, int a_height, int a_channels);
    int Save(const std::string &a_path);
    int Width() const { return width; }
    int Height() const { return height; }
    int Channels() const { return channels; }
    size_t Size() const { return size; }
    Pixel *Data() { return data; }
    Pixel GetPixel(int x, int y);
    bool PutPixel(int x, int y, const Pixel &pix);
    bool PutSavePixel(int x, int y, const Pixel &pix);

    ~Image();

private:
    int width = -1;
    int height = -1;
    int channels = 3;
    size_t size = 0;
    Pixel *data = nullptr;
    Pixel *saved_data = nullptr;
    bool self_allocated = false;
    std::string a_path = "none";
};


#endif //MAIN_IMAGE_H

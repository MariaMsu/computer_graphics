#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../SupportClasses/stb_image.h"
#include "../SupportClasses/stb_image_write.h"  // todo do something with this ubozhestvo

#include <iostream>
#include <cstring>

Image::Image(const std::string &a_path) {
    if ((data = (Pixel *) stbi_load(a_path.c_str(), &width, &height, &channels, 0)) != nullptr) {
        std::cout << "path: " << a_path << "    width=" << width << "    height=" << height << "\n";
        size = width * height * channels;
        this->a_path = a_path;
        this->saved_data = new Pixel[width * height]{};
        std::memcpy(saved_data, data, width * height * sizeof(Pixel));
    }
}

Image::Image(int a_width, int a_height, int a_channels) {
    data = new Pixel[a_width * a_height]{};

    if (data != nullptr) {
        width = a_width;
        height = a_height;
        size = a_width * a_height * a_channels;
        channels = a_channels;
        self_allocated = true;
    }
    this->saved_data = new Pixel[width * height]{};
    std::memcpy(saved_data, data, width * height * sizeof(Pixel));
}

Pixel Image::GetPixel(int x, int y) {
    if ((x < 0) or (x >= this->width) or (y < 0) or (y >= this->height)) {
        std::cout << "incorrect get pixel index x: " << x << " y: " << y << this->a_path << "\n";
        return Pixel{0, 0, 0, 0};
    }
    return saved_data[width * y + x];
};

bool Image::PutPixel(int x, int y, const Pixel &pix) {
    if ((x < 0) or (x >= this->width) or (y < 0) or (y >= this->height)) {
        std::clog << "incorrect set pixel index x: "<< x << " y: " << y << ", " <<this->a_path << "\n";
        return false;
    }
    data[width * y + x] = pix;
    return true;
}

bool Image::PutSavePixel(int x, int y, const Pixel &pix) {
    if (PutPixel(x, y, pix)) {
        saved_data[width * y + x] = pix;
        return true;
    }
    return false;
}


int Image::Save(const std::string &a_path) {
    auto extPos = a_path.find_last_of('.');
    if (a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG") {
        stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
    } else if (a_path.substr(extPos, std::string::npos) == ".jpg" ||
               a_path.substr(extPos, std::string::npos) == ".JPG" ||
               a_path.substr(extPos, std::string::npos) == ".jpeg" ||
               a_path.substr(extPos, std::string::npos) == ".JPEG") {
        stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
    } else {
        std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path
                  << "\n";
        return 1;
    }

    return 0;
}

Image::~Image() {
    delete[] saved_data;
    if (self_allocated)
        delete[] data;
    else {
        stbi_image_free(data);
    }
}

#include <memory>
#include "Image.h"
#include "Utils.h"

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

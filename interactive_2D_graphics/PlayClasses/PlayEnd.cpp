#include "PlayEnd.h"

PlayEnd::PlayEnd(const std::string &path_win, const std::string &path_lose) {
    this->image_win = std::make_shared<Image>(path_win);
    this->image_lose = std::make_shared<Image>(path_lose);
}

void PlayEnd::DrawRoom(Image &screen, GlobalState &globalState) {
    drawTrSaveAsset(screen, image_win, 100, 100);
}



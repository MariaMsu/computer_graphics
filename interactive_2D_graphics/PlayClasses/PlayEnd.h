#ifndef MAIN_PLAYEND_H
#define MAIN_PLAYEND_H


#include <string>
#include <memory>
#include "Image.h"
#include "GlobalState.h"

struct PlayEnd {
    PlayEnd(const std::string &path_win, const std::string &path_lose);
    void DrawRoom(Image &screen, GlobalState& globalState);

private:
    std::shared_ptr<Image> image_win;
    std::shared_ptr<Image> image_lose;
};


#endif //MAIN_PLAYEND_H

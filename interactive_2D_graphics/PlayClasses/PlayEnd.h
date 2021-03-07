#ifndef MAIN_PLAYEND_H
#define MAIN_PLAYEND_H


#include <string>
#include <memory>
#include "Image.h"
#include "GlobalState.h"
#include "Logs.h"

struct PlayEnd {
    PlayEnd(const std::string &path_win, const std::string &path_lose);
    void DrawRoom(Image &screen, int game_state);
    void DrawUpdate(Image &screen, GLfloat deltaTime);

private:
    std::shared_ptr<Image> image_win;
    std::shared_ptr<Image> image_lose;

    GLfloat last_update_time = 0;
};


#endif //MAIN_PLAYEND_H

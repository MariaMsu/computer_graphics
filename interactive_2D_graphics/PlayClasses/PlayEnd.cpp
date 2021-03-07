#include "PlayEnd.h"

PlayEnd::PlayEnd(const std::string &path_win, const std::string &path_lose) {
    this->image_win = std::make_shared<Image>(path_win);
    this->image_lose = std::make_shared<Image>(path_lose);
}

void PlayEnd::DrawRoom(Image &screen, int game_end_state) {
    if (game_end_state == 1){
        drawTrSaveAsset(screen, image_win,
                        (h_WINDOW_WIDTH - image_win->Width()) / 2,
                        (h_WINDOW_HEIGHT - image_win->Height()) / 2);
    } else {
        drawTrSaveAsset(screen, image_lose,
                        (h_WINDOW_WIDTH - image_win->Width()) / 2,
                        (h_WINDOW_HEIGHT - image_win->Height()) / 2);
    }
}



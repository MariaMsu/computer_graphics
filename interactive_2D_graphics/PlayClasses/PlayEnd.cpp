#include "PlayEnd.h"

#define max(x,y) (((int)((x)<(y)) * (y)) + ((int)((y)<=(x)) * (x)))

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
                        (h_WINDOW_WIDTH - image_lose->Width()) / 2,
                        (h_WINDOW_HEIGHT - image_lose->Height()) / 2);
    }
}

void PlayEnd::DrawUpdate(Image &screen, GLfloat deltaTime) {
    last_update_time += deltaTime;
    if (last_update_time < 0.7) { return;}
    last_update_time = 0;
    for (int y = 0; y < h_WINDOW_HEIGHT; ++y) {
        for (int x = 0; x < h_WINDOW_WIDTH; ++x) {screen.PutPixel(x, y, screen.GetPixel(x, y));}
    }

    for (int i = 0; i < 6; ++i){
        PointT t = getRandomPoint();
        int y_position = t.y * h_TEXTURE_SIZE + (h_TEXTURE_SIZE / 4 * (t.x % 4));
        int x_offset = t.x * h_TEXTURE_SIZE / 10; // todo remove magic numbers

        for (int y = 0; y < h_TEXTURE_SIZE; ++y) {
            for (int x = 0; x < h_WINDOW_WIDTH; ++x) {
                screen.PutPixel( x, y_position + y, screen.GetPixel(max(0, x-x_offset), y_position+y));
            }
        }
    }
}

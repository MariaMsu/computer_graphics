#ifndef MAIN_GLOBALSTATE_H
#define MAIN_GLOBALSTATE_H


#include "../Constants.h"
#include "Image.h"

// по-хорошему, надо было Screnn унаследовать от Image и
// все поля ScreenState добавить в наследника
struct ScreenState {
    Pixel *background_state = new Pixel[h_WINDOW_HEIGHT * h_WINDOW_WIDTH];
    BackgroundMap* background_map = nullptr;

    ~ScreenState() {
        delete[] background_state;
//        delete background_map;
    }
};

#endif //MAIN_GLOBALSTATE_H

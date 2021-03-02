#ifndef MAIN_SCREANSTATE_H
#define MAIN_SCREANSTATE_H


#include "Constants.h"

// по-хорошему, надо было Screnn унаследовать от Image и
// все поля ScreenState добавить в наследника
struct ScreenState {
    Pixel *background_state = new Pixel[h_WINDOW_T_HEIGHT * h_WINDOW_T_HEIGHT];
};

#endif //MAIN_SCREANSTATE_H

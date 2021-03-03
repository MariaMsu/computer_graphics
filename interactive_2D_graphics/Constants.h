#ifndef MAIN_CONSTANTS_H
#define MAIN_CONSTANTS_H

#include <set>
#include <array>

#define h_WINDOW_WIDTH 768
#define h_WINDOW_HEIGHT 768

#define h_TEXTURE_SIZE 32
#define h_WINDOW_T_WIDTH h_WINDOW_WIDTH / h_TEXTURE_SIZE
#define h_WINDOW_T_HEIGHT h_WINDOW_HEIGHT / h_TEXTURE_SIZE

typedef std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT> BackgroundMap;

const std::set<char> h_walls = {'B'};

#endif //MAIN_CONSTANTS_H

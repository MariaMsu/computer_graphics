#ifndef MAIN_CONSTANTS_H
#define MAIN_CONSTANTS_H

#include <set>
#include <array>

#define h_WINDOW_WIDTH 768
#define h_WINDOW_HEIGHT 768

#define h_TEXTURE_SIZE 32
#define h_WINDOW_T_WIDTH h_WINDOW_WIDTH / h_TEXTURE_SIZE
#define h_WINDOW_T_HEIGHT h_WINDOW_HEIGHT / h_TEXTURE_SIZE

#define h_PLAYER_WIDTH 64
#define h_PLAYER_HEIGHT 44
#define h_P_PHIS_WIDTH_SHIFT 15
#define h_P_PHIS_HEIGHT_SHIFT 5

#define h_N_TRANSITIONS 4
#define h_N_ROOMS 2

typedef std::array<std::array<char, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT> TitleMap;
typedef std::array<int, h_N_TRANSITIONS> TransitionsData;

const std::set<char> h_walls = {'B', 'C', 'D', 'E', 'F'};

#endif //MAIN_CONSTANTS_H

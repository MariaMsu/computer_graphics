#ifndef MAIN_CONSTANTS_H
#define MAIN_CONSTANTS_H

#include <set>
#include <array>

#define h_WINDOW_WIDTH 768
#define h_WINDOW_HEIGHT 768

#define h_TEXTURE_SIZE 32
#define h_WINDOW_T_WIDTH h_WINDOW_WIDTH / h_TEXTURE_SIZE
#define h_WINDOW_T_HEIGHT h_WINDOW_HEIGHT / h_TEXTURE_SIZE

#define h_PLAYER_PHIS_WIDTH_SHIFT 15
#define h_PLAYER_PHIS_HEIGHT_SHIFT 5

#define h_N_TRANSITIONS 4
#define h_N_ROOMS 3
#define h_MAP_CODE_SIZE 3

typedef std::array<std::array<short, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT> TitleMap;
typedef std::array<short, h_N_TRANSITIONS> TransitionsData;

const std::set<short> h_walls = {01, 02, 03, 04, 05};
const std::set<short> h_lava = {10, 11, 12, 13, 14, 15, 16, 17, 18};
#define h_LAVA_CENTRE 14
#define h_BRIDGE_REQ_DISTANCE 200

#endif //MAIN_CONSTANTS_H

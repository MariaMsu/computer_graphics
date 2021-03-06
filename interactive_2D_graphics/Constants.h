#ifndef MAIN_CONSTANTS_H
#define MAIN_CONSTANTS_H

#include <set>
#include <array>
#include <algorithm>

#define h_PATH_ROOMS "../resources/rooms"
#define h_TXT_BACKGROUND "../resources/path_background.txt"
#define h_TXT_PLAYER "../resources/path_player.txt"
#define h_TXT_BRIDGE "../resources/path_bridges.txt"
#define h_PATH_LOG "../resources/log1.png"
#define h_PATH_LAMP "../resources/lamp8.png"

#define h_WINDOW_WIDTH 768
#define h_WINDOW_HEIGHT 768

#define h_TEXTURE_SIZE 32
#define h_WINDOW_T_WIDTH h_WINDOW_WIDTH / h_TEXTURE_SIZE
#define h_WINDOW_T_HEIGHT h_WINDOW_HEIGHT / h_TEXTURE_SIZE

#define h_PLAYER_PHIS_WIDTH_SHIFT 15
#define h_PLAYER_PHIS_HEIGHT_SHIFT 7
#define h_PLAYER_MOVE_SPEED 4
#define h_PLAYER_SKIN_SPEED 10
#define h_PLAYER_NEW_POINT_SHIFT 2

#define h_N_ROOMS 4
#define h_MAP_CODE_SIZE 3

typedef std::array<std::array<short, h_WINDOW_T_WIDTH>, h_WINDOW_T_HEIGHT> TitleMap;

const std::set<short> h_walls = {01, 02, 03, 04, 05,
                                 21, 22, 23, 24, 25, 26, 27,
                                 41, 42, 43, 44, 45,
                                 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73};
const std::set<short> h_lava = {91, 92, 93, 94, 95, 96, 97, 98, 99};
// лол. плюсы с их переобределенияе всего и вся не могут созать сет из объединения константных
const std::set<short> h_not_flore = {01, 02, 03, 04, 05,
                                     21, 22, 23, 24, 25, 26, 27,
                                     41, 42, 43, 44, 45,
                                     61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                                     91, 92, 93, 94, 95, 96, 97, 98, 99};
#define h_LAVA_CENTRE 95
#define h_BRIDGE_REQ_DISTANCE 160
#define h_LOGS_REQ_DISTANCE 70
#define h_INIT_LOGS_NUMBER 8

#define h_N_ROOM_SIDES 4

#endif //MAIN_CONSTANTS_H

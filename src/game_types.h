#include <stdint.h>

enum player { PLAYER1 = 0, PLAYER2 = 1};

struct board {
    uint32_t holes[12];
    uint32_t points[2];
    enum player to_play;
};

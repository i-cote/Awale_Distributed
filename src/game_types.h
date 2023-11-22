#include <stdint.h>

enum player { PLAYER1 = 0, PLAYER2 = 1};

#define OPPONENT(player) (player == PLAYER2 ? PLAYER1 : PLAYER2)


struct board {
    uint32_t holes[12];
    uint32_t points[2];
    enum player to_play;
};

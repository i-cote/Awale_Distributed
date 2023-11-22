#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <stdint.h>

enum player { PLAYER1 = 0, PLAYER2 = 1};

#define OPPONENT(player) (player == PLAYER2 ? PLAYER1 : PLAYER2)
#define NB_HOLES_PLAYER 6
#define WINNING_POINTS 25

enum board_status {
	NEXT_PLAY,
	INVALID_MOVE,
	GAME_ENDED
};

struct board {
    uint32_t holes[12];
    uint32_t points[2];
    enum player to_play;
};
#endif

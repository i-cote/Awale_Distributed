#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "game_types.h"

enum board_status update_board(struct board * final_board, uint32_t move);

#endif

#include "game_types.h"

struct ui_state {
    int selected;
};

/*
 * Draw the board to the screen from the view point of player
 */
void redraw(const struct board* board, enum player player, const struct ui_state* state);


void ui_init(void);
void ui_close(void);

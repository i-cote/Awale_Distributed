#ifndef UI_H
#define UI_H
#include "game_types.h"

#define DEFAULT_COLOR 1
#define PLAYING_COLOR 2
#define SELECTION_COLOR 3

struct client_state {
    struct board board;
    enum player current_player;
};

struct ui_state {
    int selected;
};

struct ui_window {
    void (*open)();
    struct ui_window* (*on_key_press)(int);
    void (*update)(struct client_state*);
    void (*close)();
};

/*
 * Draw the board to the screen from the view point of player
 */
void redraw(const struct board* board, enum player player, const struct ui_state* state);


void ui_init(void);
void ui_close(void);
#endif

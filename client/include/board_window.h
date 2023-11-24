#ifndef BOARD_WINDOW_H
#define BOARD_WINDOW_H

#include "ui.h"

extern struct ui_window board_window;

void board_window_open();
void board_window_close();
struct ui_window* board_window_on_key_press(int key);
void board_window_update(struct client_state* state);

#endif

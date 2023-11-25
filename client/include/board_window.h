#ifndef BOARD_WINDOW_H
#define BOARD_WINDOW_H

#include "ui.h"

extern struct ui_window board_window;

void board_window_open(struct app_state* state);
void board_window_close(struct app_state* state);
void board_window_on_key_press(struct app_state* state, int key);
void board_window_update(struct app_state* state, enum event* event);

#endif

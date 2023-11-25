#ifndef NAME_WINDOW_H
#define NAME_WINDOW_H

#include "ui.h"

extern struct ui_window name_window;

void name_window_open(struct app_state* state);
void name_window_close(struct app_state* state);
void name_window_on_key_press(struct app_state* state, int key);
void name_window_update(struct app_state* state, enum event* event);

#endif


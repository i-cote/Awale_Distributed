#ifndef NAME_WINDOW_H
#define NAME_WINDOW_H

#include "ui.h"

extern struct ui_window name_window;

void name_window_open();
void name_window_close();
struct ui_window* name_window_on_key_press(int key);
void name_window_update(struct client_state* state);

#endif


#ifndef LOBBY_WINDOW_H
#define LOBBY_WINDOW_H

#include "ui.h"

extern struct ui_window lobby_window;

void lobby_window_open(struct app_state* state);
void lobby_window_close(struct app_state* state);
void lobby_window_on_key_press(struct app_state* state, int key);
void lobby_window_update(struct app_state* state, enum event* event);

#endif


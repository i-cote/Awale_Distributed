#ifndef MESSAGE_WINDOW_H
#define MESSAGE_WINDOW_H

#include "ui.h"
#include <stdbool.h>

extern struct ui_window message_window;

void message_window_setup(bool interact, struct ui_window* n_window,
                          void (*callback)(struct app_state*, int,
                                           struct ui_window**),
                          const char* format, ...);
void message_window_open(struct app_state* state);
void message_window_close(struct app_state* state);
void message_window_on_key_press(struct app_state* state, int key);
void message_window_update(struct app_state* state, enum event event);

#endif

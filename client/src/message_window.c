
#include "message_window.h"
#include "app.h"
#include "board_window.h"
#include "ui.h"
#include <curses.h>
#include <form.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

struct ui_window message_window = {
    .open = message_window_open,
    .close = message_window_close,
    .update = message_window_update,
    .on_key_press = message_window_on_key_press,
};

#define MAX_MESSAGE_SIZE 256
static char message[256] = {0};

static struct ui_window* next_window;
static bool can_interact;
static void (*on_interact)(struct app_state*, int, struct ui_window**);

void message_window_open(struct app_state* state) {
    clear();
    mvprintw(1, 1, "%s", message);
    box(stdscr, ACS_VLINE, ACS_HLINE);
}

void message_window_close(struct app_state* state) {}

void message_window_on_key_press(struct app_state* state, int key) {
    if (can_interact) {
        if (on_interact != NULL) {
            on_interact(state, key, &next_window);
        }

        if (next_window != NULL) {
            app_set_next_window(next_window);
        } else {
            app_close();
        }
    }
}

void message_window_update(struct app_state* state, enum event event) {}

void message_window_setup(bool interact, struct ui_window* n_window,
                          void (*callback)(struct app_state*, int, struct ui_window**),
                          const char* format, ...) {
    can_interact = interact;
    next_window = n_window;

    va_list arglist;
    va_start(arglist, format);
    vsnprintf(message, MAX_MESSAGE_SIZE, format, arglist);
    va_end(arglist);

    on_interact = callback;
}

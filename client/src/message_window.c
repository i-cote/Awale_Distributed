
#include "message_window.h"
#include "app.h"
#include "board_window.h"
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

void message_window_open(struct app_state* state) {
    clear();
    box(stdscr, ACS_VLINE, ACS_HLINE);
    mvprintw(1, 1, "%s", message);
}

void message_window_close(struct app_state* state) {}

void message_window_on_key_press(struct app_state* state, int key) {
    if (can_interact) {
        if (next_window != NULL) {
            app_set_next_window(next_window);
        } else {
            app_close();
        }
    }
}

void message_window_update(struct app_state* state, enum event* event) {}

void message_window_set_message(const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vsnprintf(message, MAX_MESSAGE_SIZE, format, arglist);
    va_end(arglist);
}

void message_window_set_continuation(struct ui_window* n_window) {
    next_window = n_window;
}

void message_window_can_interact(bool enable) { can_interact = enable; }

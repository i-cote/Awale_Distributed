
#include "name_window.h"
#include "board_window.h"
#include <curses.h>
#include <form.h>
#include <ncurses.h>
#include <stdio.h>

struct ui_window name_window = {
    .open = name_window_open,
    .close = name_window_close,
    .update = name_window_update,
    .on_key_press = name_window_on_key_press,
};

static FIELD* fields[2];
static FORM* form;

void name_window_open() {
    fields[0] = new_field(1, 17, 0, 0, 0, 0);
    fields[1] = NULL;
    set_field_back(fields[0], A_UNDERLINE);

    form = new_form(fields);
    post_form(form);
}

void name_window_close() {
    unpost_form(form);
    free_field(fields[0]);
}

struct ui_window* name_window_on_key_press(int key) {
    switch (key) {
    case 'q':
        return NULL;
    case '\n':
        return &board_window;
    default: 
        form_driver(form, key);
        break;
    }

    return &name_window;
}

void name_window_update(struct client_state* state) {

}

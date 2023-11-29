
#include "name_window.h"
#include "app.h"
#include "board_window.h"
#include "message_window.h"
#include "protocol.h"
#include <ctype.h>
#include <curses.h>
#include <form.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

struct ui_window name_window = {
    .open = name_window_open,
    .close = name_window_close,
    .update = name_window_update,
    .on_key_press = name_window_on_key_press,
};

static FIELD* fields[3];
static FORM* form;

static char* trim_whitespaces(char* str) {
    char* end;

    // trim leading space
    while (isspace(*str))
        str++;

    if (*str == 0) // all spaces?
        return str;

    // trim trailing space
    end = str + strlen(str) - 1;

    while (end > str && isspace(*end))
        end--;

    // write new null terminator
    *(end + 1) = '\0';

    return str;
}
static void highlight_current_field() {
    FIELD* curr = current_field(form);
    for (int i = 0; fields[i] != NULL; i++) {
        if (curr == fields[i]) {
            set_field_back(fields[i], A_STANDOUT | A_UNDERLINE);
        } else {
            set_field_back(fields[i], A_UNDERLINE);
        }
    }
}

void name_window_open(struct app_state* state) {
    clear();
    fields[0] = new_field(1, 17, 1, 23, 0, 0);
    fields[1] = new_field(1, 17, 2, 23, 0, 0);
    fields[2] = NULL;

    field_opts_off(fields[1], O_PUBLIC);
    form = new_form(fields);
    post_form(form);
    mvprintw(1, 1, "Enter your name     : ");
    mvprintw(2, 1, "Enter your password : ");
    box(stdscr, ACS_VLINE, ACS_HLINE);
    highlight_current_field();
}

void name_window_close(struct app_state* state) {
    unpost_form(form);
    free_form(form);
    free_field(fields[0]);
    free_field(fields[1]);
}

void name_window_on_key_press(struct app_state* state, int key) {
    switch (key) {
    case 'q':
        app_close();
        break;
    case KEY_UP:
        form_driver(form, REQ_PREV_FIELD);
        highlight_current_field();
        break;
    case KEY_DOWN:
        form_driver(form, REQ_NEXT_FIELD);
        highlight_current_field();
        break;
    case '\t':
        form_driver(form, REQ_NEXT_FIELD);
        highlight_current_field();
        break;
    case '\n':
        form_driver(form, REQ_NEXT_FIELD);
        form_driver(form, REQ_PREV_FIELD);

        state->state = WAITING_FOR_SERVER_LOGIN_RES;
        char* name = trim_whitespaces(field_buffer(fields[0], 0));
        char* password = trim_whitespaces(field_buffer(fields[1], 0));
        if (strlen(name) == 0 || strlen(password) == 0) {
            message_window_setup(true, &name_window, NULL,
                                 "Name and password connot be empty");
            app_set_next_window(&message_window);
        } else {
            message_window_setup(false, NULL, NULL, "Connection as %s...",
                                 name);
            app_set_next_window(&message_window);

            send_packet(state->connection, LOGIN, "%s %s", name, password);
            strncpy(state->name, name, MAX_NAME_LEN);
        }
        form_driver(form, REQ_CLR_FIELD);
        break;
    case KEY_BACKSPACE:
        form_driver(form, REQ_DEL_PREV);
        break;
    default:
        form_driver(form, key);
        break;
    }
}

void name_window_update(struct app_state* state, enum event event) {}

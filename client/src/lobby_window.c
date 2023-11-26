
#include "lobby_window.h"

#include "app.h"
#include "player_list.h"
#include <curses.h>
#include <menu.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

struct ui_window lobby_window = {
    .open = lobby_window_open,
    .close = lobby_window_close,
    .update = lobby_window_update,
    .on_key_press = lobby_window_on_key_press,
};

static ITEM* items[MAX_PLAYERS + 1];
static MENU* menu;

static void create_menu(struct app_state* state) {
    size_t i = 0;
    PLAYER_FOREACH(player, state->players_in_lobby) {
        items[i++] = new_item(player->name, "");
    }
    items[i] = NULL;

    menu = new_menu(items);
    post_menu(menu);
}

static void destroy_menu() {
    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; items[i] != NULL; i++) {
        free_item(items[i]);
    }
    items[0] = NULL;
}

void lobby_window_open(struct app_state* state) {
    clear();
    create_menu(state);
}

void lobby_window_close(struct app_state* state) {
    destroy_menu();
}

void lobby_window_on_key_press(struct app_state* state, int key) {
    switch (key) {
    case KEY_UP:
        menu_driver(menu, REQ_UP_ITEM);
        break;
    case KEY_DOWN:
        menu_driver(menu, REQ_DOWN_ITEM);
        break;
    default:
        menu_driver(menu, key);
        break;
    }
}

void lobby_window_update(struct app_state* state, enum event event) {
    if (event == EV_LOBBY_UPDATE) {
        destroy_menu();
        erase();
        create_menu(state);
    }
}

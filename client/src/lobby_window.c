
#include "lobby_window.h"

#include "app.h"
#include "game_types.h"
#include "message_window.h"
#include "player_list.h"
#include "protocol.h"
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

enum selected_panel {
    P_LOBBY,
    P_GAME,
};

static ITEM* lobby_items[MAX_PLAYERS + 1];
static MENU* lobby_menu;

static ITEM* game_items[MAX_PLAYERS + 1];
static MENU* game_menu;

static enum selected_panel selected_panel = P_LOBBY;

static void create_lobby_menu(struct app_state* state) {
    WINDOW* surounding_window = subwin(stdscr, LINES, COLS / 2, 0, 0);
    WINDOW* menu_window = derwin(surounding_window, LINES - 3, (COLS / 2) - 3, 2, 2);
    size_t i = 0;
    PLAYER_FOREACH(player, state->players_in_lobby) {
        lobby_items[i++] = new_item(player->name, "");
    }
    lobby_items[i] = NULL;

    lobby_menu = new_menu(lobby_items);
    set_menu_win(lobby_menu, surounding_window);
    set_menu_sub(lobby_menu, menu_window);
    post_menu(lobby_menu);

    if (selected_panel == P_LOBBY) {
        wattron(surounding_window, COLOR_PAIR(PLAYING_COLOR));
    }
    box(surounding_window, ACS_VLINE, ACS_HLINE);
    mvwprintw(surounding_window, 0, 1, "Players in lobby");
    if (selected_panel == P_LOBBY) {
        wattroff(surounding_window, COLOR_PAIR(PLAYING_COLOR));
    }
}

static void create_game_menu(struct app_state* state) {
    WINDOW* surounding_window = subwin(stdscr, LINES, COLS / 2, 0,  COLS / 2);
    WINDOW* menu_window = derwin(surounding_window, LINES - 3, (COLS / 2) - 3,  2, 2);
    size_t i = 0;
    PLAYER_FOREACH(player, state->players_in_game) {
        game_items[i++] = new_item(player->name, "");
    }
    game_items[i] = NULL;

    game_menu = new_menu(game_items);
    set_menu_win(game_menu, surounding_window);
    set_menu_sub(game_menu, menu_window);
    post_menu(game_menu);

    if (selected_panel == P_GAME) {
        wattron(surounding_window, COLOR_PAIR(PLAYING_COLOR));
    }
    box(surounding_window, ACS_VLINE, ACS_HLINE);
    mvwprintw(surounding_window, 0, 1, "Players in game");
    if (selected_panel == P_GAME) {
        wattroff(surounding_window, COLOR_PAIR(PLAYING_COLOR));
    }
}

static void destroy_lobby_menu() {
    unpost_menu(lobby_menu);
    free_menu(lobby_menu);
    for (int i = 0; lobby_items[i] != NULL; i++) {
        free_item(lobby_items[i]);
    }
    lobby_items[0] = NULL;
}

static void destroy_game_menu() {
    unpost_menu(game_menu);
    free_menu(game_menu);
    for (int i = 0; game_items[i] != NULL; i++) {
        free_item(game_items[i]);
    }
    game_items[0] = NULL;
}

void lobby_window_open(struct app_state* state) {
    clear();
    create_lobby_menu(state);
    create_game_menu(state);
}

void lobby_window_close(struct app_state* state) {
    destroy_lobby_menu();
    destroy_game_menu();
}

static void challenge_cancel(struct app_state* state, int key,
                             struct ui_window** continuation) {
    send_packet(state->connection, CHALLENGE_CANCEL, "");
}

static void challenge_player(struct app_state* state) {
    const char* player = item_name(current_item(lobby_menu));
    if (player == NULL) 
        return;
    send_packet(state->connection, CHALLENGE_PLAYER, player);
    message_window_setup(true, &lobby_window, challenge_cancel, "En attente de reponse...");
    app_set_next_window(&message_window);
}

static void spec_player(struct app_state* state) {
    const char* player = item_name(current_item(game_menu));
    if (player == NULL) 
        return;
    send_packet(state->connection, SPEC, player);
    message_window_setup(false, &lobby_window, NULL, "Loading...");
    app_set_next_window(&message_window);
}

static void redraw_all(struct app_state* state) {
    destroy_lobby_menu();
    destroy_game_menu();
    erase();
    create_lobby_menu(state);
    create_game_menu(state);
}

void lobby_window_on_key_press(struct app_state* state, int key) {
    MENU* menu = selected_panel == P_LOBBY ? lobby_menu : game_menu;
    switch (key) {
    case KEY_UP:
        menu_driver(menu, REQ_UP_ITEM);
        break;
    case KEY_DOWN:
        menu_driver(menu, REQ_DOWN_ITEM);
        break;
    case KEY_RIGHT:
        selected_panel = P_GAME;
        redraw_all(state);
        break;
    case KEY_LEFT:
        selected_panel = P_LOBBY;
        redraw_all(state);
        break;
    case '\n':
        if (selected_panel == P_LOBBY) {
            challenge_player(state);
        } else {
            spec_player(state);
        }
        break;
    default:
        menu_driver(menu, key);
        break;
    }
}

void lobby_window_update(struct app_state* state, enum event event) {
    if (event == EV_LOBBY_UPDATE) {
        redraw_all(state);
    }
}

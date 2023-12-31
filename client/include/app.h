#ifndef APP_H
#define APP_H

#include "game_types.h"
#include "protocol.h"

enum state {
    CONNECTION,
    WAITING_FOR_LOGIN,
    WAITING_FOR_SERVER_LOGIN_RES,
    LOBBY,
    PLAY,
    SPECTATOR,
};

enum event {
    EV_BOARD_UPDATE,
    EV_LOBBY_UPDATE,
};

struct app_state {
    struct board board;
    enum player current_player;
    struct connection* connection;
    struct player_list* players_in_lobby;
    struct player_list* players_in_game;
    enum state state;
    char name[MAX_NAME_LEN];
    char opponent[MAX_NAME_LEN];
    char spectated[MAX_NAME_LEN];
};

struct ui_window;

void app_start(const char* addr, int port);

void app_set_next_window(struct ui_window* window);

void app_on_new_packet(struct app_state* state, struct packet* packet);

void app_close();

#endif

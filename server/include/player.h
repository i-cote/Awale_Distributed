#ifndef PLAYER_H
#define PLAYER_H

#include "game_types.h"
#include <protocol.h>


enum player_state {
    WAITING_FOR_LOGIN,
    LOBBY,
    PLAY,
    SPECTATOR,
    DISCONNECTED,
    DESTROYED,
};

struct game {
    struct board board;
    struct connected_player* players[2];
    struct connected_player* spectator[MAX_PLAYERS];
    size_t spectator_count;
};

struct connected_player {
    struct connection* connection;
    enum player_state state;
    struct connected_player* challenged_player;
    struct game* game;
    char name[MAX_NAME_LEN + 1];
    char password[MAX_NAME_LEN + 1];
};

#endif

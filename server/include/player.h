#ifndef PLAYER_H
#define PLAYER_H

#include <protocol.h>

#define MAX_NAME_LEN 16

enum player_state {
    WAITING_FOR_LOGIN,
    LOBBY,
    PLAY,
    SPEC,
};

struct connected_player {
    struct connection* connection;
    enum player_state state;
    char name[MAX_NAME_LEN];
};

#endif

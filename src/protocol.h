#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>

enum server_message {
    BOARD_UPDATED = 0,
    PLAYER_ASSIGED = 1,
    SPECATOR_ASSIGNED = 2,
};

enum client_message {
    DECLARE_PLAYER = 0,
    DECLARE_SPEC = 1,
    PLAY = 2,
};

struct connection {
    int socketfd;
    size_t buf_pos;
    char buffer[1024];
};

void send_declare_player(struct connection*);

void send_declare_spec(struct connection*);

void send_move(struct connection*, int hole);

#endif


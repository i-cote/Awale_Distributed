#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>

#define UNKNOWN_TYPE -2
#define SOCKET_ERROR -1
#define SOCKET_SUCCESS 0
#define PACKET_INCOMPLETE 1
#define PLAYER_ASSIGED 2 // assign <0|1>
#define SPECATOR_ASSIGNED 3 // spec_assign
#define DECLARE_PLAYER 4 // player
#define DECLARE_SPEC 5 // spec
#define MAKE_MOVE 6 // move <0..5>
#define BOARD_UPDATED 7 // update <holes> <scores> <to_play>

#define BUFFER_LEN 1024
struct connection {
    int socketfd;
    size_t buf_size;
    size_t last_packet_size;
    char read_buffer[BUFFER_LEN];
};

struct packet {
    int type;
    const char* payload;
};

int send_declare_player(struct connection*);

int send_declare_spec(struct connection*);

int send_move(struct connection*, int hole);

struct packet receive(struct connection* conn);

#endif

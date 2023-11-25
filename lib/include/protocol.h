#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include "game_types.h"

#define MAX_PLAYERS 128

#define SOCKET_ERROR -1
#define SOCKET_SUCCESS 0
enum packet_type {
    UNKNOWN_TYPE = 0,
    PACKET_INCOMPLETE,
    PLAYER_ASSIGN, // assign <0|1>
    LOGIN, // login <player>
    MAKE_MOVE, // move <0..5>
    BOARD_UPDATED, // update <holes> <scores> <to_play>
    PLAYER_JOIN_LOBBY, // player_join <name>
    PLAYER_QUIT_LOBBY, // player_quit <name>
    CHALLENGE_PLAYER, // challenge <name>
    CHALLENGE_RECEIVE,
    CHALLENGE_ACCEPT, // challenge_refuse <name>
    CHALLENGE_REFUSE, // challenge_refuse <name>
    ACK,
    ERROR,
};

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

struct connection* create_connection(int socketfd);
void destroy_connection(struct connection* connection);

int send_packet(struct connection* conn, enum packet_type type, const char* payload);

int send_error(struct connection* conn, const char* err);

int send_ack(struct connection* conn);

int send_login(struct connection* conn, const char* name);

int send_move(struct connection*, int hole);

int send_update(struct connection* conn, const struct board* board);

int send_player_assigned(struct connection* conn, enum player player);

struct packet receive(struct connection* conn);

#endif

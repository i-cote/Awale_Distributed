#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdbool.h>
#include <stdlib.h>
#include "game_types.h"

#define MAX_PLAYERS 128
#define MAX_NAME_LEN 16
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

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
    PLAYER_JOIN_GAME, 
    PLAYER_QUIT_GAME, 
    PLAYER_QUIT_LOBBY, // player_quit <name>
    CHALLENGE_PLAYER, // challenge <name>
    CHALLENGE_RECEIVE,
    CHALLENGE_ACCEPT, // challenge_accept
    CHALLENGE_REFUSE, // challenge_refuse 
    CHALLENGE_CANCEL, // challenge_cancel
    SPEC,
    SPEC_ASSIGN,
    GAME_END,
    ACK,
    ERROR,
};

#define BUFFER_LEN 1024
struct connection {
    int socketfd;
    size_t read_buf_size;
    size_t write_buf_size;
    size_t last_packet_size;
    char read_buffer[BUFFER_LEN];
    char write_buffer[BUFFER_LEN];
};

struct packet {
    int type;
    const char* payload;
};

struct connection* create_connection(int socketfd);
void destroy_connection(struct connection* connection);

bool connection_need_write(struct connection* conn);

int connection_dispatch(struct connection* conn);

bool send_packet(struct connection* conn, enum packet_type type, const char* payload_fmt, ...);

int send_update(struct connection* conn, const struct board* board);

struct packet receive(struct connection* conn);

#endif

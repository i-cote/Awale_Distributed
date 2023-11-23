#ifndef SERVER_H
#define SERVER_H

#include "player.h"
#include <stdbool.h>

#define MAX_PLAYERS 128

struct server {
    int listening_socketfd;
    size_t player_count;
    struct connected_player players[MAX_PLAYERS];
};

// Init a zero initialized server structure given to listen on the given port
// crash the program on error
void server_init(struct server* server, int port);

void server_on_new_connection(struct server* server, int sockfd);

void server_clean_disconnected_players(struct server* server);

void server_disconnect_player(struct server* server, int index);

void server_on_new_packet(struct server* server, int index, const struct packet* packet);

#endif

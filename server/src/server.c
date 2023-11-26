#include "server.h"
#include "player.h"
#include "protocol.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void server_init(struct server* server, int port) {

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin = {0};

    if (sock == -1) {
        perror("socket()");
        exit(errno);
    }

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    sin.sin_family = AF_INET;

    if (bind(sock, (struct sockaddr*)&sin, sizeof sin) == SOCKET_ERROR) {
        perror("bind()");
        exit(errno);
    }

    if (listen(sock, MAX_PLAYERS) == SOCKET_ERROR) {
        perror("listen()");
        exit(errno);
    }

    server->listening_socketfd = sock;
}

void server_on_new_connection(struct server* server, int sockfd) {
    struct connection* connection = create_connection(sockfd);
    server->players[server->player_count++] = (struct connected_player){
        .connection = connection,
        .state = WAITING_FOR_LOGIN,
    };
}

void server_clean_disconnected_players(struct server* server) {
    while (server->player_count > 0 &&
           server->players[server->player_count - 1].connection == NULL) {
        server->player_count--;
    }

    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i].connection == NULL) {
            server->players[i] = server->players[server->player_count - 1];
            server->player_count--;
            while (server->player_count > 0 &&
                   server->players[server->player_count - 1].connection ==
                       NULL) {
                server->player_count--;
            }
        }
    }
}
void server_disconnect_player(struct server* server, int index) {
    destroy_connection(server->players[index].connection);
    enum player_state last_state = server->players[index].state;
    server->players[index].state = DISCONNECTED;
    server->players[index].connection = NULL;
    printf("disconnected\n");

    if (last_state == LOBBY)
        server_send_packet_to_lobby(server, PLAYER_QUIT_LOBBY, server->players[index].name);
}

void server_on_new_packet(struct server* server, int index,
                          const struct packet* packet) {
    struct connected_player* source = &server->players[index];

    switch (packet->type) {
    case LOGIN:
        if (source->state != WAITING_FOR_LOGIN) {
            send_error(source->connection, "Already logged in");
            return;
        }

        for (int i = 0; i < server->player_count; i++) {
            if (strncmp(server->players[i].name, packet->payload,
                        MAX_NAME_LEN) == 0) {
                send_error(source->connection, "Name already used");
                return;
            }
        }

        send_packet(source->connection, ACK, "");
        strncpy(source->name, packet->payload, MAX_NAME_LEN);
        for (int i = 0; i < server->player_count; i++) {
            if (server->players[i].state == LOBBY)
                send_packet(source->connection, PLAYER_JOIN_LOBBY, server->players[i].name);
        }
        server_send_packet_to_lobby(server, PLAYER_JOIN_LOBBY, source->name);
        source->state = LOBBY;
        break;
    }
}
void server_send_packet_to_lobby(struct server* server, enum packet_type type,
                                 const char* payload) {
    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i].state == LOBBY)
            send_packet(server->players[i].connection, type, payload);
    }
}

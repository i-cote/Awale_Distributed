#include "server.h"
#include "game.h"
#include "game_types.h"
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
    int i = server->player_count++;
    server->players[i] = malloc(sizeof(struct connected_player));
    *server->players[i] = (struct connected_player){
        .connection = connection,
        .state = WAITING_FOR_LOGIN,
    };
}

static void player_join_lobby(struct server* server,
                              struct connected_player* player) {

    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i]->state == LOBBY)
            send_packet(player->connection, PLAYER_JOIN_LOBBY,
                        server->players[i]->name);
        if (server->players[i]->game != NULL && server->players[i]->state == PLAY)
            send_packet(player->connection, PLAYER_JOIN_GAME,
                        server->players[i]->name);
    }
    server_send_packet_to_lobby(server, PLAYER_JOIN_LOBBY, player->name);
    player->state = LOBBY;
}

static void clean_player(struct server* server,
                         struct connected_player* disconnected_player) {
    for (int i = 0; i < server->player_count; i++) {
        struct connected_player* player = server->players[i];
        if (player->challenged_player == disconnected_player) {
            player->challenged_player = NULL;
            send_packet(player->connection, CHALLENGE_REFUSE,
                        "Le joueur s'est déconnecté.");
        }
    }

    if (disconnected_player->state == PLAY && disconnected_player->game != NULL) {
        server_send_packet_to_lobby(server, PLAYER_QUIT_GAME,
                                    disconnected_player->name);
        struct game* game = disconnected_player->game;
        struct connected_player* other = NULL;
        for (int i = 0; i < 2; i++) {
            if (game->players[i] != disconnected_player) {
                other = game->players[i];
            }
        }

        send_packet(other->connection, GAME_END, "You win by forfait");
        other->game = NULL;
        player_join_lobby(server, other);
        for (int i = 0; i < game->spectator_count; i++) {
            send_packet(game->spectator[i]->connection,
                        GAME_END, "%s wins by forfait", other->name);
            game->spectator[i]->game = NULL;
            player_join_lobby(server, game->spectator[i]);
        }

        server_send_packet_to_lobby(server, PLAYER_QUIT_GAME, other->name);

        free(game);
    } else if (disconnected_player->state == SPECTATOR && disconnected_player->game != NULL) {
        struct game* game = disconnected_player->game;
        for (int i = 0; i < game->spectator_count; i++) {
            if (game->spectator[i] == disconnected_player) {
                game->spectator[i] = game->spectator[game->spectator_count - 1];
                game->spectator_count--;
            }
        }
    }

    free(disconnected_player);
}

void server_clean_disconnected_players(struct server* server) {
    static struct connected_player* removed_player[MAX_PLAYERS];

    size_t removed_player_count = 0;
    while (server->player_count > 0 &&
           server->players[server->player_count - 1]->connection == NULL) {
        removed_player[removed_player_count++] =
            server->players[server->player_count - 1];
        server->player_count--;
    }

    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i]->connection == NULL) {
            removed_player[removed_player_count++] = server->players[i];
            server->players[i] = server->players[server->player_count - 1];
            server->player_count--;
            while (server->player_count > 0 &&
                   server->players[server->player_count - 1]->connection ==
                       NULL) {
                removed_player[removed_player_count++] =
                    server->players[server->player_count - 1];
                server->player_count--;
            }
        }
    }

    for (int i = 0; i < removed_player_count; i++) {
        clean_player(server, removed_player[i]);
    }
}
void server_disconnect_player(struct server* server, int index) {
    destroy_connection(server->players[index]->connection);
    enum player_state last_state = server->players[index]->state;
    server->players[index]->connection = NULL;
    printf("disconnected\n");

    if (last_state == LOBBY)
        server_send_packet_to_lobby(server, PLAYER_QUIT_LOBBY,
                                    server->players[index]->name);
}

static void handle_login_packet(struct server* server,
                                struct connected_player* source,
                                const struct packet* packet) {

    if (source->state != WAITING_FOR_LOGIN) {
        send_packet(source->connection, ERROR, "Already logged in");
        return;
    }

    for (int i = 0; i < server->player_count; i++) {
        if (strncmp(server->players[i]->name, packet->payload, MAX_NAME_LEN) ==
            0) {
            send_packet(source->connection, ERROR, "Name already used");
            return;
        }
    }

    send_packet(source->connection, ACK, "");
    strncpy(source->name, packet->payload, MAX_NAME_LEN);
    player_join_lobby(server, source);
}

static void handle_challenge_packet(struct server* server,
                                    struct connected_player* source,
                                    const struct packet* packet) {
    if (packet->payload == NULL) {
        send_packet(source->connection, ERROR, "Paquet malforme");
        return;
    }

    if (source->challenged_player != NULL) {
        send_packet(source->connection, ERROR, "Vous defiez deja quelqu'un.");
        return;
    }

    struct connected_player* target = NULL;
    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i]->state == LOBBY &&
            strcmp(server->players[i]->name, packet->payload) == 0) {
            target = server->players[i];
        }
    }

    if (target == NULL) {
        send_packet(source->connection, ERROR, "Joueur non trouve.");
        return;
    }

    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i]->state == LOBBY &&
            server->players[i]->challenged_player == target) {
            send_packet(source->connection, CHALLENGE_REFUSE,
                        "Ce joueur est déjà solicite.");
            return;
        }
    }

    source->challenged_player = target;
    send_packet(target->connection, CHALLENGE_RECEIVE, source->name);
}

static void handle_challenge_refuse_packet(struct server* server,
                                           struct connected_player* source,
                                           const struct packet* packet) {

    struct connected_player* challenger = NULL;
    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i]->challenged_player == source) {
            challenger = server->players[i];
            break;
        }
    }

    if (challenger == NULL) {
        return;
    }

    challenger->challenged_player = NULL;
    send_packet(challenger->connection, CHALLENGE_REFUSE,
                "Le joueur a refuse votre challenge.");
}

static void handle_challenge_accept_packet(struct server* server,
                                           struct connected_player* source,
                                           const struct packet* packet) {
    struct connected_player* challenger = NULL;
    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i]->challenged_player == source) {
            challenger = server->players[i];
            break;
        }
    }

    if (challenger == NULL) {
        return;
    }

    challenger->challenged_player = NULL;
    challenger->state = PLAY;
    source->state = PLAY;

    server_send_packet_to_lobby(server, PLAYER_QUIT_LOBBY, challenger->name);
    server_send_packet_to_lobby(server, PLAYER_QUIT_LOBBY, source->name);

    server_send_packet_to_lobby(server, PLAYER_JOIN_GAME, challenger->name);
    server_send_packet_to_lobby(server, PLAYER_JOIN_GAME, source->name);

    struct game* game = malloc(sizeof(struct game));

    source->game = game;
    challenger->game = game;

    *game = (struct game){
        .board =
            {
                .holes = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
                // .holes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
            },
    };

    if (rand() < RAND_MAX / 2) {
        game->players[0] = challenger;
        game->players[1] = source;
    } else {
        game->players[0] = source;
        game->players[1] = challenger;
    }

    for (int i = 0; i < 2; i++) {
        send_packet(game->players[i]->connection, PLAYER_ASSIGN, "%d %s", i,
                    game->players[OPPONENT(i)]->name);
        send_update(game->players[i]->connection, &game->board);
    }
}
static void handle_challenge_cancel_packet(struct server* server,
                                           struct connected_player* source,
                                           const struct packet* packet) {
    if (source->challenged_player != NULL) {
        send_packet(source->challenged_player->connection, CHALLENGE_CANCEL,
                    "");
        source->challenged_player = NULL;
    }
}

static void handle_move_packet(struct server* server,
                               struct connected_player* source,
                               const struct packet* packet) {
    if (source->game == NULL) {
        return;
    }

    if (source->game->players[source->game->board.to_play] == source) {
        int move;
        if (sscanf(packet->payload, "%d", &move) == 1) {
            enum board_status status = update_board(&source->game->board, move);
            for (int i = 0; i < 2; i++) {
                send_update(source->game->players[i]->connection,
                            &source->game->board);
            }

            for (int i = 0; i < source->game->spectator_count; i++) {
                send_update(source->game->spectator[i]->connection,
                            &source->game->board);
            }

            if (status == GAME_ENDED) {
                struct board* board = &source->game->board;
                if (board->points[PLAYER1] == board->points[PLAYER2]) {
                    for (int i = 0; i < 2; i++) {
                        send_packet(source->game->players[i]->connection,
                                    GAME_END, "Draw");
                    }
                    for (int i = 0; i < source->game->spectator_count; i++) {
                        send_packet(source->game->spectator[i]->connection,
                                    GAME_END, "Draw");
                    }
                } else {
                    enum player winner;
                    if (board->points[PLAYER1] > board->points[PLAYER2]) {
                        winner = PLAYER1;
                    } else {
                        winner = PLAYER2;
                    }

                    send_packet(source->game->players[winner]->connection,
                                GAME_END, "You win");
                    send_packet(
                        source->game->players[OPPONENT(winner)]->connection,
                        GAME_END, "You lose");

                    for (int i = 0; i < source->game->spectator_count; i++) {
                        send_packet(source->game->spectator[i]->connection,
                                    GAME_END, "%s wins", source->game->players[winner]->name);
                        source->game->spectator[i]->game = NULL;
                        player_join_lobby(server, source->game->spectator[i]);
                    }
                }
                struct game* game = source->game;
                for (int i = 0; i < 2; i++) {
                    game->players[i]->game = NULL;
                    server_send_packet_to_lobby(server, PLAYER_QUIT_GAME,
                                                game->players[i]->name);
                    player_join_lobby(server, game->players[i]);
                }
                free(game);
            }
        }
    }
}

static void handle_spec_packet(struct server* server,
                               struct connected_player* source,
                               const struct packet* packet) {

    struct connected_player* target = NULL;
    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i]->state == PLAY &&
            strcmp(server->players[i]->name, packet->payload) == 0) {
            target = server->players[i];
        }
    }

    printf("%s\n", packet->payload);

    if (target == NULL) {
        send_packet(source->connection, ERROR, "Joueur non trouve.");
        return;
    }

    if (target->state != PLAY || target->game == NULL) {
        send_packet(source->connection, ERROR, "Not playing");
    }

    enum player p;
    if (target->game->players[PLAYER1] == target) {
        p = PLAYER1;
    } else {
        p = PLAYER2;
    }

    target->game->spectator[target->game->spectator_count++] = source;

    send_packet(source->connection, SPEC_ASSIGN, "%d %s %s", p,
                target->game->players[p]->name,
                target->game->players[OPPONENT(p)]->name);

    send_update(source->connection, &target->game->board);
    source->game = target->game;
    source->state = SPECTATOR;
}
void server_on_new_packet(struct server* server,
                          struct connected_player* source,
                          const struct packet* packet) {

    switch (packet->type) {
    case LOGIN:
        handle_login_packet(server, source, packet);
        break;
    case CHALLENGE_PLAYER:
        handle_challenge_packet(server, source, packet);
        break;
    case CHALLENGE_REFUSE:
        handle_challenge_refuse_packet(server, source, packet);
        break;
    case CHALLENGE_ACCEPT:
        handle_challenge_accept_packet(server, source, packet);
        break;
    case CHALLENGE_CANCEL:
        handle_challenge_cancel_packet(server, source, packet);
        break;
    case MAKE_MOVE:
        handle_move_packet(server, source, packet);
        break;
    case SPEC:
        handle_spec_packet(server, source, packet);
        break;
    default:
        send_packet(source->connection, ERROR, "Bad packet");
        break;
    }
}
void server_send_packet_to_lobby(struct server* server, enum packet_type type,
                                 const char* payload) {
    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i]->state == LOBBY)
            send_packet(server->players[i]->connection, type, payload);
    }
}

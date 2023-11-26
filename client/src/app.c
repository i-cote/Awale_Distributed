#include "app.h"
#include "lobby_window.h"
#include "message_window.h"
#include "name_window.h"
#include "player_list.h"
#include "protocol.h"
#include "ui.h"

#include <arpa/inet.h>
#include <errno.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static struct ui_window* current_window;
static struct ui_window* next_window;
static bool window_changed;
static bool running;
static struct pollfd fds[2];

static struct connection* connect_to_server(const char* addr, int port) {
    printf("Connecting...\n");
    struct sockaddr_in serv_addr = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(port);

    int socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error\n");
        exit(1);
    }

    /* effectue la connection */
    if (connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <
        0) {
        printf("Connection error\n");
        exit(1);
    }
    return create_connection(socketfd);
}
void app_start(const char* addr, int port) {

    struct app_state state = {
        .state = CONNECTION,
        .players_in_lobby = player_list_create(),
    };

    player_list_remove(state.players_in_lobby, "bla");
    state.connection = connect_to_server(addr, port);
    state.state = WAITING_FOR_LOGIN;
    current_window = next_window;

    ui_init();

    next_window = &name_window;
    current_window = next_window;
    current_window->open(&state);

    running = 1;
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    fds[1].fd = state.connection->socketfd;
    fds[1].events = POLLIN;

    refresh();

    bool lost_connection = false;
    while (running) {
        if (connection_need_write(state.connection)) {
            fds[1].events |= POLLOUT;
        } else {
            fds[1].events &= ~POLLOUT;
        }
        if (poll(fds, 2, -1) == -1) {
            if (errno == EINTR) {
                continue;
            }

            perror("Poll failed");
            printf("%d\n", errno);
            exit(errno);
        }

        if (fds[0].revents != 0) {
            current_window->on_key_press(&state, getch());
        }

        if (fds[1].revents != 0) {
            if (fds[1].revents & POLLIN) {

                struct packet packet;
                do {
                    packet = receive(state.connection);
                    if (packet.type < 0) {
                        lost_connection = true;
                        break;
                    }

                    if (packet.type != PACKET_INCOMPLETE)
                        app_on_new_packet(&state, &packet);

                } while (packet.type != PACKET_INCOMPLETE);
            }
            if (fds[1].revents & POLLOUT) {
                if (connection_dispatch(state.connection) == SOCKET_ERROR) {
                    lost_connection = true;
                    break;
                }
            }
        }

        if (window_changed) {
            current_window->close(&state);
            current_window = next_window;
            current_window->open(&state);
            window_changed = false;
        }

        refresh();
    }
    current_window->close(&state);
    if (lost_connection) {
        message_window_set_message("Lost connection");
        message_window_can_interact(true);
        message_window.open(&state);
        getch();
        message_window.close(&state);
    }

    destroy_connection(state.connection);
    player_list_free(state.players_in_lobby);

    ui_close();
}

void app_on_new_packet(struct app_state* state, struct packet* packet) {
    switch (packet->type) {
    case ACK:
        if (state->state == WAITING_FOR_SERVER_LOGIN_RES) {
            app_set_next_window(&lobby_window);
        }
        break;
    case PLAYER_JOIN_LOBBY:
        if (packet->payload != NULL) {
            player_list_add(state->players_in_lobby, packet->payload);
            // exit(42);
            current_window->update(state, EV_LOBBY_UPDATE);
        }
        break;
    case PLAYER_QUIT_LOBBY:
        if (packet->payload != NULL) {
            player_list_remove(state->players_in_lobby, packet->payload);
            current_window->update(state, EV_LOBBY_UPDATE);
        }
        break;
    case ERROR:
        message_window_can_interact(true);

        switch (state->state) {
        case WAITING_FOR_SERVER_LOGIN_RES:
            state->state = WAITING_FOR_LOGIN;
            message_window_set_continuation(&name_window);
            break;
        default:
            message_window_set_continuation(NULL);
            break;
        }
        message_window_set_message("Error from server: %s",
                                   packet->payload != NULL ? packet->payload
                                                           : "Unkown error");
        app_set_next_window(&message_window);
    }
}

void app_set_next_window(struct ui_window* window) {
    next_window = window;
    window_changed = true;
}

void app_close() { running = false; }

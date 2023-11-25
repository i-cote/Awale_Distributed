#include <errno.h>
#include <server.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
    printf("Hello from server\n");
    struct server server = {0};
    server_init(&server, 8080);

    struct pollfd fds[MAX_PLAYERS + 2]; // 1 for each connections + 1 for stdin
                                        // + 1 for listening

    while (1) {
        server_clean_disconnected_players(&server);
        for (int i = 0; i < server.player_count; i++) {
            fds[i].fd = server.players[i].connection->socketfd;
            fds[i].events = POLLIN;
        }

        fds[server.player_count].fd = server.listening_socketfd;
        fds[server.player_count].events = POLLIN;

        fds[server.player_count + 1].fd = STDIN_FILENO;
        fds[server.player_count + 1].events = POLLIN;

        if (poll(fds, server.player_count + 2, -1) == -1) {
            perror("epoll error");
            return errno;
        }

        if (fds[server.player_count].revents != 0) {
            // New connection

            struct sockaddr_in csin = {0};
            socklen_t sinsize = sizeof csin;
            int csock = accept(server.listening_socketfd,
                               (struct sockaddr*)&csin, &sinsize);
            if (csock == SOCKET_ERROR) {
                perror("accept()");
                continue;
            }
            
            printf("New connection\n");
            server_on_new_connection(&server, csock);
        }

        if (fds[server.player_count + 1].revents != 0) {
            char c;
            while ((c = getchar() != '\n' && c != EOF))
                ;
            break;
        }

        for (int i = 0; i < server.player_count; i++) {
            if (fds[i].revents != 0) {
                struct packet packet = receive(server.players[i].connection);
                if (packet.type < 0) {
                    server_disconnect_player(&server, i);
                } else if (packet.type != UNKNOWN_TYPE && packet.type != PACKET_INCOMPLETE) {
                    server_on_new_packet(&server, i, &packet);
                }
            }
        }
    }

    close(server.listening_socketfd);

    return 0;
}

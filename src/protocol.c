#include "protocol.h"
#include "string.h"
#include <linux/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void send(int socketfd, const char* payload) {
    int len = strlen(payload);
    int pos = 0;
    int r;

    while ((r = write(socketfd, payload + pos, len - pos)) > 0 && pos < len) {
        pos += r;
    }
}

void send_declare_player(struct connection* conn) {
    send(conn->socketfd, "play\n");
}

void send_declare_spec(struct connection* conn) {
    send(conn->socketfd, "spec\n");
}

void send_move(struct connection* conn, int hole) {
    char message[256];
    snprintf(message, 256, "move %d\n", hole);
    send(conn->socketfd, message);
}

#include "protocol.h"
#include "string.h"
#include <linux/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PACKET_BUFFER_SIZE 256

static int send(int socketfd, const char* payload) {
    int len = strlen(payload);
    int pos = 0;
    int r;

    while ((r = write(socketfd, payload + pos, len - pos)) > 0 && pos < len) {
        if (r <= 0) {
            return SOCKET_ERROR;
        }
        pos += r;
    }
    return SOCKET_SUCCESS;
}

int send_declare_player(struct connection* conn) {
    return send(conn->socketfd, "play\n");
}

int send_declare_spec(struct connection* conn) {
    return send(conn->socketfd, "spec\n");
}

int send_move(struct connection* conn, int hole) {
    char packet_buffer[PACKET_BUFFER_SIZE];
    sprintf(packet_buffer, "move %d\n", hole);
    return send(conn->socketfd, packet_buffer);
}

int send_update(struct connection* conn, const struct board* board) {
    char packet_buffer[PACKET_BUFFER_SIZE];
    size_t current_size = 0;
    current_size += sprintf(packet_buffer, "update ");
    for (int i = 0; i < 12; i++) {
        current_size += sprintf(packet_buffer + current_size, "%d ", board->holes[i]);
    }

    for (int i = 0; i < 2; i++) {
        current_size += sprintf(packet_buffer + current_size, "%d ", board->points[i]);
    }

    
    current_size += sprintf(packet_buffer + current_size, "%d\n", board->to_play);

    return send(conn->socketfd, packet_buffer);
}

int send_player_assigned(struct connection* conn, enum player player) {
    char packet_buffer[PACKET_BUFFER_SIZE];
    sprintf(packet_buffer, "assign %d\n", player);
    return send(conn->socketfd, packet_buffer);
}

int send_spec_assigned(struct connection* conn) {
    return send(conn->socketfd, "spec_assign\n");
}

struct packet receive(struct connection* conn) {
    // First we clear the last payload to make space in the buffer
    if (conn->last_packet_size > 0) {
        memmove(conn->read_buffer, conn->read_buffer + conn->last_packet_size,
                conn->buf_size - conn->last_packet_size);
        conn->buf_size -= conn->last_packet_size;
        conn->last_packet_size = 0;
    }

    size_t bytes_read = read(conn->socketfd, conn->read_buffer + conn->buf_size,
                             BUFFER_LEN - conn->buf_size - 1);

    if (bytes_read <= 0) {
        return (struct packet){.type = SOCKET_ERROR};
    }

    conn->buf_size += bytes_read;
    // 0 terminate our buffer to be able to use string method on it
    conn->read_buffer[conn->buf_size] = 0;

    struct packet packet = {.type = PACKET_INCOMPLETE};

    char* terminator = strchr(conn->read_buffer, '\n');
    if (terminator == NULL) {
        // The packet is not complete yet we must wait the next message
        return packet;
    }
    // Null terminate the payload
    *terminator = '\0';
    conn->last_packet_size = terminator - conn->read_buffer + 1;

    char* first_word_end = strchr(conn->read_buffer, ' ');
    if (first_word_end != NULL) {
        *first_word_end = '\0';
        packet.payload = first_word_end + 1;
    }

    const char* type_str = conn->read_buffer;
    if (strcmp(type_str, "player") == 0) {
        packet.type = DECLARE_PLAYER;
    } else if (strcmp(type_str, "spec") == 0) {
        packet.type = DECLARE_SPEC;
    } else if (strcmp(type_str, "move") == 0) {
        packet.type = MAKE_MOVE;
    } else if (strcmp(type_str, "update") == 0) {
        packet.type = BOARD_UPDATED;
    } else if (strcmp(type_str, "assign") == 0) {
        packet.type = PLAYER_ASSIGED;
    } else if (strcmp(type_str, "spec_assign") == 0) {
        packet.type = SPECATOR_ASSIGNED;
    } else {
        packet.type = UNKNOWN_TYPE;
    }

    return packet;
}

#include "protocol.h"
#include "string.h"
#include <linux/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PACKET_BUFFER_SIZE 256

struct packet_lookup_entry {
    const char* text;
    enum packet_type type;
};

const char* LOOKUP_TABLE[] = {
    [LOGIN] = "login",
    [PLAYER_ASSIGN] = "assign",
    [MAKE_MOVE] = "move",
    [BOARD_UPDATED] = "update",
    [PLAYER_JOIN_LOBBY] = "player_join",
    [PLAYER_QUIT_LOBBY] = "player_quit",
    [CHALLENGE_PLAYER] = "challenge",
    [CHALLENGE_RECEIVE] = "challenge_receive",
    [CHALLENGE_ACCEPT] = "challenge_accept",
    [CHALLENGE_REFUSE] = "challenge_refuse",
};

const size_t LOOKUP_TABLE_SIZE = sizeof(LOOKUP_TABLE) / sizeof(const char*);

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

int send_login(struct connection* conn, const char* name) {
    char packet_buffer[PACKET_BUFFER_SIZE];
    sprintf(packet_buffer, "%s %s\n", LOOKUP_TABLE[LOGIN], name);
    return send(conn->socketfd, packet_buffer);
}

int send_move(struct connection* conn, int hole) {
    char packet_buffer[PACKET_BUFFER_SIZE];
    sprintf(packet_buffer, "%s %d\n", LOOKUP_TABLE[MAKE_MOVE], hole);
    return send(conn->socketfd, packet_buffer);
}

int send_update(struct connection* conn, const struct board* board) {
    char packet_buffer[PACKET_BUFFER_SIZE];
    size_t current_size = 0;
    current_size += sprintf(packet_buffer, "%s ", LOOKUP_TABLE[BOARD_UPDATED]);
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
    sprintf(packet_buffer, "%s %d\n", LOOKUP_TABLE[PLAYER_ASSIGN], player);
    return send(conn->socketfd, packet_buffer);
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
    for (int i = 0; i < LOOKUP_TABLE_SIZE; i++) {
        if (LOOKUP_TABLE[i] == NULL) continue;
        if (strcmp(LOOKUP_TABLE[i], type_str) == 0) {
            packet.type = i;
            break;
        }
    }

    return packet;
}

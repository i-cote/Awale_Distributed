#include "protocol.h"
#include "string.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/socket.h>
#include <stdarg.h>
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
    [CHALLENGE_CANCEL] = "challenge_cancel", // challenge_refuse <name>
    [ACK] = "ack",
    [ERROR] = "error",
};

const size_t LOOKUP_TABLE_SIZE = sizeof(LOOKUP_TABLE) / sizeof(const char*);

struct connection* create_connection(int socketfd) {
    struct connection* conn = malloc(sizeof(struct connection));
    fcntl(socketfd, F_SETFL, O_NONBLOCK);
    memset(conn, 0, sizeof(struct connection));
    conn->socketfd = socketfd;

    return conn;
}

void destroy_connection(struct connection* connection) {
    close(connection->socketfd);
    free(connection);
}

bool connection_need_write(struct connection* conn) {
    return conn->write_buf_size > 0;
}

int connection_dispatch(struct connection* conn) {
    int total_written = 0;
    int written = 0;
    while (total_written < conn->write_buf_size &&
           (written = write(conn->socketfd, conn->write_buffer + total_written,
                            conn->write_buf_size - total_written)) > 0) {

        total_written += written;
    }

    if (total_written == conn->write_buf_size) {
        conn->write_buf_size = 0;
    } else {
        memmove(conn->write_buffer, conn->write_buffer + total_written,
                conn->write_buf_size - total_written);
        conn->write_buf_size -= total_written;
    }

    if (written == 0) {
        return SOCKET_ERROR;
    } else if (written == -1) {
        if (errno == EAGAIN) {
            return SOCKET_SUCCESS;
        }
        return SOCKET_ERROR;
    }

    else return SOCKET_SUCCESS;
}

bool send_packet(struct connection* conn, enum packet_type type,
                 const char* payload_fmt, ...) {
    char packet_buffer[PACKET_BUFFER_SIZE];
    int packet_size = 0;
    va_list arglist;

    packet_size +=
        snprintf(packet_buffer, PACKET_BUFFER_SIZE, "%s ", LOOKUP_TABLE[type]);
    if (packet_size >= PACKET_BUFFER_SIZE) {
        return false;
    }

    va_start(arglist, payload_fmt);
    packet_size +=
        vsnprintf(packet_buffer + packet_size, PACKET_BUFFER_SIZE - packet_size,
                  payload_fmt, arglist);
    va_end(arglist);

    if (PACKET_BUFFER_SIZE < packet_size + 1) {
        return false;
    }
    packet_buffer[packet_size++] = '\n';

    if (BUFFER_LEN - conn->write_buf_size < packet_size) {
        return false;
    }

    memcpy(conn->write_buffer + conn->write_buf_size, packet_buffer,
           packet_size);

    conn->write_buf_size += packet_size;

    return true;
    // vsnprintf(message, MAX_MESSAGE_SIZE, format, arglist);
    // send(conn->socketfd, packet_buffer);
}


int send_update(struct connection* conn, const struct board* board) {
    char packet_buffer[PACKET_BUFFER_SIZE];
    size_t current_size = 0;
    for (int i = 0; i < 12; i++) {
        current_size +=
            sprintf(packet_buffer + current_size, "%d ", board->holes[i]);
    }

    for (int i = 0; i < 2; i++) {
        current_size +=
            sprintf(packet_buffer + current_size, "%d ", board->points[i]);
    }

    current_size +=
        sprintf(packet_buffer + current_size, "%d", board->to_play);

    return send_packet(conn, BOARD_UPDATED, packet_buffer);
}

struct packet receive(struct connection* conn) {
    // First we clear the last payload to make space in the buffer
    if (conn->last_packet_size > 0) {
        memmove(conn->read_buffer, conn->read_buffer + conn->last_packet_size,
                conn->read_buf_size - conn->last_packet_size);
        conn->read_buf_size -= conn->last_packet_size;
        conn->last_packet_size = 0;
    } else {
        int bytes_read =
            read(conn->socketfd, conn->read_buffer + conn->read_buf_size,
                 BUFFER_LEN - conn->read_buf_size - 1);

        if (bytes_read < 0) {
            if (errno == EAGAIN) {
                return (struct packet){.type = PACKET_INCOMPLETE};
            }
            return (struct packet){.type = SOCKET_ERROR};
        }

        if (bytes_read == 0) {
            return (struct packet){.type = SOCKET_ERROR};
        }

        conn->read_buf_size += bytes_read;
        // 0 terminate our buffer to be able to use string method on it
        conn->read_buffer[conn->read_buf_size] = 0;
    }

    struct packet packet = {.type = PACKET_INCOMPLETE};

    char* terminator = strchr(conn->read_buffer, '\n');
    if (terminator == NULL) {
        // The packet is not complete yet we must wait the next message
        return packet;
    }
    packet.type = UNKNOWN_TYPE;
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
        if (LOOKUP_TABLE[i] == NULL)
            continue;
        if (strcmp(LOOKUP_TABLE[i], type_str) == 0) {
            packet.type = i;
            break;
        }
    }

    return packet;
}

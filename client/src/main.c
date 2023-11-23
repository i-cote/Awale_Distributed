#include "protocol.h"
#include <arpa/inet.h>
#include "ui.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

void demo_ui(void);
void demo_conn(void);

int main(void) {
    // demo_ui();
    demo_conn();
    return 0;
}

void demo_conn(void) {
    struct connection conn = {0};
    struct sockaddr_in cli_addr, serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8080);

    if ((conn.socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error\n");
        exit(1);
    }

    /* effectue la connection */
    if (connect(conn.socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("socket error\n");
        exit(1);
    }

    printf("connected\n");
    struct board b = {
        .to_play = PLAYER1,
        .holes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
        .points = {2, 3},
    };
    while (1) {
        struct packet packet = receive(&conn);
        printf("%d ", packet.type);
        if (packet.payload != NULL) {
            printf("%s\n", packet.payload);
        } else {
            printf("\n");
        }
        send_update(&conn, &b);
    }
}

void demo_ui(void) {
    ui_init();

    struct board b = {
        .to_play = PLAYER1,
        .holes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
        .points = {2, 3},
    };

    struct ui_state state = {0};
    bool running = true;
    while (running) {

        clear();
        redraw(&b, PLAYER2, &state);
        refresh();

        switch (getch()) {
        case 'q':
            running = false;
            break;
        case 'l':
        case 'd':
            state.selected++;
            if (state.selected > 5)
                state.selected = 0;
            break;
        case 'h':
        case 'a':
            state.selected--;
            if (state.selected < 0)
                state.selected = 5;
            break;
        }
    }
    ui_close();
    endwin();
}

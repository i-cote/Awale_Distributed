#include "board_window.h"
#include "name_window.h"
#include "protocol.h"
#include <arpa/inet.h>
#include "ui.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

void demo_ui(void);
void demo_conn(void);

int main(int argc, char** argv) {
    demo_ui();
    // struct connection conn = {0};
    // struct sockaddr_in serv_addr = {0};
    //
    // if (argc < 3) {
    //     fputs("client ip port\n", stderr);
    //     return 1;
    // }
    //
    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    // serv_addr.sin_port = htons(atoi(argv[2]));
    //
    // if ((conn.socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    //     printf("socket error\n");
    //     exit(1);
    // }
    //
    // /* effectue la connection */
    // if (connect(conn.socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    //     printf("socket error\n");
    //     exit(1);
    // }
    //
    // while (1) {
    // }
    // demo_conn();
    // return 0;
}

void demo_conn(void) {
}

void demo_ui(void) {
    ui_init();

    struct client_state state = {0};
    struct ui_window* current_window;
    current_window = &name_window;

    current_window->open();

    while (1) {
        current_window->update(&state);
        struct ui_window* next_window = current_window->on_key_press(getch());

        if (next_window == NULL) {
            break;
        } else if (next_window != current_window) {
            current_window->close();
            current_window = next_window;
            current_window->open();
        }

    }
    board_window.close();
    ui_close();
    endwin();
}

#include "app.h"
#include "board_window.h"
#include "name_window.h"
#include "protocol.h"
#include "ui.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

void demo_ui(void);
void demo_conn(void);

int main(int argc, char** argv) {
    if (argc < 3) {
        fputs("client ip port\n", stderr);
        return 1;
    }

    app_start(argv[1], atoi(argv[2]));
    return 0;
}


#include "ui.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>

int main(void) {
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
    return 0;
}

#include "ui.h"
#include <stdlib.h>
#include <curses.h>
#include <ncurses.h>


void ui_init(void) {
    initscr();
    noecho();
    curs_set(0);
    start_color();
    use_default_colors();
    keypad(stdscr, true);
    init_pair(DEFAULT_COLOR, -1, -1);
    init_pair(PLAYING_COLOR, COLOR_GREEN, -1);
    init_pair(SELECTION_COLOR, COLOR_BLACK, COLOR_WHITE);
}

void ui_close(void) { endwin(); }



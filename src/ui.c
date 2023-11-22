#ifndef UI_H
#define UI_H
#include "ui.h"
#include <stdlib.h>
#include <curses.h>
#include <ncurses.h>

#define DEFAULT_COLOR 1
#define PLAYING_COLOR 2
#define SELECTION_COLOR 3


enum line_pos { BOT, TOP };

static inline int grid_p1_to_pos(int x, int y) {
    if (y == 0) {
        return 6 + 5 - x;
    } else {
        return x;
    }
}

static inline int grid_to_p2_pos(int x, int y) {
    if (y == 1) {
        return 5 - x;
    } else {
        return 6 + x;
    }
}

void ui_init(void) {
    initscr();
    noecho();
    curs_set(0);
    start_color();
    use_default_colors();
    init_pair(DEFAULT_COLOR, -1, -1);
    init_pair(PLAYING_COLOR, COLOR_GREEN, -1);
    init_pair(SELECTION_COLOR, COLOR_BLACK, COLOR_WHITE);
}

void ui_close(void) { endwin(); }

static void draw_player_line(const char* name, short color, uint32_t score,
                             enum line_pos pos) {
    attron(COLOR_PAIR(color));
    WINDOW* line;
    switch (pos) {
    case TOP:
        line = subwin(stdscr, 3, COLS, 0, 0);
        break;
    case BOT:
        line = subwin(stdscr, 3, COLS, LINES - 3, 0);
        break;
    }

    box(line, ACS_VLINE, ACS_HLINE);
    mvwprintw(line, 1, 1, "%10s : %2d", name, score);
    attroff(COLOR_PAIR(color));
}

static void draw_cell(WINDOW *board, bool selected, int score, int x, int y, int offsetx, int offsety) {
    WINDOW* cell = subwin(board, 3, 7, y * 2 + offsety, x * 6 + offsetx);
    wborder(cell, '|', '|', '-', '-', '+', '+', '+', '+');
    if (selected) {
        wattron(cell, COLOR_PAIR(SELECTION_COLOR));
        wattron(cell, A_BOLD);
    }
    mvwprintw(cell, 1, 1, " %2d  ", score);
    if (selected) {
        wattroff(cell, COLOR_PAIR(SELECTION_COLOR));
        wattroff(cell, A_BOLD);
    }
}


void redraw(const struct board* board, enum player player, const struct ui_state* state) {

    draw_player_line("Opponent",
                     board->to_play != player ? PLAYING_COLOR : DEFAULT_COLOR,
                     board->points[OPPONENT(player)], TOP);
    draw_player_line("You",
                     board->to_play == player ? PLAYING_COLOR : DEFAULT_COLOR,
                     board->points[player], BOT);

    WINDOW* middle = subwin(stdscr, LINES - 6, COLS, 3, 0);
    box(middle, ACS_VLINE, ACS_HLINE);
    for (int x = 0; x < 6 ; x++) {
        for (int y = 0; y < 2; y++) {
            int pos = player == PLAYER1 ? grid_p1_to_pos(x, y) : grid_to_p2_pos(x, y);
            bool selected = (y == 1 && x == state->selected) ;
            draw_cell(middle, selected, board->holes[pos], x, y, 5, 5);
        }
    }
}
#endif

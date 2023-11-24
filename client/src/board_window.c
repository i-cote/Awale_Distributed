#include "board_window.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>

struct ui_window board_window = {
    .open = board_window_open,
    .close = board_window_close,
    .update = board_window_update,
    .on_key_press = board_window_on_key_press,
};

static WINDOW* top_line;
static WINDOW* bot_line;
static WINDOW* middle_window;
static int selected_hole = 0;

void board_window_open() {
    top_line = newwin(3, COLS, 0, 0);
    bot_line = newwin(3, COLS, LINES - 3, 0);
    middle_window = newwin(LINES - 6, COLS, 3, 0);
}

void board_window_close() {
    delwin(top_line);
    delwin(bot_line);
    delwin(middle_window);
}

struct ui_window* board_window_on_key_press(int key) {
    switch(key) {
    case KEY_RESIZE:
        wclear(top_line);
        wresize(top_line, 3, COLS);

        wclear(bot_line);
        wresize(bot_line, 3, COLS);
        mvwin(bot_line, LINES - 3, 0);

        wclear(middle_window);
        wresize(middle_window, LINES - 6, COLS);
        mvwin(middle_window, 3, 0);
        break;
    case 'q':
        return NULL;
    case 'l':
        selected_hole++;
        if (selected_hole > 5) {
            selected_hole = 0;
        }
        break;
    case 'h':
        selected_hole--;
        if (selected_hole < 0) {
            selected_hole = 5;
        }
        break;
    }

    return &board_window;
}

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

static void draw_player_line(WINDOW* line, const char* name, short color, uint32_t score) {
    wattron(line, COLOR_PAIR(color));
    box(line, ACS_VLINE, ACS_HLINE);
    mvwprintw(line, 0, 1, "%s", name);
    mvwprintw(line, 1, 1, "%2d", score);
    wattroff(line, COLOR_PAIR(color));
    wrefresh(line);
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

void board_window_update(struct client_state* state) {
    draw_player_line(top_line, "Opponent",
                     state->board.to_play != state->current_player ? PLAYING_COLOR : DEFAULT_COLOR,
                     state->board.points[OPPONENT(state->current_player)]);
    draw_player_line(bot_line, "You",
                     state->board.to_play == state->current_player ? PLAYING_COLOR : DEFAULT_COLOR,
                     state->board.points[state->current_player]);

    box(middle_window, ACS_VLINE, ACS_HLINE);
    for (int x = 0; x < 6 ; x++) {
        for (int y = 0; y < 2; y++) {
            int pos = state->current_player == PLAYER1 ? grid_p1_to_pos(x, y) : grid_to_p2_pos(x, y);
            bool selected = (y == 1 && x == selected_hole) ;
            draw_cell(middle_window, selected, state->board.holes[pos], x, y, 5, 5);
        }
    }
    wrefresh(middle_window);
}


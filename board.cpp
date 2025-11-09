//
// Created by Kacper Marciniak on 11/7/25.
//

#include "board.h"

WINDOW *game_window = nullptr;
WINDOW *status_window = nullptr;

 void init_board(board_t *board) { // initialize everything for the board and status bar
     //------------ curses stuff -----------
     initscr(); // start screen curses
     cbreak(); // stop the buffer input is real-time
     noecho(); // do not show current input
     curs_set(0); // disable cursor display
     nodelay(stdscr, TRUE); // non blocking input
     keypad(stdscr, TRUE); //enable arrows
     // ------------------------------------

     const int game_height = LINES - STATUS_LINE_SIZE;
     const int game_width = COLS;
     const int status_height = STATUS_LINE_SIZE;
     const int status_width = COLS;

     game_window = newwin(game_height, game_width, 0, 0);
     status_window = newwin(status_height, status_width, 0, 0);
 }

void display_board(board_t *board) { // draw the box "the map"
  box(game_window, 0, 0);
 }

void draw_status(const player_t *player, WINDOW *window) { // draw the box around and default status bar
 box(status_window, 0, 0);
}

void update_status(const player_t *player, WINDOW *window) { // update health and score

 }

void draw_swallow (const player_t *player, WINDOW *window, int current_frame ) {



 }


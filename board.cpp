//
// Created by Kacper Marciniak on 11/7/25.
//

#include "board.h"

#define AMM_OF_ANIM 2 // number of frames for bird animations

WINDOW *game_window = nullptr;
WINDOW *status_window = nullptr;
WINDOW *game_area_window = nullptr;

 void init_board(board_t *board) { // initialize everything for the board and status bar
     //------------ CURSES STUFF -----------
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
     game_area_window = derwin(game_window, game_height - 2, game_width - 2, 1, 1);
     status_window = newwin(status_height, status_width, game_height, 0);

     box(game_window, '|', '-');
     box(status_window, 0, 0);

     wrefresh(game_window);
     wrefresh(status_window);
 }

void update_status(const player_t *player, WINDOW *window) { // update health and score
  // werase(status_window);
  mvwprintw(status_window, 1, 2, "MAX SPEED: %d", player->max_speed);
  mvwprintw(status_window, 1, 20, "CURRENT SPEED: %d", player->current_speed);
  mvwprintw(status_window, 1, 40, "CURRENT HEADING: %d", player->current_heading);
  mvwprintw(status_window, 1, 60, "CURRENT HEALTH: %d", player->health);
  // std::cout << "MAX SPEED: " << player->max_speed << std::endl;
  // std:: cout << "CURRENT SPEED: " << player->current_speed << std::endl;
  // std:: cout << "CURRENT HEADING: " << player->current_heading << std::endl;
  wrefresh(status_window);

 }


void update_player(const player_t *player, WINDOW *window, const int current_frame) {

}


void update_screen(const player_t *player) {
  static int current_frame = 0;

  if (current_frame == AMM_OF_ANIM) {
   current_frame = 0;
  }

  update_status(player, status_window);
  update_player(player, game_area_window, current_frame);

  current_frame++;
 }


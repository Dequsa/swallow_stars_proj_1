//
// Created by Kacper Marciniak on 11/7/25.
//

#include "board.h"

WINDOW *game_window = nullptr;
WINDOW *status_window = nullptr;
WINDOW *game_over_window = nullptr;

void init_board(board_t *board) { // initialize everything for the board and status bar
     //------------ CURSES STUFF -----------
     initscr(); // start screen curses
     start_color(); // sets colors on on the termianl
     has_colors(); // checks if terminal has colors
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
     //game_area_window = derwin(game_window, game_height - 2, game_width - 2, 1, 1);
     status_window = newwin(status_height, status_width, game_height, 0);



}

void update_status(const player_t *player, WINDOW *window, int stars_count, const int time_left) { // update health and score
  werase(status_window);
  box(status_window, 0, 0);

  const int time_left_seen = time_left / 60;

  const float text_max_pos = COLS - 40;

  //mvwprintw(status_window, 1, 2, "MAX SPEED: %d", player->max_speed);
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.20f), "CURRENT SPEED: %d", player->current_speed);
  //mvwprintw(status_window, 1, 40, "CURRENT HEADING: d", player->current_heading);
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.45f), "CURRENT HEALTH: %d", player->health);
  //mvwprintw(status_window, 1, 80, "CURRENT X: %f", player->coordinates.x);
  //mvwprintw(status_window, 1, 100, "CURRENT Y: %f", player->coordinates.y);
  //mvwprintw(status_window, 1, 120, "Lines: %d COLS: %d",game_height, game_width);
  //mvwprintw(status_window, 1, 140, "STARS COUNT: %d", stars_count);
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.70f), "STARS COLLECTED: %d", player->stars_collected);
  //mvwprintw(status_window, 1, (int)(text_max_pos * 0.95f), "Timeleft: %d sec.", time_left_seen);
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  wattron(status_window, COLOR_PAIR(1));
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.95f), "Timeleft: %d sec.", time_left_seen);
  wattroff(status_window, COLOR_PAIR(1));
 if (time_left_seen > 20 && time_left_seen <= 60) {
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  wattron(status_window, COLOR_PAIR(1));
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.95f), "Timeleft: %d sec.", time_left_seen);
  wattroff(status_window, COLOR_PAIR(1));
 } else if (time_left_seen >= 0) {
  init_pair(1, COLOR_RED, COLOR_BLACK);
  wattron(status_window, COLOR_PAIR(1));
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.95f), "Timeleft: %d sec.", time_left_seen);
  wattroff(status_window, COLOR_PAIR(1));

 }


  wrefresh(status_window);
 }


void update_player(const player_t *player, WINDOW *window, const int current_frame) {

  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

  if (current_frame >= 0 && current_frame < FPS/15) {
   wattron(game_window, COLOR_PAIR(2));
   mvwprintw(game_window,(int)player->coordinates.y, (int)player->coordinates.x,"%s", player->frame_one ); // conversion from float to int

  } else if (current_frame >= FPS/15) {
   wattron(game_window, COLOR_PAIR(2));
   mvwprintw(game_window,(int)player->coordinates.y, (int)player->coordinates.x,"%s", player->frame_two ); // same thing here just so i dont have to write more vars
  }
  wattroff(game_window, COLOR_PAIR(2));
}


void update_star(const star_t *star) {
   if (star->is_active) {
    mvwprintw(game_window, (int)star->position.y, (int)star->position.x, "%c", star->sprite);
   }
}

void update_hunter(const hunter_t *hunter) {
 if (hunter->is_active) {
  mvwprintw(game_window, (int)hunter->hunter_pos.y, (int)hunter->hunter_pos.x, "%c", 'H');
 }
}

void update_screen(const player_t *player, const star_t *stars, const hunter_t *hunter, int stars_count, const int time_left) {
  werase(game_window);
  box(game_window, 0, 0);

  static int current_frame = 0;

  if (current_frame == FPS/6) {
   current_frame = 0;
  }

  update_player(player, game_window, current_frame);

  for (int i = 0; i < MAX_AMM_STARS; i++) {

   update_star(&stars[i]);

  }

 for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

  update_hunter(&hunter[i]);

 }

  update_status(player, status_window, stars_count, time_left);

  wrefresh(game_window);

  current_frame++;
 }


void game_over() {
 werase(game_window);
 werase(status_window);
 mvwprintw(game_window, COLS/2, LINES/2, "GAME OVER\n PRESS ANY KEY TO CONTINUE...");
 getch();
}
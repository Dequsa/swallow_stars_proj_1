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

     board->is_over = FALSE;

}

void update_time(const int time_left, const float text_max_pos) {

 const int time_left_seen = time_left / 60;

 if (time_left_seen > 60) {
  init_pair(1, COLOR_GREEN, COLOR_BLACK);

  wattron(status_window, COLOR_PAIR(1));
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.95f), "Timeleft: %d sec.", time_left_seen);
  wattroff(status_window, COLOR_PAIR(1));
 }

 if (time_left_seen > 20 && time_left_seen <= 60) {

  init_pair(1, COLOR_YELLOW, COLOR_BLACK);

  wattron(status_window, COLOR_PAIR(1));
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.95f), "Timeleft: %d sec.", time_left_seen);
  wattroff(status_window, COLOR_PAIR(1));

 } else if (time_left_seen >= 0 && time_left_seen <= 20) {

  init_pair(1, COLOR_RED, COLOR_BLACK);

  wattron(status_window, COLOR_PAIR(1));
  mvwprintw(status_window, 1, (int)(text_max_pos * 0.95f), "Timeleft: %d sec.", time_left_seen);
  wattroff(status_window, COLOR_PAIR(1));

 }
 wrefresh(status_window);
}


void update_status(const player_t *player, WINDOW *window ,const char *name, const int time_left, const int current_lvl) {
 // update health and score

 werase(status_window);
 box(status_window, 0, 0);

 const float text_max_pos = COLS - 40.0f;

 mvwprintw(status_window, 1, (int)(text_max_pos * 0.05f), "LEVEL: %d", current_lvl);
 mvwprintw(status_window, 1, (int)(text_max_pos * 0.10f), "NAME: %s", name);
 mvwprintw(status_window, 1, (int)(text_max_pos * 0.20f), "CURRENT SPEED: %d", player->current_speed);
 mvwprintw(status_window, 1, (int)(text_max_pos * 0.45f), "CURRENT HEALTH: %d", player->health);
 mvwprintw(status_window, 1, (int)(text_max_pos * 0.70f), "STARS COLLECTED: %d", player->stars_collected);

 update_time(time_left, text_max_pos);

}


void update_player(const player_t *player, WINDOW *window, const int current_frame) {

  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

  if (current_frame >= 0 && current_frame < FPS/15) {
   wattron(game_window, COLOR_PAIR(2));
   mvwprintw(game_window,(int)player->coordinates.y, (int)player->coordinates.x,"%s", player->frame_one ); // conversion from float to int

  } else if (current_frame >= FPS/15) {
   wattron(game_window, COLOR_PAIR(2));
   mvwprintw(game_window,(int)player->coordinates.y, (int)player->coordinates.x,"%s", player->frame_two ); // same thing here just so I don't have to write more
  }
  wattroff(game_window, COLOR_PAIR(2));
}


void update_star(const star_t *star) {

 init_pair(4, COLOR_YELLOW, COLOR_BLACK);
 init_pair(5, COLOR_RED, COLOR_BLACK);

   if (star->is_active) {

    if (star->position.y > COLS / 5 && star->position.y < COLS / 2) {

     wattron(game_window, COLOR_PAIR(5));

    }


    mvwprintw(game_window, (int)star->position.y, (int)star->position.x, "%c", star->sprite);

    wattroff(game_window, COLOR_PAIR(5));

   }
}


void update_hunter(hunter_t *hunter) {

 if (hunter->is_active) {

  for (int i = 0; i < hunter->height; i++) {

   for (int j = 0; j < hunter->width; j++) {

    const int h_x = (int)hunter->hunter_pos.x + i;
    const int h_y = (int)hunter->hunter_pos.y + j;

    mvwprintw(game_window, h_y, h_x, "%c", 'H');

    if (j == hunter->width / 2 && hunter->displayed == FALSE) {
     hunter->displayed = TRUE;
     const int display_bounces = hunter->bounces_left - hunter->bounces_done;

     mvwprintw(game_window, h_y - hunter->height, h_x, "%d", display_bounces);

    }
   }
  }
  hunter->displayed = FALSE;
 }
}


void update_taxi(taxi_t *taxi, const player_t *player) {

 if (taxi->visible) {

  init_pair(3, COLOR_CYAN, COLOR_BLACK);
  for (int i = 0; i < 3; i ++) {

    wattron(game_window, COLOR_PAIR(3));
    mvwprintw(game_window, (int)taxi->position.y, (int)taxi->position.x + i, "T");

  }
  wattroff(game_window, COLOR_PAIR(3));

 } else {

  if (taxi->cooldown > 0) {
   taxi->cooldown--;
  }

 }

}


void update_screen(const player_t *player, const star_t *stars, hunter_t *hunter,
 const char *name, const int time_left, const int current_lvl, taxi_t *taxi) {

  werase(game_window);
  box(game_window, 0, 0);

  static int current_frame = 0;

  if (current_frame == FPS/6) {
   current_frame = 0;
  }

  update_player(player, game_window, current_frame);

  update_taxi(taxi, player);

  for (int i = 0; i < MAX_AMM_STARS; i++) {

   update_star(&stars[i]);

  }

 for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

  update_hunter(&hunter[i]);

 }

  update_status(player, status_window, name, time_left, current_lvl);

  wrefresh(game_window);

  current_frame++;
 }


void game_over() {

 nocbreak(); // stop the buffer input is real-time
 echo(); // do not show current input
 curs_set(1); // disable cursor display
 nodelay(stdscr, FALSE); // non blocking input
 keypad(stdscr, FALSE); //enable arrows

 werase(game_window);

 mvwprintw(game_window, LINES/2, COLS/2, "GAME OVER...");
 mvwprintw(game_window, LINES/2 + 1, COLS/2, "PRESS ANY BUTTON TO CONTINUE");

 wrefresh(game_window);

 getch(); // Wait indefinitely until a key is pressed


 werase(game_window);
 werase(status_window);

 endwin();
}

void show_lvl_complete(const int current_lvl) {

 werase(game_window);

 mvwprintw(game_window, LINES/2, COLS/2, "%d COMPLETED", current_lvl + 1);
 mvwprintw(game_window, LINES/2 + 1, COLS/2, "PRESS ANY BUTTON TO CONTINUE");

 wrefresh(game_window);

 nodelay(stdscr, FALSE);

 timespec req{};
 timespec rem{};
 req.tv_nsec = 0;
 req.tv_sec = 1; // 3s sleep so player doesn't instantly turn  off-screen

 nanosleep(&req, &rem);

 while (getch() == -1) {

 }
 nodelay(stdscr, TRUE);

}

void get_player_name(char *name) {

 werase(game_window);
 mvwprintw(game_window, LINES/2, COLS/2 - 10, "ENTER NAME: ");
 wrefresh(game_window);

 nodelay(stdscr, FALSE); // Blocking input
 echo(); // Show what user types
 curs_set(1); // Show cursor

 wgetnstr(game_window, name, MAX_PLAYER_NAME_LENGTH);

 noecho(); // Hide input again
 curs_set(0); // Hide cursor
 nodelay(stdscr, TRUE); // Non-blocking input

}
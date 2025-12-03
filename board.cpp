//
// Created by Kacper Marciniak on 11/7/25.
//

#include "board.h"
#include <cstring>
#define SCORE_P "./SCORES/scores.txt"
#define AMM_OF_SCORES_TO_SHOW 10
#define GREEN 1
#define YELLOW 2
#define RED 3
#define MAGENTA 4
#define CYAN 5

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
     curs_set(FALSE); // disable cursor display
     nodelay(stdscr, TRUE); // non blocking input
     keypad(stdscr, TRUE); //enable arrows
     // init colors
     init_pair(1, COLOR_GREEN, COLOR_BLACK);
     init_pair(2, COLOR_YELLOW, COLOR_BLACK);
     init_pair(3, COLOR_RED, COLOR_BLACK);
     init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // player
     init_pair(5, COLOR_CYAN, COLOR_BLACK); // taxi
     // ------------------------------------

     const int game_height = LINES - STATUS_LINE_SIZE;
     const int game_width = COLS;
     const int status_height = STATUS_LINE_SIZE;
     const int status_width = COLS;

     game_window = newwin(game_height, game_width, 0, 0);
     status_window = newwin(status_height, status_width, game_height, 0);

     board->is_over = FALSE;

}

void update_time(const int time_left, const float text_max_pos) {

 const int time_left_seen = time_left / FPS;
 int color = GREEN;

 if (time_left_seen > 60) {

  color = GREEN;

 } else if (time_left_seen > 20 && time_left_seen <= 60) {

  color = YELLOW;

 } else if (time_left_seen >= 0 && time_left_seen <= 20) {

  color = RED;

 }

 wattron(status_window, COLOR_PAIR(color));

 mvwprintw(status_window, 1, (int)(text_max_pos * 0.95f), "Timeleft: %d sec.", time_left_seen);

 wattroff(status_window, COLOR_PAIR(color));

 wrefresh(status_window);
}


void update_status(const player_t *player, WINDOW *window ,const char *name, const int time_left, const int current_lvl) {
 // update health and score

 werase(status_window);
 box(status_window, 0, 0);

 const float text_max_pos = COLS - 40.0f;

 mvwprintw(status_window, 1, (int)(text_max_pos * 0.05f), "LEVEL: %d", player->score);
 mvwprintw(status_window, 1, (int)(text_max_pos * 0.10f), "NAME: %s", name);
 mvwprintw(status_window, 1, (int)(text_max_pos * 0.20f), "CURRENT SPEED: %d", player->current_speed);
 mvwprintw(status_window, 1, (int)(text_max_pos * 0.45f), "CURRENT HEALTH: %d", player->health);
 mvwprintw(status_window, 1, (int)(text_max_pos * 0.70f), "STARS COLLECTED: %d", player->stars_collected);

 update_time(time_left, text_max_pos);

}


void update_player( const player_t *player, WINDOW *window, const int current_frame) {

  int color = MAGENTA;
  const char *sprite_to_draw;

  if (current_frame >= 0 && current_frame < FPS/15) {

    sprite_to_draw = player->frame_one;
  
  } else if (current_frame >= FPS/15) {

    sprite_to_draw = player->frame_two;
  }

  
  wattron(game_window, COLOR_PAIR(color));

  mvwprintw(game_window,(int)player->coordinates.y, (int)player->coordinates.x,"%s", sprite_to_draw ); // print first frame of player sprite at player coordinates

  wattroff(game_window, COLOR_PAIR(color));
}


void update_star(const star_t *star) {



   if (star->is_active) {

    int color;

    if (star->position.y > COLS / 5 && star->position.y < COLS / 2) {

     color = RED;

    } else {

      color = YELLOW;

    }

     wattron(game_window, COLOR_PAIR(color));
     
     mvwprintw(game_window, (int)star->position.y, (int)star->position.x, "%c", star->sprite);

     wattroff(game_window, COLOR_PAIR(color));

   }
}


void update_hunter(hunter_t *hunter) {

 if (hunter->is_active) {

  int color = hunter->color;

  wattron(game_window, COLOR_PAIR(color));

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
  wattroff(game_window, COLOR_PAIR(color));
 }
}


void update_taxi(taxi_t *taxi, const player_t *player) {

 if (taxi->visible) {

  int color = CYAN;
  
  for (int i = 0; i < 3; i ++) {

    wattron(game_window, COLOR_PAIR(color));
    mvwprintw(game_window, (int)taxi->position.y, (int)taxi->position.x + i, "T");

  }
  wattroff(game_window, COLOR_PAIR(color));

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


int store_score(const char* player_name, int score, score_entry_t *top_scores) {

   FILE* fptr = fopen(SCORE_P, "a");
   if (fptr == nullptr) return 1;

   fprintf(fptr, "%s %d\n", player_name, score);
   fclose(fptr);

   fptr = fopen(SCORE_P, "r");

   int n = 0;
   char buffer[256];
   while (fgets(buffer, sizeof(buffer), fptr) != nullptr) {
    n++;
   }

   rewind(fptr);

   auto *all_scores = new player_data_t[n];
   int score_count = 0;

   while (score_count < n && fscanf(fptr, "%s %d", all_scores[score_count].player_name, &all_scores[score_count].score) == 2) {
    score_count++;
   }

   fclose(fptr);

   // bubble sort
   for (int i = 0; i < score_count - 1; i++) {
    for (int j = i + 1; j < score_count; j++) {

     if (all_scores[j].score > all_scores[i].score) {

      player_data_t temp = all_scores[i];

      all_scores[i] = all_scores[j];
      all_scores[j] = temp;

     }
    }
   }

   for (int i = 0; i < AMM_OF_SCORES_TO_SHOW && i < score_count; i++) {

    strcpy(top_scores[i].player_name, all_scores[i].player_name);

    top_scores[i].score = all_scores[i].score;

   }

   delete[] all_scores;
   return 0;
}


void display_scoreboard(score_entry_t *top_scores) {

    mvwprintw(game_window, 3, COLS / 2 - 10, "SCOREBOARD");

    for (int i = 0; i < AMM_OF_SCORES_TO_SHOW; i++) {

        if (top_scores[i].player_name[0] == '\0' && top_scores[i].score == 0) {
            break;
        }

        mvwprintw(game_window, 5 + i, COLS / 2 - 15, "%d. %s - %d", i + 1, top_scores[i].player_name, top_scores[i].score);

    }
}


void game_over(char* player_name, const int score) {

    nocbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, FALSE);
    keypad(stdscr, TRUE);

    werase(game_window);
    box(game_window, 0, 0);

    mvwprintw(game_window, LINES/2 - 5, COLS/2 - 5, "GAME OVER");

    score_entry_t top_scores[AMM_OF_SCORES_TO_SHOW] = {};

    
    if (store_score(player_name, score, top_scores)) {

     endwin();

     mvwprintw(game_window, LINES/2, COLS/2, "error while storing the score");

    }


    display_scoreboard(top_scores);

    mvwprintw(game_window, LINES + 2, COLS/2 + 10, "Press any key to exit...");

    wrefresh(game_window);

    getch(); // wait

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

 nodelay(stdscr, FALSE); // blocking input
 echo(); // show what user types
 curs_set(1); // show cursor

 wgetnstr(game_window, name, MAX_PLAYER_NAME_LENGTH);

 noecho(); // hide input again
 curs_set(0); // hide cursor
 nodelay(stdscr, TRUE); // non-blocking input

}
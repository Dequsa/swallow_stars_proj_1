//
// Created by Kacper Marciniak on 11/7/25.
//

#ifndef SWALLOW_GAME_PROJ_1_BOARD_H
#define SWALLOW_GAME_PROJ_1_BOARD_H

#include <curses.h>
#include <cstdlib>
#include <ctime>
#include "player.h"
#include "stars.h"
#include "hunters.h"

#define STATUS_LINE_SIZE 3 // size of status bar in lines
#define FPS 60 // number of frames for bird animations
#define MAX_PLAYER_NAME_LENGTH 256

#endif //SWALLOW_GAME_PROJ_1_BOARD_H


extern WINDOW *game_window;
extern WINDOW *status_window;
extern WINDOW *game_over_window;

typedef struct {
    unsigned long current_seed;
    bool is_over;
    unsigned int star_quota;
    int max_hunters;
    int current_amm_of_hunters_on_board;
    int eva_time_interval;
    int eva_lvl;
    int time_left;
} board_t;


void init_board(board_t *board);

//void draw_hunter();

// void update_status(const player_t *player, WINDOW *window, int stars_count, const int time_left);
//
// void update_star(const star_t *star);
//
// void update_player(const player_t *player, WINDOW *window, int current_frame);

void update_screen(const player_t *player, const star_t *stars, hunter_t *hunter, const char *name, const int time_left, const int current_lvl);

void game_over();

void show_lvl_complete(const int current_lvl);

void get_player_name(char *name);
//
// Created by Kacper Marciniak on 11/7/25.
//

#ifndef SWALLOW_GAME_PROJ_1_BOARD_H
#define SWALLOW_GAME_PROJ_1_BOARD_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include "curses.h"
#include "player.h"
#include "stars.h"
#include "hunters.h"
#include "taxi.h"
#include "boss.h"

#define STATUS_LINE_SIZE 3 // size of status bar in lines
#define FPS 60 // number of frames for bird animations
#define MAX_PLAYER_NAME_LENGTH 256
#define GREEN 1
#define YELLOW 2
#define RED 3
#define MAGENTA 4
#define CYAN 5
#define BLUE 6
#define WHITE 7

#endif //SWALLOW_GAME_PROJ_1_BOARD_H


extern WINDOW *game_window;
extern WINDOW *status_window;
extern WINDOW *game_over_window;

typedef struct {
    unsigned long current_seed;
    int is_over;
    unsigned int star_quota;
    int max_hunters;
    int current_amm_of_hunters_on_board;
    int eva_time_interval;
    int eva_lvl;
    int time_left; // Deprecated: kept for backwards compatibility, now calculated from delta_time
    double level_start_time; // Real-world time when level started (in seconds)
    double time_limit_seconds; // How many seconds the level should last
} board_t;

typedef struct {
    int place;
    char player_name[MAX_PLAYER_NAME_LENGTH];
    int score;
} player_data_t;

typedef struct {
    char player_name[MAX_PLAYER_NAME_LENGTH];
    int score;
} score_entry_t;

void init_board(board_t *board);

void update_screen(const player_t *player, const star_t *stars, hunter_t *hunter, const char *name, const int time_left, const int current_lvl, taxi_t *taxi);

void game_over(char* player_name, const int score);

void show_lvl_complete(const int current_lvl);

void get_player_name(char *name);

void show_win_screen();
//
// Created by Kacper Marciniak on 11/7/25.
//

#ifndef SWALLOW_GAME_PROJ_1_BOARD_H
#define SWALLOW_GAME_PROJ_1_BOARD_H

#include "player.h"
#include <curses.h>
#include <cstdlib>
#include <ctime>
#include "stars.h"
#include "hunters.h"

#define STATUS_LINE_SIZE 3 // size of status bar in lines
#define FPS 60 // number of frames for bird animations

#endif //SWALLOW_GAME_PROJ_1_BOARD_H


extern WINDOW *game_window;
extern WINDOW *status_window;
extern WINDOW *game_over_window;

typedef struct {
    unsigned long current_seed;
    bool is_over;
    //int size_x;
    //int size_y;
    unsigned int star_quota;
} board_t;

typedef struct {


} status_line_t;

void init_board(board_t *board);

//void draw_hunter();

void update_status(const player_t *player, WINDOW *window, int stars_count, const int time_left);

void update_star(const star_t *star);

void update_player(const player_t *player, WINDOW *window, int current_frame);

void update_screen(const player_t *player, const star_t *stars,  const hunter_t *hunter, int stars_count, const int time_left);

void game_over();

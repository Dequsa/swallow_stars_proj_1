//
// Created by Kacper Marciniak on 11/7/25.
//

#ifndef SWALLOW_GAME_PROJ_1_BOARD_H
#define SWALLOW_GAME_PROJ_1_BOARD_H

#endif //SWALLOW_GAME_PROJ_1_BOARD_H

#include "player.h"
#include <curses.h>
#define STATUS_LINE_SIZE 3 // size of status bar in lines

extern WINDOW *game_window;
extern WINDOW *status_window;

typedef struct {
    bool is_over;
    int size_x;
    int size_y;
} board_t;

typedef struct {


} status_line_t;

void init_board(board_t *board);

void display_board(board_t *board);

void draw_swallow(const player_t *player, WINDOW *window, int current_frame ); // current_frame for animating swallow

//void draw_hunter();

void draw_status(const player_t *player, WINDOW *window);

void update_status(const player_t *player, WINDOW *window);

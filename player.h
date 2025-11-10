//
// Created by Kacper Marciniak on 11/7/25.
//

#ifndef SWALLOW_GAME_PROJ_1_PLAYER_H
#define SWALLOW_GAME_PROJ_1_PLAYER_H

#endif //SWALLOW_GAME_PROJ_1_PLAYER_H

#include <curses.h>

//^                      0
//0     0 >      < 0     u              its 2x2 matrix for sprite generating
typedef struct {
    int health; // CONFIG FILE
    int current_heading; // 0 down 1 up 2 left 3 right
    int current_speed;
    int stars_collected;
    char sprite[2][2];
    int max_speed; // CONFIG FIlLE 0 1 2 3 4 5 ...
    int time_survived;
    struct coordinates {
        int x;
        int y;
    } coordinates;
} player_t;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} directions_t;

typedef enum {
    LOW_HEALTH = 1,
    MEDIUM_HEALTH = 2,
    MAX_HEALTH = 3
} swallow_colors_t;

void init_player(player_t *player);

void render_player(player_t *player);

void destroy_player(player_t *player);

void move_player(player_t *player);

void draw_player(player_t *player);

void draw_status(player_t *player);

//
// Created by Kacper Marciniak on 11/14/25.
//

#ifndef SWALLOW_GAME_PROJ_1_STARS_H
#define SWALLOW_GAME_PROJ_1_STARS_H
#define MAX_AMM_STARS 20
#define TRUE 1
#define FALSE 0
#define PLAYER_WIDTH 3
#endif //SWALLOW_GAME_PROJ_1_STARS_H

typedef struct {
    int x;
    float y;
}position_t;

typedef struct {
    float difficulty;
    float velocity;
    char is_active;
    char sprite;
    position_t position;
}star_t;



void stars_init(star_t *star);

void stars_spawn(star_t *star, int *stars_count);

void stars_update(star_t *star, int *stars_count);

void stars_collect(star_t *star, player_t *player, int *stars_count);
//
// Created by Kacper Marciniak on 11/27/25.
//

#ifndef SWALLOW_GAME_PROJ_1_TAXI_H
#define SWALLOW_GAME_PROJ_1_TAXI_H
#endif //SWALLOW_GAME_PROJ_1_TAXI_H


typedef struct {
    int cooldown;
    position_t position;
    position_t spawn;
    velocity_t velocity;
    position_t pick_up;
    position_t drop_off;
}taxi_t;



void taxi_call(taxi_t *taxi, const player_t *player, const hunter_t *hunters);
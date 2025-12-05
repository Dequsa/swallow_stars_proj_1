//
// Created by Kacper Marciniak on 11/27/25.
//

#ifndef SWALLOW_GAME_PROJ_1_TAXI_H
#define SWALLOW_GAME_PROJ_1_TAXI_H



typedef struct {
    int cooldown;
    int is_active;
    int is_roaming;
    int picked;
    int visible;
    int found_drop;
    int dropped;
    position_t position;
    velocity_t velocity;
    position_t pick_up;
    position_t drop_off;
}taxi_t;

int check_position(const float tar_x, const float tar_y, const float source_x, const float source_y, const float r); // used for hunter dash check

void taxi_spawn(taxi_t *taxi);

void taxi_spawn(taxi_t *taxi);

void taxi_update(taxi_t *taxi, player_t *player, const int input_key);

#endif //SWALLOW_GAME_PROJ_1_TAXI_H
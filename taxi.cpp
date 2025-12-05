//
// Created by Kacper Marciniak on 11/27/25.
//

#include "board.h"
#define PLAYABLE_AREA_X (1 + (COLS - 1))
#define PLAYABLE_AREA_Y (LINES - STATUS_LINE_SIZE - 1)
#define BASE_SPEED_TAXI 0.5f
#define PICKUP_RADIUS 1.5f
#define TAXI_WIDTH 3
#define SPAWN_TAXI_X (float)(TAXI_WIDTH)
#define SPAWN_TAXI_Y (float)(TAXI_WIDTH)
#define TAXI_EXIT_KEY 'x'

void calc_vel_taxi(taxi_t *taxi, const float target_pos_x, const float target_pos_y) {

        taxi->pick_up.x = target_pos_x;
        taxi->pick_up.y = target_pos_y;

        const float d_x = taxi->pick_up.x - taxi->position.x; // get delta position x
        const float d_y = taxi->pick_up.y - taxi->position.y; // get delta position y
        const float d = sqrt(d_x * d_x + d_y * d_y); // pythagoras theorem for the length of vector pointing at player
        const float u_x = d_x / d; // vector x of this vector
        const float u_y = d_y / d; // vector y of this vector

        taxi->velocity.x = u_x * BASE_SPEED_TAXI; // vec * by the speed of taxi for slower movement
        taxi->velocity.y = u_y * BASE_SPEED_TAXI;
}


int check_position(const float tar_x, const float tar_y, const float source_x, const float source_y, const float r) {

        const float distance_x = source_x -  tar_x;
        const float distance_y = source_y - tar_y ;
        const float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

        if (distance <= r) {
                return 1;
        }

        return 0;
}


int out_of_bounds_check_taxi(float taxi_x, float taxi_y) {

        const float min_x = 1.0f;
        const float max_x = (float)COLS - TAXI_WIDTH - 1;
        const float min_y = 1.0f;
        const float max_y = (float)PLAYABLE_AREA_Y - 1.0f;

        if (taxi_x < min_x || taxi_x > max_x || taxi_y < min_y || taxi_y > max_y) {
                return 1;
        }

        return 0;

}


void taxi_spawn(taxi_t *taxi) {

        taxi->is_active = FALSE;
        taxi->visible = FALSE;
        taxi->picked = FALSE;
        taxi->cooldown = 0;
        taxi->dropped = FALSE;

        // spawn taxi on the left corer of the screen
        taxi->position.x = (float)TAXI_WIDTH;
        taxi->position.y = (float)TAXI_WIDTH;

}


void taxi_state_pickup(taxi_t *taxi, player_t *player) {

    calc_vel_taxi(taxi, player->coordinates.x, player->coordinates.y);
    taxi->position.x += taxi->velocity.x;
    taxi->position.y += taxi->velocity.y;

    if (check_position(player->coordinates.x, player->coordinates.y, taxi->position.x, taxi->position.y, PICKUP_RADIUS)) {
        taxi->picked = TRUE;
        player->in_taxi = TRUE;

        // look for random drop location
        taxi->drop_off.x = (float)(rand() % PLAYABLE_AREA_X);
        taxi->drop_off.y = (float)(rand() % PLAYABLE_AREA_Y);

        taxi->found_drop = FALSE; // don't look for another
    }
}


void taxi_state_transport(taxi_t *taxi, player_t *player, const int input_key) {

    // calculate velocity only once when picked up
    if (!taxi->found_drop) {
        calc_vel_taxi(taxi, taxi->drop_off.x, taxi->drop_off.y);
        taxi->found_drop = TRUE;
    }

    // taxi--player movement
    taxi->position.x += taxi->velocity.x;
    taxi->position.y += taxi->velocity.y;
    player->coordinates.x = taxi->position.x;
    player->coordinates.y = taxi->position.y;

    int reached_drop_off = check_position(taxi->drop_off.x, taxi->drop_off.y, taxi->position.x, taxi->position.y, PICKUP_RADIUS);

    if (reached_drop_off || input_key == TAXI_EXIT_KEY) {
        // leave player
        player->in_taxi = FALSE;
        player->health = player->max_health; // heal
        taxi->dropped = TRUE;
        taxi->picked = FALSE;
        taxi->cooldown = FPS * 30;

        float exit_x = (taxi->position.x > COLS / 2) ? COLS + 10 : -10;
        calc_vel_taxi(taxi, exit_x, taxi->position.y);
    }
}


void taxi_state_leave(taxi_t *taxi, player_t *player) {

    taxi->position.x += taxi->velocity.x;
    taxi->position.y += taxi->velocity.y;

    if (out_of_bounds_check_taxi(taxi->position.x, taxi->position.y)) { // in case taxi wants to go out

        taxi->is_active = FALSE;
        taxi->visible = FALSE;

        player->in_taxi = FALSE; // leave player
    }
}


void taxi_update(taxi_t *taxi, player_t *player, const int input_key) {

    if (!taxi->is_active) {
        return;
    }

    taxi->visible = TRUE;

    if (taxi->dropped) { // leave the screen

        taxi_state_leave(taxi, player);

    }
    else if (taxi->picked) { // move w. the player

        taxi_state_transport(taxi, player, input_key);

    }
    else { // try to pick up the player

        taxi_state_pickup(taxi, player);

    }
}

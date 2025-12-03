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

        const float d_x = taxi->pick_up.x - taxi->position.x; // get delta x
        const float d_y = taxi->pick_up.y - taxi->position.y; // get delta y
        const float d = sqrt(d_x * d_x + d_y * d_y); // pythagoras theorem for the length of vector pointing at player
        const float u_x = d_x / d; // vector x of this vector
        const float u_y = d_y / d; // vector y of this vector

        const float speed = BASE_SPEED_TAXI;

        taxi->velocity.x = u_x * speed; // vec * by the speed of taxi for slower movement
        taxi->velocity.y = u_y * speed;
}


int check_position(float tar_x, float tar_y, float source_x, float source_y, float r) {

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
        const float max_y = (float)LINES - STATUS_LINE_SIZE - 2.0f;

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


void taxi_update(taxi_t *taxi, player_t *player, const int input_key) {

        if (taxi->is_active) {
                
                taxi->visible = TRUE;

                if (!taxi->dropped) {

                        if (!taxi->picked) {

                                calc_vel_taxi(taxi, player->coordinates.x, player->coordinates.y);
                                taxi->position.x += taxi->velocity.x;
                                taxi->position.y += taxi->velocity.y;

                                int collision_taxi_player = check_position(player->coordinates.x, player->coordinates.y, taxi->position.x, taxi->position.y, PICKUP_RADIUS);

                                if ( collision_taxi_player ) { // reached playe 
                                        
                                        taxi->picked = TRUE;
                                        player->in_taxi = TRUE;

                                        float drop_x = (float)(rand() % PLAYABLE_AREA_X);
                                        float drop_y = (float)(rand() % PLAYABLE_AREA_Y);

                                        taxi->drop_off.x = drop_x;
                                        taxi->drop_off.y = drop_y;

                                }

                        }else {

                                if(!taxi->found_drop) {
                                
                                        calc_vel_taxi(taxi, taxi->drop_off.x, taxi->drop_off.y);
                                        taxi->found_drop = TRUE;

                                }
                                
                                taxi->position.x += taxi->velocity.x;
                                taxi->position.y += taxi->velocity.y;

                                player->coordinates.x = taxi->position.x;
                                player->coordinates.y = taxi->position.y;

                                int reached_dropoff = check_position(taxi->drop_off.x, taxi->drop_off.y, taxi->position.x, taxi->position.y, PICKUP_RADIUS);

                                if (reached_dropoff || input_key == TAXI_EXIT_KEY) { // reached drop off point

                                        player->in_taxi = FALSE;
                                        player->health = player->max_health; // heal player on drop off

                                        taxi->dropped = TRUE;  
                                        taxi->picked = FALSE;
                                        taxi->cooldown = FPS * 30; // reset cooldown 30s

                                        float exit_x = taxi->position.x / 2 ? COLS + 10 : - 10; // if closer to left go left if else go right
                                

                                        float exit_y = taxi->position.y;

                                        calc_vel_taxi(taxi, exit_x, exit_y); // calc exit vector

                                }
                        }
                }

                if (taxi->dropped) {

                        taxi->position.x += taxi->velocity.x;
                        taxi->position.y += taxi->velocity.y;

                        if (out_of_bounds_check_taxi(taxi->position.x, taxi->position.y)) {
                                taxi->is_active = FALSE;
                                taxi->visible = FALSE;
                                player->in_taxi = FALSE;
                        }

                }
        }
}
//
// Created by Kacper Marciniak on 11/27/25.
//

#include "board.h"
#include <cmath>
#define PLAYABLE_AREA_X (1 + (COLS - 1))
#define PLAYABLE_AREA_Y (LINES - STATUS_LINE_SIZE - 1)

void calc_vel_taxi(taxi_t *taxi, const float target_pos_x, const float target_pos_y) {

        taxi->pick_up.x = target_pos_x;
        taxi->pick_up.y = target_pos_y;

        const float d_x = taxi->pick_up.x - taxi->spawn.x; // get delta x
        const float d_y = taxi->pick_up.y - taxi->spawn.y; // get delta y
        const float d = sqrt(d_x * d_x + d_y * d_y); // pythagoras theorem for the length of vector pointing at player
        const float u_x = d_x / d; // vector x of this vector
        const float u_y = d_y / d; // vector y of this vector

        const float speed = 0.8f;

        taxi->velocity.x = u_x * speed; // speed evaluated by time and level
        taxi->velocity.y = u_y * speed;
}


int taxi_check(const taxi_t *taxi) {

        if (taxi->cooldown > 0) {
                return 1;
        }
        return 0;
}


void taxi_init(taxi_t *taxi) {

        taxi->drop_off.x = 0.0f;
        taxi->drop_off.y = 0.0f;
        taxi->pick_up.x = 0.0f;
        taxi->pick_up.y = 0.0f;
        taxi->spawn.x = PLAYABLE_AREA_X / 2.0f;
        taxi->spawn.y = PLAYABLE_AREA_Y / 2.0f;
        taxi->velocity.x = 0.0f;

}


void taxi_spawn(taxi_t *taxi, const player_t *player) {

        float p_x = player->coordinates.x;
        float *taxi_x = &taxi->spawn.x;
        float *taxi_y = &taxi->spawn.y;

        if (p_x < COLS / 2) {   // left side of the screen

                *taxi_x = 1.0f;
                *taxi_y = (float)(LINES / 2);

        }else {         // right side screen spawn

                *taxi_x = (float)(COLS - 1);
                *taxi_y = (float)(LINES / 2);

        }
}


void taxi_search_safe(taxi_t *taxi, const hunter_t *hunters) {

        const int BOX = 5; // Size of the safe zone needed

        // Stop 'BOX' units before the edge so the 5x5 area fits inside the map.
        for (int y = 1; y <= PLAYABLE_AREA_Y - BOX; y++) {
                for (int x = 1; x <= COLS - 1 - BOX; x++) {

                        int is_safe = TRUE;

                        // 2. Check this candidate spot against ALL active hunters
                        for (int k = 0; k < MAX_AMM_HUNTERS; k++) {
                                if (hunters[k].is_active == TRUE) {

                                        // Hunter Bounding Box
                                        int h_left = (int)hunters[k].hunter_pos.x;
                                        int h_right = h_left + hunters[k].width;
                                        int h_top = (int)hunters[k].hunter_pos.y;
                                        int h_bottom = h_top + hunters[k].height;

                                        // Candidate 5x5 Box
                                        int box_left = x;
                                        int box_right = x + BOX;
                                        int box_top = y;
                                        int box_bottom = y + BOX;

                                        // 3. Intersection Check (AABB Collision)
                                        // If the boxes overlap, this spot is NOT safe.
                                        if (box_left < h_right && box_right > h_left &&
                                            box_top < h_bottom && box_bottom > h_top) {

                                                is_safe = FALSE;
                                                break;
                                            }
                                }
                        }

                        if (is_safe == TRUE) {
                                taxi->drop_off.x = (float)x;
                                taxi->drop_off.y = (float)y;
                                return;
                        }
                }
        }
}


void taxi_pickup(taxi_t *taxi, const player_t *player) {

        calc_vel_taxi(taxi, player->coordinates.x, player->coordinates.y);

        float *t_x = &taxi->position.x;
        float *t_y = &taxi->position.y;
        float p_x = player->coordinates.x;
        float p_y = player->coordinates.y;
        timespec req{};
        timespec rem{};
        req.tv_nsec = 500000000;
        req.tv_sec = 0;

        while(*t_x != p_x && *t_y != p_y) {
                *t_x += taxi->velocity.x;
                *t_y += taxi->velocity.y;
                nanosleep(&req, &rem);
        }

        taxi->velocity.x = 0.0f;
        taxi->velocity.y = 0.0f;
}


void taxi_delete(taxi_t *taxi, const player_t *player) {

}

void taxi_call(taxi_t *taxi, const player_t *player, const hunter_t *hunters) {

        if (taxi_check(taxi)) {

                taxi_spawn(taxi, player);

                taxi_search_safe(taxi, hunters);

                taxi_pickup(taxi, player);

                taxi_delete(taxi, player);
        }

}



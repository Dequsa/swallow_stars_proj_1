//
// Created by Kacper Marciniak on 11/18/25.
//
#include "board.h"
#include <cmath>
#define PLAYABLE_AREA_SIZE_Y (LINES - STATUS_LINE_SIZE - 2)
#define PLAYABLE_AREA_SIZE_X (COLS - 2)
#define EVALUATION_BY_LEVEL 1
#define EVALUATION_BY_TIME 1


void hunter_bounce(hunter_t *hunter) {
    const float min_x = hunter->width;
    const float min_y = hunter->height;
    const float max_x = COLS - hunter->width;
    const float max_y = PLAYABLE_AREA_SIZE_Y + 1 - hunter->height;

    if (hunter->hunter_pos.x < min_x || hunter->hunter_pos.x > max_x) {
        hunter->vel.x *= -1;
        hunter->bounces_done++;
    }else if (hunter->hunter_pos.y < min_y || hunter->hunter_pos.y > max_y) {
        hunter->vel.y *= -1;
        hunter->bounces_done++;
    }
}


void hunter_move(hunter_t *hunter, const player_t *player) {


    hunter->hunter_pos.x += hunter->vel.x;
    hunter->hunter_pos.y += hunter->vel.y;
}


void hunter_init(hunter_t *hunter, const type_t *type) {
    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

        hunter[i].hunter_type = rand() % HUNTER_TYPE_AMM;

        const unsigned int h_type = hunter[i].hunter_type;

        hunter[i].bounces_done = 0;
        hunter[i].is_active = FALSE;
        hunter[i].bounces_left = rand() % type[h_type].bounces_max + 1;
        hunter[i].dmg = type[h_type].dmg;
        hunter[i].width = type[h_type].size.width;
        hunter[i].height = type[h_type].size.height;
        hunter[i].spawn_chance = type[h_type].spawn_chance;
        hunter[i].cooldown = 0;

    }
}


void hunter_spawn(hunter_t *hunter, player_t *player) {

        for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

            if (rand() % 100 <= hunter[i].spawn_chance && player->current_amm_of_hunters_on_board < player->max_hunters_on_board
                && hunter[i].is_active == FALSE && hunter[i].cooldown <= 0) {

                const int chance = rand() % 4;

                if (chance == 1) {// TOP
                    hunter[i].hunter_pos.x = (float)(rand() % COLS + 1);
                    hunter[i].hunter_pos.y = hunter[i].height;

                }else if (chance == 0) { // LEFT

                    hunter[i].hunter_pos.x = (float)hunter[i].width;
                    hunter[i].hunter_pos.y = rand() % PLAYABLE_AREA_SIZE_Y;

                }else if (chance == 2) { // DOWN

                    hunter[i].hunter_pos.x = (float)(rand() % COLS - 1);
                    hunter[i].hunter_pos.y = PLAYABLE_AREA_SIZE_Y - hunter[i].height;

                }else if (chance == 3) { // RIGHT

                    hunter[i].hunter_pos.x = (float)(COLS - hunter[i].width);
                    hunter[i].hunter_pos.y = rand() % (PLAYABLE_AREA_SIZE_Y - hunter[i].height);

                }

                // vector calculation
                hunter[i].target_pos.x = player->coordinates.x; // set player position "target" when hunter spawns on map
                hunter[i].target_pos.y = player->coordinates.y;

                const float d_x = hunter[i].target_pos.x - hunter[i].hunter_pos.x; // get delta x
                const float d_y = hunter[i].target_pos.y - hunter[i].hunter_pos.y; // get delta y
                const float d = sqrt(d_x * d_x + d_y * d_y); // pythagoras theorem for the length of vector pointing at player
                const float u_x = d_x / d; // vector x of this pointing vector
                const float u_y = d_y / d; // vector y of this pointing vector

                hunter[i].vel.x = u_x * ((rand() % 4 + 1.0f) / 10.0f) * EVALUATION_BY_LEVEL * EVALUATION_BY_TIME; // speed evaluated by time and level
                hunter[i].vel.y = u_y * ((rand() % 4 + 1.0f) / 10.0f) * EVALUATION_BY_TIME;
                // -----------------


                hunter[i].is_active = TRUE;
                player->current_amm_of_hunters_on_board++;
                return;
        }
    }
}


void hunter_update(hunter_t *hunter, player_t *player) {

    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

        if (hunter[i].is_active == TRUE) {

            hunter_move(&hunter[i], player);
            hunter_bounce(&hunter[i]);

            if (hunter[i].bounces_left == hunter[i].bounces_done) { // despawn after bouncing
                hunter[i].is_active = FALSE;
                hunter[i].bounces_done = 0;
                player->current_amm_of_hunters_on_board--;
            }
            hunter_dmg(hunter, player);
        }
        if (hunter[i].cooldown > 0) {
            hunter[i].cooldown--;
        }
    }
}


void hunter_dmg( hunter_t *hunter, player_t *player) {

    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {
        if (hunter[i].is_active == TRUE) {

            if (player->coordinates.x < hunter[i].hunter_pos.x + (float)hunter[i].width &&
                hunter[i].hunter_pos.x < player->coordinates.x + (float)PLAYER_SPRITE_SIZE) { // check x coords

                if (player->coordinates.y < hunter[i].hunter_pos.y + (float)hunter[i].height &&
                    hunter[i].hunter_pos.y < player->coordinates.y + (float)PLAYER_SPRITE_SIZE) { // check y coords

                        player->health -= hunter[i].dmg;
                        hunter[i].is_active = FALSE;
                        hunter[i].cooldown = 10;
                        player->current_amm_of_hunters_on_board--;
                }
            }
        }
    }
}
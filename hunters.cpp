//
// Created by Kacper Marciniak on 11/18/25.
//
#include "board.h"
#include <cmath>
#define PLAYABLE_AREA_SIZE_Y (LINES - STATUS_LINE_SIZE - 2)
#define PLAYABLE_AREA_SIZE_X (COLS - 2)
#define EVALUATION_BY_LEVEL 1
#define EVALUATION_BY_TIME 1
#define DASH_COOLDOWN_SEC 1
#define DASH_RADIUS (15.0f)


void calculate_vel_vec(hunter_t *hunter, const player_t *player,const int eva_time) {

    hunter->target_pos.x = player->coordinates.x; // set player position "target" when hunter spawns on map
    hunter->target_pos.y = player->coordinates.y;

    const float d_x = hunter->target_pos.x - hunter->hunter_pos.x; // get delta x
    const float d_y = hunter->target_pos.y - hunter->hunter_pos.y; // get delta y
    const float d = sqrt(d_x * d_x + d_y * d_y); // pythagoras theorem for the length of vector pointing at player
    const float u_x = d_x / d; // vector x of this vector
    const float u_y = d_y / d; // vector y of this vector

    const float time_multiplier = 1.0f + (eva_time - 1) * 0.10f;   // +0.1 speed every eva_time tick percentage of a lvl
    const int range = hunter->max_speed - hunter->min_speed;
    const float base_speed = ((hunter->min_speed + (rand() % range)) / 10.0f) * eva_time;
    const float speed = base_speed * time_multiplier;

    hunter->vel.x = u_x * speed; // speed evaluated by time and level
    hunter->vel.y = u_y * speed;

}


void hunter_dash(hunter_t *hunter, const player_t *player, int eva_time) {

    float p_x = player->coordinates.x;
    float p_y = player->coordinates.y;
    const float d_multiplier = 1.5f;

    hunter->target_pos.x = p_x;
    hunter->target_pos.y = p_y;

    calculate_vel_vec(hunter, player, eva_time * d_multiplier);

}


void check_dash(hunter_t *hunter, player_t *player, const int eva_time) {

    float h_x = hunter->hunter_pos.x;
    float h_y = hunter->hunter_pos.y;
    float p_x = player->coordinates.x;
    float p_y = player->coordinates.y;
    float t_x = hunter->target_pos.x;
    float t_y = hunter->target_pos.y;

    int *h_c = &hunter->dash_cooldown;

    // if (*h_c <= 0
    //     &&p_x != t_x
    //     && p_y != t_y
    //     && h_x >= p_x - 20.0f
    //     && h_x <= p_x + 20.0f
    //     && h_y >= p_y - 20.0f
    //     && h_y <= p_y + 20.0f)
    // {
    //         hunter->dash_cooldown = FPS * DASH_COOLDOWN_SEC; // 1 second cooldown
    //         hunter_dash(hunter, player, eva_time);
    // }

    if (check_position(p_x, p_y, h_x, h_y, DASH_RADIUS) && *h_c <= 0) {

        hunter->dash_cooldown = FPS * DASH_COOLDOWN_SEC; // 1 second cooldown
        hunter_dash(hunter, player, eva_time);

    }

    if (*h_c > 0) {

        (*h_c)--;

    }
}


void hunter_bounce(hunter_t *hunter) {
    const float min_x = 1.0f;
    const float min_y = 1.0f;
    const float max_x = PLAYABLE_AREA_SIZE_X - hunter->width;
    const float max_y = PLAYABLE_AREA_SIZE_Y;

    // x bounce
    if (hunter->hunter_pos.x < min_x) {
        hunter->hunter_pos.x = min_x + (min_x - hunter->hunter_pos.x); // reflect inside
        hunter->vel.x *= -1;
        hunter->bounces_done++;
    }

    if (hunter->hunter_pos.x > max_x) {
        hunter->hunter_pos.x = max_x - (hunter->hunter_pos.x - max_x);
        hunter->vel.x *= -1;
        hunter->bounces_done++;
    }

    // y bounce
    if (hunter->hunter_pos.y < min_y) {
        hunter->hunter_pos.y = min_y + (min_y - hunter->hunter_pos.y);
        hunter->vel.y *= -1;
        hunter->bounces_done++;
    }

    if (hunter->hunter_pos.y > max_y) {
        hunter->hunter_pos.y = max_y - (hunter->hunter_pos.y - max_y);
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

        hunter[i].is_active = FALSE;
        hunter[i].dmg = type[h_type].dmg;
        hunter[i].width = type[h_type].size.width;
        hunter[i].height = type[h_type].size.height;
        hunter[i].spawn_chance = type[h_type].spawn_chance;
        hunter[i].min_speed = type[h_type].min_speed;
        hunter[i].max_speed = type[h_type].max_speed;
        hunter[i].cooldown = 0;

    }
}


void hunter_spawn(hunter_t *hunter, player_t *player, const type_t *type, const int eva_time) {

        for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

            if (rand() % 100 <= hunter[i].spawn_chance && player->current_amm_of_hunters_on_board < player->max_hunters_on_board
                && hunter[i].is_active == FALSE && hunter[i].cooldown <= 0) {

                const unsigned int h_type = hunter[i].hunter_type;

                const int base_bounces = 1 + (rand() % type[h_type].bounces_max); //
                hunter[i].bounces_left = base_bounces * eva_time;       // scale by eva_time & level


                hunter[i].bounces_done = 0;
                hunter->dash_cooldown = 0;
                hunter[i].displayed = FALSE;

                const int X_MARGIN = 2;
                const int Y_MARGIN = 2;

                // maximum spawn ranges
                int max_spawn_x = PLAYABLE_AREA_SIZE_X - hunter[i].width - 2 * X_MARGIN;
                int max_spawn_y = PLAYABLE_AREA_SIZE_Y - hunter[i].height - 2 * Y_MARGIN;

                switch (rand() % 4) {
                    case 0: // TOP
                        hunter[i].hunter_pos.x = X_MARGIN + (float)(rand() % max_spawn_x);
                        hunter[i].hunter_pos.y = 1.0f; // top row
                        break;

                    case 1: // LEFT
                        hunter[i].hunter_pos.x = 1.0f; // left edge
                        hunter[i].hunter_pos.y = Y_MARGIN + (float)(rand() % max_spawn_y);
                        break;

                    case 2: // BOTTOM
                        hunter[i].hunter_pos.x = X_MARGIN + (float)(rand() % max_spawn_x);
                        hunter[i].hunter_pos.y = PLAYABLE_AREA_SIZE_Y - hunter[i].height; // bottom
                        break;

                    case 3: // RIGHT
                        hunter[i].hunter_pos.x = PLAYABLE_AREA_SIZE_X - hunter[i].width; // right edge
                        hunter[i].hunter_pos.y = Y_MARGIN + (float)(rand() % max_spawn_y);
                        break;
                }




                // vector calculation
                    calculate_vel_vec(&hunter[i], player, eva_time);
                // -----------------


                hunter[i].is_active = TRUE;
                player->current_amm_of_hunters_on_board++;
                return;
        }
    }
}


void hunter_update(hunter_t *hunter, player_t *player, const int eva_time) {

    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

        if (hunter[i].is_active == TRUE) {

            hunter_move(&hunter[i], player);
            hunter_bounce(&hunter[i]);
            hunter_dmg(hunter, player);

            if (hunter[i].bounces_left == hunter[i].bounces_done) { // despawn after bouncing
                hunter[i].is_active = FALSE;
                hunter[i].bounces_done = 0;
                player->current_amm_of_hunters_on_board--;
            }
        }
        if (hunter[i].cooldown > 0) {
            hunter[i].cooldown--;
        }

        check_dash(&hunter[i], player, eva_time);
    }
}


void hunter_dmg(hunter_t *hunter, player_t *player) {

    if(!player->in_taxi) {
        const float p_x = player->coordinates.x;
        const float p_y = player->coordinates.y;
        const float p_w = PLAYER_SPRITE_SIZE;
        const float p_h = PLAYER_SPRITE_Y_SIZE;

        for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

            if (!hunter[i].is_active) {
                continue;
            }

            const float h_x = hunter[i].hunter_pos.x;
            const float h_y = hunter[i].hunter_pos.y;
            const float h_w = hunter[i].width;
            const float h_h = hunter[i].height;

            if (p_x < h_x + h_w && p_x + p_w > h_x && // collision check
                p_y < h_y + h_h && p_y + p_h > h_y)
            {
                player->health -= hunter[i].dmg;
                hunter[i].is_active = FALSE;
                hunter[i].cooldown = FPS * 5; // 5 seconds cooldown
                player->current_amm_of_hunters_on_board--;
            }
        }
    }

}

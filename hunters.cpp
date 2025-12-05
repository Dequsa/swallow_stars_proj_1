//
// Created by Kacper Marciniak on 11/18/25.
//
#include "board.h"
#include <cmath>
#define PLAYABLE_AREA_SIZE_Y (LINES - STATUS_LINE_SIZE)
#define PLAYABLE_AREA_SIZE_X (COLS - 1)
#define DASH_COOLDOWN_SEC 1
#define STOP_TIME_SEC 1
#define DASH_RADIUS (15.0f)
#define DASH_SPEED_MULTIPLIER (2.5f)
#define SPEED_BOOST (0.3f)
#define SCREEN_RATIO (0.56f)


void calculate_vel_vec(hunter_t *hunter, const player_t *player,const int eva_time) {

    hunter->target_pos.x = player->coordinates.x; // set player position "target" when hunter spawns on map
    hunter->target_pos.y = player->coordinates.y;

    const float d_x = hunter->target_pos.x - hunter->hunter_pos.x; // get delta x
    const float d_y = hunter->target_pos.y - hunter->hunter_pos.y; // get delta y

    const float d = sqrt(d_x * d_x + d_y * d_y); // pythagoras theorem for the length of vector pointing at player

    const float u_x = d_x / d; // vector x of this vector
    const float u_y = d_y / d; // vector y of this vector

    const float time_multiplier = eva_time * SPEED_BOOST;  // boost speed up to eva_time * SPEED_BOOST;
    const int range = hunter->max_speed - hunter->min_speed;
    const float base_speed = ((hunter->min_speed + (rand() % range)) / 10.0f);

    const float speed = base_speed * time_multiplier;

    hunter->vel.x = u_x * speed; // speed evaluated by time
    hunter->vel.y = u_y * speed * SCREEN_RATIO;

}


void hunter_dash(hunter_t *hunter, const player_t *player, int eva_time) {

    float p_x = player->coordinates.x;
    float p_y = player->coordinates.y;
    const float d_multiplier = DASH_SPEED_MULTIPLIER;

    hunter->target_pos.x = p_x; // target current player position
    hunter->target_pos.y = p_y;

    calculate_vel_vec(hunter, player, eva_time * d_multiplier);

}


void check_dash(hunter_t *hunter, player_t *player, const int eva_time) {

    float h_x = hunter->hunter_pos.x;
    float h_y = hunter->hunter_pos.y;
    float p_x = player->coordinates.x;
    float p_y = player->coordinates.y;


    if (hunter->dash_cooldown > 0) {

        hunter->dash_cooldown--;
        return;

    }

    if (hunter->dash_state == STOPPED) { // stop the hunter for a moment

        hunter->vel.x = 0.0f;
        hunter->vel.y = 0.0f;
        hunter->stop_timer--;

        if (hunter->stop_timer <= 0) {
            hunter->dash_state = DASHING;
        }

    } else if (hunter->dash_state == DASHING) { // dash towards player

        hunter->dash_cooldown = FPS * DASH_COOLDOWN_SEC; // set 1 second cooldown so they don't dash again instantly
        hunter->dash_state = NORMAL;

        hunter_dash(hunter, player, eva_time);

    }else if (check_position(p_x, p_y, h_x, h_y, DASH_RADIUS) && hunter->dash_cooldown <= 0) { // if within range of dashing and cooldown is off set hunter state to STOPPED "taxi.h"

        hunter->dash_state = STOPPED;

        hunter->stop_timer = FPS * STOP_TIME_SEC;
    }
}


void hunter_bounce(hunter_t *hunter) {
    const float min_x = 1.0f;
    const float min_y = 1.0f;
    const float max_x = PLAYABLE_AREA_SIZE_X - hunter->width;
    const float max_y = PLAYABLE_AREA_SIZE_Y - hunter->height;

    // x bounce
    if (hunter->hunter_pos.x < min_x) {
        hunter->hunter_pos.x = min_x + (min_x - hunter->hunter_pos.x); // set position back to max - overshoot for safety
        hunter->vel.x *= -1; // invert velocity
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


void hunter_move(hunter_t *hunter) {


    hunter->hunter_pos.x += hunter->vel.x;
    hunter->hunter_pos.y += hunter->vel.y;
}


void hunter_init(hunter_t *hunter, const type_t *type) {

    int total_weight = 0;
    
    for (int i = 0; i < HUNTER_TYPE_AMM; i++) {

        total_weight += type[i].spawn_chance;

    }

    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

        int rand_n = rand() % total_weight;
        
        int current_weight = 0;
        int current_id = 0;
        
        for (int j = 0; j < HUNTER_TYPE_AMM; j++) {

            current_weight += type[j].spawn_chance;
            
            if (rand_n < current_weight) {

                current_id = j;

                break;

            }
        }

        hunter[i].hunter_type = current_id;

        const unsigned int h_type = hunter[i].hunter_type;

        hunter[i].is_active = FALSE;
        hunter[i].dmg = type[h_type].dmg;
        hunter[i].width = type[h_type].size.width;
        hunter[i].height = type[h_type].size.height;
        hunter[i].spawn_chance = type[h_type].spawn_chance;
        hunter[i].min_speed = type[h_type].min_speed;
        hunter[i].max_speed = type[h_type].max_speed;
        hunter[i].color = type[h_type].color;
        hunter[i].cooldown = 0;

    }
}


int hunter_check_spawn(const hunter_t *hunter, player_t *player) {

    if (player->current_amm_of_hunters_on_board < player->max_hunters_on_board && hunter->is_active == FALSE && hunter->cooldown <= 0)
        return 1;

    return 0;
}


void hunter_choose_spawn_point(hunter_t *hunter) {


    // maximum spawn ranges
    int max_spawn_x = PLAYABLE_AREA_SIZE_X - hunter->width;
    int max_spawn_y = PLAYABLE_AREA_SIZE_Y - hunter->height;

    switch (rand() % 4) {

        case 0: // TOP
            hunter->hunter_pos.x = 1.0f + (float)(rand() % max_spawn_x);
            hunter->hunter_pos.y = 1.0f; //
            break;

        case 1: // LEFT
            hunter->hunter_pos.x = 1.0f; //
            hunter->hunter_pos.y = 1.0f + (float)(rand() % max_spawn_y);
            break;

        case 2: // BOTTOM
            hunter->hunter_pos.x = 1.0f + (float)(rand() % max_spawn_x);
            hunter->hunter_pos.y = PLAYABLE_AREA_SIZE_Y - hunter->height; //
            break;

        case 3: // RIGHT
            hunter->hunter_pos.x = PLAYABLE_AREA_SIZE_X - hunter->width; //
            hunter->hunter_pos.y = 1.0f + (float)(rand() % max_spawn_y);
            break;
    }
}


void hunter_spawn(hunter_t *hunter, player_t *player, const type_t *type, const int eva_time) {

        for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

            if (hunter_check_spawn(&hunter[i], player)) {

                const unsigned int h_type = hunter[i].hunter_type;

                const int base_bounces = 1 + (rand() % type[h_type].bounces_max);

                hunter[i].bounces_left = base_bounces * eva_time; // scale by eva_time
                hunter[i].bounces_done = 0;
                hunter[i].dash_state = NORMAL;
                hunter[i].stop_timer = 0;
                hunter[i].dash_cooldown = FPS * 2;

                hunter_choose_spawn_point(&hunter[i]);

                // vector calculation
                calculate_vel_vec(&hunter[i], player, eva_time);
                // -----------------


                hunter[i].is_active = TRUE;
                player->current_amm_of_hunters_on_board++;
                return;
        }
    }
}


void hunter_dmg(hunter_t *hunters, player_t *player, int **map) {
    
    if (player->in_taxi) return; // no damage in taxi

    // int converison for map comparison
    int p_x = (int)player->coordinates.x;
    int p_y = (int)player->coordinates.y;

    for (int i = 0; i < PLAYER_SPRITE_SIZE; i++) {

        int check_x = p_x + i; // player width offset
        int check_y = p_y; // player height offset

        if (check_x >= 0 && check_x < COLS && check_y >= 0 && check_y < LINES) { // bounds check for safety in case of runner hunter

            int cell_content = map[check_y][check_x]; // get content of the cell on occupancy map from board

            if (cell_content >= 0) {

                int hunter_id = cell_content; // hunter index on map is stored as cell content with size of index

                player->health -= hunters[hunter_id].dmg;
                player->current_amm_of_hunters_on_board--;

                hunters[hunter_id].is_active = FALSE;
                hunters[hunter_id].bounces_done = 0;
                hunters[hunter_id].cooldown = FPS * 5; // 5 seconds cooldown

                return;
            }
        }
    }
}


void hunter_update(hunter_t *hunter, player_t *player, const int eva_time, int **map) {

    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

        if (hunter[i].is_active == TRUE) {

            hunter_move(&hunter[i]); // move hunter on map
            hunter_bounce(&hunter[i]); // bounce of the border

            if (hunter[i].bounces_left <= hunter[i].bounces_done) { // despawn after bouncing

                hunter[i].is_active = FALSE;
                hunter[i].bounces_done = 0;
                player->current_amm_of_hunters_on_board--;

            }
        }

        if (hunter[i].cooldown > 0) {
            hunter[i].cooldown--; // spawn cooldown
        }

        check_dash(&hunter[i], player, eva_time);
    }
}

// hitbox check (decaprecated) it's only used in stars.cpp for star collection for simplicity
int check_object_player_collision( const float obj_x, const float obj_y, const float player_x, const float player_y,
    const float obj_offset_x, const float obj_offset_y, const float player_offset_x, const float player_offset_y) { // offset is width or height

    if (player_x < obj_x + obj_offset_x && player_x + player_offset_x > obj_x && player_y < obj_y + obj_offset_y && player_y + player_offset_y > obj_y) {
        return 1;
    }

    return 0;
}

// // void hunter_dmg(hunter_t *hunter, player_t *player) {

// //     if(!player->in_taxi) {


// //         for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

// //             if (!hunter[i].is_active) {
// //                 continue;
// //             }



// //             if (check_object_player_collision(hunter[i].hunter_pos.x, hunter[i].hunter_pos.y,
// //                 player->coordinates.x, player->coordinates.y, hunter[i].width,
// //                 hunter[i].height, PLAYER_SPRITE_SIZE, PLAYER_SPRITE_Y_SIZE))
// //             {
// //                 player->health -= hunter[i].dmg;
// //                 hunter[i].is_active = FALSE;
// //                 hunter[i].cooldown = FPS * 5; // 5 seconds cooldown
// //                 player->current_amm_of_hunters_on_board--;
// //             }
// //         }
// //     }

// // }



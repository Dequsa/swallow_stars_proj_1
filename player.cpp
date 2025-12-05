//
// Created by Kacper Marciniak on 11/7/25.
//
#include <iostream>
#include "board.h"
#define PLAYER_SPRITE_WIDTH_X 5
#define LOW_HEALTH 0.3f
#define MEDIUM_HEALTH 0.7f
#define BASE_TILE_CHANGE 0.3f

void init_player(player_t *player) {

    player->coordinates.x = (float)COLS / 2; // middle of the screen x
    player->coordinates.y = (float)LINES / 2; // middle of the screen y
    player->current_speed = 0;
    player->current_heading = UP;
    player->stars_collected = 0;
    player->current_amm_of_hunters_on_board = 0;
    player->has_called_taxi = FALSE;
    player->in_taxi = FALSE;


    const char temp_sprite_1[] = "\\\\V//\0";
    const char temp_sprite_2[] = "//O\\\\\0";

    int i = 0;
    while (temp_sprite_1[i] != '\0') {
        player->frame_one[i] = temp_sprite_1[i];
        i++;
    }
    player->frame_one[i] = '\0';

    i = 0;
    while (temp_sprite_2[i] != '\0') {
        player->frame_two[i] = temp_sprite_2[i];
        i++;
    }
    player->frame_two[i] = '\0';

}


int choose_color_player(int health, int max_health) {

    const float health_ratio = (float)health / (float)max_health;

    if (health_ratio <= LOW_HEALTH) {
        return RED;
    } else if (health_ratio <= MEDIUM_HEALTH) {
        return YELLOW;
    } else {
        return GREEN;
    }
}


void out_of_bounds_check_player(float *new_x,  float *new_y, int *heading) {

    const float min_x = 1.0f;
    const float max_x = (float)COLS - PLAYER_SPRITE_WIDTH_X - 1;
    const float min_y = 1.0f;
    const float max_y = (float)LINES - STATUS_LINE_SIZE - 2.0f;

    if (*new_x < min_x) {
        *new_x = min_x;
        if (*heading == LEFT) *heading = RIGHT;
    }else if (*new_x > max_x) {
        *new_x = max_x;
        if (*heading == RIGHT) *heading = LEFT;
    }

    if (*new_y < min_y) {
        *new_y = min_y;
        if (*heading == UP) *heading = DOWN;
    }else if (*new_y > max_y) {
        *new_y = max_y;
        if (*heading == DOWN) *heading = UP;
    }
}


void calculate_vel_vec(player_t *player) {
    
    float dir_x = 0.0f;
    float dir_y = 0.0f;

    if (player->current_heading == UP) {
        dir_y = -1.0f; 
    } else if (player->current_heading == DOWN) {
        dir_y = 1.0f;
    } else if (player->current_heading == LEFT) {
        dir_x = -1.0f;
    } else if (player->current_heading == RIGHT) {
        dir_x = 1.0f;
    }

    const float speed = (float)player->current_speed * BASE_TILE_CHANGE;

    player->vel_x = dir_x * speed;
    player->vel_y = dir_y * speed * 0.56f; // adjust for terminal character aspect ratio

}


void player_move(player_t *player) {


    player->coordinates.x += player->vel_x;
    player->coordinates.y += player->vel_y;

}

void update_player_position(player_t *player) {
    
    if (!player->in_taxi) {
    
    calculate_vel_vec(player);
    
    player_move(player);

    out_of_bounds_check_player(&player->coordinates.x, &player->coordinates.y, &player->current_heading);

    }

}


void move_player(player_t *player, int *input_key) {
    int key;
    switch (key = getch()) {
        case 'w': {
            player->current_heading = UP;
            break;
        }
        case 's': {
            player->current_heading = DOWN;
            break;
        }
        case 'a': {
            player->current_heading = LEFT;
            break;
        }
        case 'd': {
            player->current_heading = RIGHT;

            break;
        }
        case 'p': {
            if (player->current_speed < player->max_speed) {
                player->current_speed++;
            }
            break;
        }
        case 'o': {
            if (player->current_speed > player->min_speed) {
                player->current_speed--;
            }
            break;
        }
        //     case 'x': { // debug button

        //     break;
        // }
        default: {
            player->current_heading = player->current_heading;
            break;
        }
    }

    *input_key = key;
    update_player_position(player);

}




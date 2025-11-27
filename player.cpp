//
// Created by Kacper Marciniak on 11/7/25.
//
#include <iostream>
#include "board.h"
#define PLAYER_SPRITE_WIDTH 3

void init_player(player_t *player) {

    player->coordinates.x = (float)COLS/2; // middle of the screen x
    player->coordinates.y = (float)LINES/2; // middle of the screen y
    player->current_speed = 0;
    player->current_heading = UP;
    player->stars_collected = 0;
    player->current_amm_of_hunters_on_board = 0;


    const char temp_sprite_1[] = "\\V/\0";
    const char temp_sprite_2[] = "/V\\\0";

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


void out_of_bounds_check_player(float *new_x,  float *new_y) {

    const float min_x = 1.0f;
    const float max_x = (float)COLS - PLAYER_SPRITE_WIDTH - 1;
    const float min_y = 1.0f;
    const float max_y = (float)LINES - STATUS_LINE_SIZE - 2.0f;

    if (*new_x < min_x) {
        *new_x = min_x;
    }else if (*new_x > max_x) {
        *new_x = max_x;
    }

    if (*new_y < min_y) {
        *new_y = min_y;
    }else if (*new_y > max_y) {
        *new_y = max_y;
    }
}


void move_player(player_t *player) {
    switch (int key = getch()) {
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
            //velocity_player(player);
            break;
        }
        case 'o': {
            if (player->current_speed > player->min_speed) {
                player->current_speed--;
            }
            //velocity_player(player);
            break;
        }
            case 'x': { // debug button

            break;
        }
        default: {
            player->current_heading = player->current_heading;
            break;
        }
    }
    //update_player_movement(player);
    const float move = (float)player->current_speed * 0.3f;
    float new_y = player->coordinates.y;
    float new_x = player->coordinates.x;

    switch (player->current_heading) {
        case UP: {
            new_y -= move * 0.56f;
            break;
        }
        case DOWN: {
            new_y += move * 0.56f;
            break;
        }
        case LEFT: {
            new_x -= move;
            break;
        }
        case RIGHT: {
            new_x += move;
            break;
        }
        default: { break; }
    }


    out_of_bounds_check_player(&new_x, &new_y);

    player->coordinates.x = new_x;
    player->coordinates.y = new_y;

}




//
// Created by Kacper Marciniak on 11/7/25.
//
#include <iostream>
#include "board.h"

#define PLAYER_SPRITE_WIDTH 4

void load_config_player(player_t *player) {
    FILE *fptr = nullptr;
    fptr = fopen("./CONFIGS/stats.cfg", "r");

    if (fptr == nullptr) {
        std::cout << "Failed to open config file..." << std::endl;
    }

    int result = 0;
    char current_line[MAX_LINE_SIZE];

    while (fgets( current_line, MAX_LINE_SIZE, fptr)) {

        char name_of_variable_config[MAX_LINE_SIZE];

        int temp_line = sscanf(current_line, "%d @%s", &result, name_of_variable_config);

        if (strcmp(name_of_variable_config, "MAX_HEALTH") == 0 && temp_line == 2) {

            player->health = result;

        }else if (strcmp(name_of_variable_config, "MAX_SPEED") == 0 && temp_line == 2) {

            player->max_speed = result;

        }
    }

    fclose(fptr);
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


void init_player(player_t *player) {
    const char temp_sprite_1[] = "\\/\\/\0";
    const char temp_sprite_2[] = "/\\/\\\0";

    player->coordinates.x = (float)COLS/2; // middle of the screen x
    player->coordinates.y = (float)LINES/2; // middle of the screen y
    player->current_speed = 0;
    player->current_heading = UP;
    player->stars_collected = 0;

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
            if (player->current_speed > 0) {
                player->current_speed--;
            }
            //velocity_player(player);
            break;
        }
            case 'x': { // debug button

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
            new_y -= move;
            break;
        }
        case DOWN: {
            new_y += move;
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

    // const float min_x = 1.0f;
    // const float max_x = (float)COLS - PLAYER_SPRITE_WIDTH - 1;
    // const float min_y = 1.0f;
    // const float max_y = (float)LINES - STATUS_LINE_SIZE - 2.0f;
    //
    // if (new_x < min_x) {
    //     new_x = min_x;
    // }else if (new_x > max_x) {
    //     new_x = max_x;
    // }
    //
    // if (new_y < min_y) {
    //     new_y = min_y;
    // }else if (new_y > max_y) {
    //     new_y = max_y;
    // }
}




//
// Created by Kacper Marciniak on 11/7/25.
//

// THIS FILE HANDLES ALL THE LOGIC WHEN IT COMES TO PLAYER
// MOVING DMG_RECIVE CURRENT SCORE

//#include "player.h"
#include <iostream>
#include <cstring>
#include <filesystem>

#include "board.h"

// #define MAX_PLAYER_HEALTH 100
#define MAX_CONFIG_SIZE 1000
#define MAX_LINE_SIZE 32

FILE *fptr = nullptr;

void init_player(player_t *player) {

    // ----------- CONFIG READING ---------
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
    // ------------------------------------
    player->current_speed = 0;
    player->current_heading = UP;
    player->stars_collected = 0;
}

void move_player(player_t *player) {
    int key = getch();
    switch (key) {
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
                update_status(player, status_window);
            }

            break;
        }
        case 'o': {
            if (player->current_speed > 1) {
                player->current_speed = player->current_speed - 1;
                update_status(player, status_window);
            }

            break;
        }
            case 'x': { // debug button

        }
        default: {
            player->current_heading = player->current_heading;
            break;
        }
    }
}

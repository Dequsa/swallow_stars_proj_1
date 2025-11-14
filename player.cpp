//
// Created by Kacper Marciniak on 11/7/25.
//

#include "board.h"
#include <iostream>

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

void init_player(player_t *player) {
    const char temp_sprite_1[] = "\\OO/\0";
    const char temp_sprite_2[] = "/OO\\\0";

    player->coordinates.x = COLS/2; // middle of the screen x
    player->coordinates.y = LINES/2; // middle of the screen y
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

//
// void velocity_player(player_t *player) {
//     if (player->current_heading == UP) {
//         player->velocity_y = (float)player->current_speed * 0.5f;
//     }else if (player->current_heading == DOWN) {
//         player->velocity_y = (float)player->current_speed * 0.5f;
//     }
//     if (player->current_heading == LEFT) {
//         player->velocity_x = (float)player->current_speed * 0.5f;
//     }else if (player->current_heading == RIGHT) {
//         player->velocity_x = (float)player->current_speed* 0.5f;
//     }
// }


// void update_player_movement(player_t *player) {
//     player->coordinates_y += player->velocity_x;
//     player->coordinates_x += player->velocity_y;
//
//
//     player->coordinates.x = (int)player->coordinates.x;
//     player->coordinates.y = (int)player->coordinates.y;
// }

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
    if (player->current_heading == UP && (int)player->coordinates.y > 1) {
        player->coordinates.y += -1.0 * player->current_speed * 0.5f;
    }
    if (player->current_heading == DOWN && (int)player->coordinates.y < (float)(LINES - STATUS_LINE_SIZE - 2.2)) {
        player->coordinates.y += 1.0 * player->current_speed * 0.5f;
    }
    if (player->current_heading == LEFT && (int)player->coordinates.x > 1.9) {
        player->coordinates.x += -1.0 * player->current_speed * 0.5f;
    }
    if (player->current_heading == RIGHT  && (int)player->coordinates.x < COLS - 5) {
        player->coordinates.x += 1.0 * player->current_speed * 0.5f;
    }

}

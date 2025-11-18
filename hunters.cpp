//
// Created by Kacper Marciniak on 11/18/25.
//

#include "board.h"
// #include <cstring>{

//     void load_config_player(player_t *player) {
//         FILE *fptr = nullptr;
//         fptr = fopen("./CONFIGS/stats.cfg", "r");
//
//         if (fptr == nullptr) {
//             exit(1);
//         }
//
//         int result = 0;
//         char current_line[MAX_LINE_SIZE];
//
//         while (fgets( current_line, MAX_LINE_SIZE, fptr)) {
//
//             char name_of_variable_config[MAX_LINE_SIZE];
//
//             int temp_line = sscanf(current_line, "%d @%s", &result, name_of_variable_config);
//
//             if (strcmp(name_of_variable_config, "MAX_HEALTH") == 0 && temp_line > 0) {
//
//                 player->health = result;
//
//             }else if (strcmp(name_of_variable_config, "MAX_SPEED") == 0 && temp_line > 0) {
//
//                 player->max_speed = result;
//
//             }else if (strcmp(name_of_variable_config, "MIN_SPEED") == 0 && temp_line > 0) {
//
//                 player->min_speed = result;
//
//             }
//         }
//
//         fclose(fptr);
//     }
// };

void hunter_init(hunter_t *hunter) {
    for (int i = 0; i < 4; i++) {

    }
    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {
        hunter[i].hunter_type = rand() % 4;
        hunter[i].bounces_done = 0;
        hunter[i].is_dashing = FALSE;
        hunter[i].is_on_course = FALSE;
        hunter[i].is_active = FALSE;
        switch (hunter[i].hunter_type) {
            case 0: {
                hunter[i].hunter_dmg = 5;
                hunter[i].bounces_left = rand() % 3;
                hunter[i].width = 1;
                hunter[i].height = 2;
                break;
            }
            case 1: {
                hunter[i].hunter_dmg = 10;
                hunter[i].bounces_left = rand() % 4;
                hunter[i].width = 2;
                hunter[i].height = 1;
                break;
            }
            case 2: {
                hunter[i].hunter_dmg = 15;
                hunter[i].bounces_left = rand() % 6;
                hunter[i].width = 1;
                hunter[i].height = 3;
                break;
            }
            case 3: {
                hunter[i].hunter_dmg = 20;
                hunter[i].bounces_left = rand() % 8;
                hunter[i].width = 3;
                hunter[i].height = 1;
                break;
            }
                case 4: {
                hunter[i].hunter_dmg = 25;
                hunter[i].bounces_left = rand() % 9;
                hunter[i].width = 2;
                hunter[i].height = 2;
                break;;
            }

            default: { break;}
        }
    }
}


void hunter_spawn(hunter_t *hunter) {
    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

        if (hunter[i].bounces_left == FALSE && rand() % 10 <= 3) {
            hunter[i].bounces_left = rand() % 4;
            hunter[i].hunter_dmg = 0;
            hunter[i].is_dashing = FALSE;
            hunter[i].is_on_course = FALSE;
            return;
        }

    }
}


void hunter_update(hunter_t *hunter);


void hunter_dmg(hunter_t *hunter, player_t *player);
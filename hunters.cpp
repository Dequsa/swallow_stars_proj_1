//
// Created by Kacper Marciniak on 11/18/25.
//

#include "board.h"
#define PLAYABLE_AREA_SIZE_Y LINES - STATUS_LINE_SIZE - 2

void hunter_move(hunter_t *hunter, player_t *player) {
    int current_player_pos_x = (int)player->coordinates.x;
    int current_player_pos_y = (int)player->coordinates.y;


}

void hunter_init(hunter_t *hunter) {
    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {
        hunter[i].hunter_type = rand() % 5;
        hunter[i].bounces_done = 0;
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
                break;
            }

            default: { break;}
        }
    }
}


void hunter_spawn(hunter_t *hunter) {
    if (rand() % 100 <= 10) {
        for (int i = 0; i < MAX_AMM_HUNTERS; i++) {
            if (hunter[i].is_active == FALSE) {
                if (rand()%4 == 1) {
                    hunter[i].is_active = TRUE;
                    hunter[i].hunter_pos.x = rand() % COLS;
                    hunter[i].hunter_pos.y = hunter->height;
                }else if (rand()%4 == 0) {
                    hunter[i].is_active = TRUE;
                    hunter[i].hunter_pos.x = hunter->width;
                    hunter[i].hunter_pos.y = rand() % LINES;
                }else if (rand()%4 == 2) {
                    hunter[i].is_active = TRUE;
                    hunter[i].hunter_pos.x = rand() % COLS;
                    hunter[i].hunter_pos.y = PLAYABLE_AREA_SIZE_Y - hunter->height;
                }else if (rand()%4 == 3) {
                    hunter[i].is_active = TRUE;
                    hunter[i].hunter_pos.x = COLS - hunter->width;
                    hunter[i].hunter_pos.y = rand() % LINES - hunter->height;
                }
                return;
            }
        }
    }
}


void hunter_update(hunter_t *hunter, player_t *player) {
    for (int i = 0; i < MAX_AMM_STARS; i++) {
        if (hunter[i].is_active == TRUE) {
            hunter_move(hunter,player);
        }
    }
}


void hunter_dmg(hunter_t *hunter, player_t *player);
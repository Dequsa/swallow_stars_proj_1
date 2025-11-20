//
// Created by Kacper Marciniak on 11/18/25.
//
#include "board.h"
#define PLAYABLE_AREA_SIZE_Y (LINES - STATUS_LINE_SIZE - 2)
#define RIGHT_MOVE_HUNTER (hunter->hunter_pos.x += 0.1f)
#define LEFT_MOVE_HUNTER (hunter->hunter_pos.x -= 0.1f)
#define UP_MOVE_HUNTER (hunter->hunter_pos.y -= 0.05)
#define DOWN_MOVE_HUNTER (hunter->hunter_pos.y += 0.05f)


void hunter_move(hunter_t *hunter, const player_t *player) {

        const float player_x = hunter->target_pos.x;
        const float player_y = hunter->target_pos.y;
        const float hunter_x = hunter->hunter_pos.x;
        const float hunter_y = hunter->hunter_pos.y;

        if (player_x > hunter_x && player_y > hunter_y) {

            RIGHT_MOVE_HUNTER;
            DOWN_MOVE_HUNTER;

        }else if (player_x < hunter_x && player_y > hunter_y) {

            LEFT_MOVE_HUNTER;
            DOWN_MOVE_HUNTER;

        }else if (player_x > hunter_x && player_y < hunter_y) {

            RIGHT_MOVE_HUNTER;
            UP_MOVE_HUNTER;

        }else if (player_x < hunter_x && player_y < hunter_y) {

            LEFT_MOVE_HUNTER;
            UP_MOVE_HUNTER;

        }

    if (hunter_y == player_y && hunter_x == player_x) {

    }
}


void hunter_init(hunter_t *hunter, type_t *type) {
    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

        hunter[i].hunter_type = rand() % 5;

        const unsigned int h_type = hunter[i].hunter_type;

        hunter[i].bounces_done = 0;
        hunter[i].is_active = FALSE;
        hunter[i].hunter_dmg = type[h_type].dmg;
        hunter[i].bounces_left = rand() % type[h_type].bounces_max;
        hunter[i].width = type[h_type].size.width;
        hunter[i].height = type[h_type].size.height;

    }
}


void hunter_spawn(hunter_t *hunter, const player_t *player) {

    if (rand() % 100 <= 100) {

        for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

            if (hunter[i].is_active == FALSE) {

                hunter[i].target_pos.x = player->coordinates.x;
                hunter[i].target_pos.y = player->coordinates.y;

                int d_x = hunter[i].target_pos.x - hunter[i].hunter_pos.x;

                hunter[i].is_active = TRUE;

                if (rand()%4 == 1) {

                    hunter[i].hunter_pos.x = (float)(rand() % COLS);
                    hunter[i].hunter_pos.y = (float)hunter->height;

                }else if (rand()%4 == 0) {

                    hunter[i].hunter_pos.x = (float)hunter->width;
                    hunter[i].hunter_pos.y = rand() % LINES;

                }else if (rand()%4 == 2) {

                    hunter[i].hunter_pos.x = (float)(rand() % COLS);
                    hunter[i].hunter_pos.y = PLAYABLE_AREA_SIZE_Y - hunter->height;

                }else if (rand()%4 == 3) {

                    hunter[i].hunter_pos.x = (float)(COLS - hunter->width);
                    hunter[i].hunter_pos.y = rand() % LINES - hunter->height;

                }
            }
        }
    }
}


void hunter_update(hunter_t *hunter, const player_t *player) {

    for (int i = 0; i < MAX_AMM_STARS; i++) {

        if (hunter[i].is_active == TRUE) {

            hunter_move(hunter,player);

        }
    }
}


void hunter_dmg(hunter_t *hunter, player_t *player) {

}
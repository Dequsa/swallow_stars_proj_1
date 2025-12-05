//
// Created by Kacper Marciniak on 11/14/25.
//

#include "board.h"
#define STAR_WIDTH (1.0f)


void stars_init(star_t *star) {

    for (int i = 0; i < MAX_AMM_STARS; i++) {

        star[i] = star_t{};
        star[i].sprite = '*';

    }
}


void stars_spawn (star_t *star, int *stars_count) {

    if (rand() % 10 <= 2) {
        
        for (int i = 0; i < MAX_AMM_STARS; i++) {

            if (star[i].is_active == FALSE && rand() % 10 <= 2) {

                star[i].is_active = TRUE;
                star[i].position.x = rand() % (COLS - 2) + 1; // -2 is borders
                star[i].position.y = 1.0f;
                star[i].velocity = (float)(rand() % 5) + 1.0f; // 1 is for rand == 0
                (*stars_count)++;
                return;
            }
        }
    }
}


void stars_update(star_t *star, int *stars_count) {
    
    for (int i = 0; i < MAX_AMM_STARS; i++) {

        if (star[i].is_active == TRUE) {

            star[i].position.y += star[i].velocity * 0.1f;

            if (star[i].position.y > LINES - STATUS_LINE_SIZE - 2.0) {

                star[i].is_active = FALSE;
                star[i].position.y = 0.0f;
                star[i].velocity = 0.0f;
                (*stars_count)--;
            }
        }
    }
}


void stars_collect(star_t *star, player_t *player, int *stars_count, int **map) {

    if (player->in_taxi) return;

    const int p_x = (int)player->coordinates.x;
    const int p_y = (int)player->coordinates.y;

    for (int i = 0; i < PLAYER_SPRITE_SIZE; i++) { // iterate through every player "hitbox"

        const int check_x = p_x + i;

        for (int dy = -1; dy <= 1; dy++) { // check for above and below of the player position in case of miss-matching speeds

            const int check_y = p_y + dy;

            if (check_x >= 0 && check_x < COLS && check_y >= 0 && check_y < LINES) { // boundary check for player so it doesn't look off map

                const int cell_content = map[check_y][check_x];

                if (cell_content == OCC_STAR) {

                    for (int k = 0; k < MAX_AMM_STARS; k++) {

                        if (star[k].is_active == TRUE) {

                            const int star_x = (int)star[k].position.x;
                            const int star_y = (int)star[k].position.y;

                            if (star_x == check_x && star_y == check_y) {

                                star[k].is_active = FALSE;
                                star[k].position.y = 0.0f;
                                star[k].velocity = 0.0f;

                                player->stars_collected++;
                                (*stars_count)--;

                                map[check_y][check_x] = OCC_EMPTY; // clear the space so player won't collect the star multiple times

                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}
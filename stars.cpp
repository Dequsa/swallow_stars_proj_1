//
// Created by Kacper Marciniak on 11/14/25.
//

#include "board.h"


void stars_init(star_t *star) {

    for (int i = 0; i < MAX_AMM_STARS; i++) {

        star[i] = star_t{};
        star[i].sprite = '*';

    }
}


void stars_spawn (star_t *star, int *stars_count) {

    for (int i = 0; i < MAX_AMM_STARS; i++) {
        if (star[i].is_active == FALSE && rand() % 10 <= 3) {
            star[i].is_active = TRUE;
            star[i].position.x = rand() % (COLS - 2);
            star[i].position.y = 1.0f;
            star[i].velocity = (float)(rand() % 5) + 1.0f;
            (*stars_count)++;
            return;
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


void stars_collect(star_t *star, player_t *player, int *stars_count) {
    for (int i = 0; i < MAX_AMM_STARS; i++) {
        if (star[i].is_active == TRUE && (float)star[i].position.x >= player->coordinates.x - 1 && (float)star[i].position.x <= player->coordinates.x + PLAYER_WIDTH && (int)player->coordinates.y == (int)star[i].position.y) {
            star[i].is_active = FALSE;
            star[i].position.y = 0.0f;
            star[i].velocity = 0.0f;
            (*stars_count)--;
            player->stars_collected++;

        }
    }
}
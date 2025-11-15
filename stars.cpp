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

void stars_spawn (star_t *stars, int *stars_count) {
    int diff = *stars_count + 1;
    const float vel = (float)(rand() % diff);
    if (*stars_count == 0) {
        stars[*stars_count].is_active = TRUE;
        stars[*stars_count].position.x = rand() % COLS - 2;
        stars[*stars_count].position.y = 1.0f;
        stars[*stars_count].velocity = vel;
        (*stars_count)++;
    } else {
        if (stars[*stars_count].is_active == FALSE && rand() % 2 == 0) {
            stars[*stars_count].is_active = TRUE;
            stars[*stars_count].position.x = rand() % COLS - 2;
            stars[*stars_count].position.y = 1.0f;
            stars[*stars_count].velocity = vel;
            (*stars_count)++;
        }
    }
}

void stars_update(star_t *stars, int *stars_count) {
    for (int i = 0; i < MAX_AMM_STARS; i++) {
        if (stars[i].is_active == TRUE) {
            stars[i].position.y += stars[i].velocity * 0.1f;
        }
        if (stars[i].position.y > LINES - STATUS_LINE_SIZE - 2.0) {
            stars[i].is_active = FALSE;
            stars[i].position.y = 0.0f;
            stars[i].velocity = 0.0f;
            (*stars_count)--;
        }
    }
}
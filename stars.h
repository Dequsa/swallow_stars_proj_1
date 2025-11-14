//
// Created by Kacper Marciniak on 11/14/25.
//

#ifndef SWALLOW_GAME_PROJ_1_STARS_H
#define SWALLOW_GAME_PROJ_1_STARS_H
#include "board.h"
#endif //SWALLOW_GAME_PROJ_1_STARS_H

typedef struct {
    float weight;
    struct position {
        float x;
        float y;
    };
    int difficulty;
}star_t;
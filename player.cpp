//
// Created by Kacper Marciniak on 11/7/25.
//

// THIS FILE HANDLES ALL THE LOGIC WHEN IT COMES TO PLAYER
// MOVING DMG_RECIVE CURRENT SCORE

#include "player.h"


#define MAX_PLAYER_HEALTH 100

void init_player(player_t *player) {
    player->health = MAX_PLAYER_HEALTH;
    player->current_speed = 0;
    player->current_heading = DOWN;
    player->stars_collected = 0;
}

void move_player(player_t *player) {

}

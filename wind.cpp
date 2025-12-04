#include "board.h"
#define SCREEN_RATIO (0.56f)
#define WIND_CHANGE_INTERVAL (1 * FPS) // 2 sec interval betwen gusts
#define WIND_SMOOTHING (0.15f)     // smoothing of the wind
#define MAX_WIND_FORCE (1.5f)      // max force of the wind gust

void update_wind(wind_t *wind) {
    
    if (wind->change_timer <= 0) {

         // generete target place where the wind wants to be
        wind->target_x = ((float)(rand() % 200) / 100.0f - 1.0f) * MAX_WIND_FORCE;
        wind->target_y = ((float)(rand() % 200) / 100.0f - 1.0f) * MAX_WIND_FORCE;
        
        wind->change_timer = WIND_CHANGE_INTERVAL;
    }

    wind->change_timer--;

    // calculte the gap between the place where the wind is and where it wants to be add smoothing so it's not like teleportation
    wind->current_x += (wind->target_x - wind->current_x) * WIND_SMOOTHING;
    wind->current_y += (wind->target_y - wind->current_y) * WIND_SMOOTHING;
}


void apply_wind(const wind_t *wind, player_t *player, hunter_t *hunters, taxi_t *taxi) {
    
    // apply wind to player only to direction where the player is going for the ease of collecting stars
    if (player->current_heading == UP || player->current_heading == DOWN) {
    
        player->coordinates.y += wind->current_y * SCREEN_RATIO;
   
    } else {

        player->coordinates.x += wind->current_x;

    }

    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {
     
        // apply wind only when hunter is not dashing so it's not missing the player spot
        if (hunters[i].is_active && hunters[i].dash_state == NORMAL) {
     
            hunters[i].hunter_pos.x += wind->current_x;
            hunters[i].hunter_pos.y += wind->current_y * SCREEN_RATIO;
        }
    }
}
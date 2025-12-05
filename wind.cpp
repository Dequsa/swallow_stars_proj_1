#include "board.h"
#define SCREEN_RATIO (0.56f)
#define WIND_CHANGE_INTERVAL (30 * FPS) // 2 sec interval betwen gusts
#define WIND_SMOOTHING (0.05f) // smoothing of the wind
#define MAX_WIND_FORCE (0.30f) // max force of the wind gust
#define PLAYABLE_AREA_X (1 + (COLS - 1))
#define PLAYABLE_AREA_Y (LINES - STATUS_LINE_SIZE - 1)

void update_wind(wind_t *wind) {
    
    if (wind->change_timer <= 0) {

         // generete target place where the wind vector points to

        // if rand gives 0 make it -1 if 1 give 1
        const int direction_x = (rand() % 2 == 0) ? -1 : 1;
        const int direction_y = (rand() % 2 == 0) ? -1 : 1;

        wind->target_x = direction_x * MAX_WIND_FORCE;
        wind->target_y = direction_y * MAX_WIND_FORCE;

        
        wind->change_timer = WIND_CHANGE_INTERVAL;
    }

    wind->change_timer--;

    // calculte the gap between the place where the wind is and where it poitns to * by smoothing so it doestn make player teleport
    wind->current_x += (wind->target_x - wind->current_x) * WIND_SMOOTHING;
    wind->current_y += (wind->target_y - wind->current_y) * WIND_SMOOTHING;

}


void apply_wind( wind_t *wind, player_t *player, hunter_t *hunters) {
    
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
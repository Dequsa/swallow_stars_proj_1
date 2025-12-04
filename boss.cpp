#include "board.h"
#define CHANCE_OF_WIND 5 // percentage chance of wind gust happening each frame
#define MAX_WIND_STRENGTH 2 // maximum strength of wind gust

void wind_effect_y(player_t *player, hunter_t *hunters, taxi_t *taxi, const float wind_strength) {

    player->coordinates.y += wind_strength * 0.56f;
    
    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {
        if (hunters[i].is_active) {
            hunters[i].vel.y += wind_strength * 0.56f;
        }
    }

    if (taxi->visible) {
         taxi->velocity.y += wind_strength * 0.56f;
    }
}

void wind_effect_x(player_t *player, hunter_t *hunters, taxi_t *taxi, const float wind_strength) {

    player->coordinates.x += wind_strength * 0.56f;
    
    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {
        if (hunters[i].is_active) {
            hunters[i].vel.x += wind_strength;
        }
    }

    if (taxi->visible) {
         taxi->velocity.x += wind_strength;
    }
}

void wind_gust(player_t *player, hunter_t *hunters, taxi_t *taxi) {
    if (rand() % 100 <= CHANCE_OF_WIND) {
        int wind_direction = rand() % 4; // 0 up, 1 down, 2 left, 3 right
        float wind_strength = (float)(rand() % MAX_WIND_STRENGTH); // how much the wind affects positions

        switch (wind_direction) {
            case 0: { // up
                wind_effect_y(player, hunters, taxi, wind_strength);
                break;
            }
            case 1: { // down
                
                wind_strength *= -1.0f;

                wind_effect_y(player, hunters, taxi, wind_strength);
                break;
            }
            case 2: { // left
                wind_strength *= -1.0f;

                wind_effect_x(player, hunters, taxi, wind_strength);
                break;
            }
            case 3: { // right

                wind_effect_x(player, hunters, taxi, wind_strength);
                break;
            }
            default: {
                break;
            }

        }
    }
}
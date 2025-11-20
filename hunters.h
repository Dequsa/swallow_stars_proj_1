//
// Created by Kacper Marciniak on 11/18/25.
//

#ifndef SWALLOW_GAME_PROJ_1_HUNTERS_H
#define SWALLOW_GAME_PROJ_1_HUNTERS_H
#define MAX_AMM_HUNTERS 10 // 2 easy 2 med-easy 2 med-hard 2 hard 2 hard-hard
#define MAX_HUNTER_TYPES_MAX 5
#endif //SWALLOW_GAME_PROJ_1_HUNTERS_H


typedef struct {
    int bounces_left;
    unsigned int bounces_done;
    unsigned int hunter_type;
    int hunter_dmg;
    int current_heading;
    unsigned short is_active;
    int width;
    int height;
    float target_player_location_x;
    float target_player_location_y;
    position_t hunter_pos;
}hunter_t;

typedef struct {
    int bounces_max;
    int color;
    int dmg;
    int spawn_chance;
    struct size {
        int height;
        int width;
    }size;
}type_t;

void hunter_init(hunter_t *hunter, type_t *type);

void hunter_spawn(hunter_t *hunter);

void hunter_update(hunter_t *hunter, player_t *player);

void hunter_dmg(hunter_t *hunter, player_t *player);

void hunter_move(hunter_t *hunter, player_t *player);
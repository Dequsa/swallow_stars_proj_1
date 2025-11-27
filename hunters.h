//
// Created by Kacper Marciniak on 11/18/25.
//

#ifndef SWALLOW_GAME_PROJ_1_HUNTERS_H
#define SWALLOW_GAME_PROJ_1_HUNTERS_H
#define MAX_AMM_HUNTERS 20 // 4 easy 4 med-easy 4 med-hard 4 hard 4 hard-hard
#define HUNTER_TYPE_AMM 5
#endif //SWALLOW_GAME_PROJ_1_HUNTERS_H

typedef struct {
    float x;
    float y;
}velocity_t;


typedef struct {
    unsigned short displayed;
    int bounces_left;
    unsigned int bounces_done;
    unsigned int hunter_type;
    int dmg;
    unsigned short is_active;
    int width;
    int height;
    int spawn_chance;
    int cooldown;
    int min_speed;
    int max_speed;
    position_t target_pos;
    position_t hunter_pos;
    velocity_t vel;
}hunter_t;

typedef struct {
    int bounces_max;
    int color;
    int dmg;
    int spawn_chance;
    int min_speed;
    int max_speed;
    struct size {
        int height;
        int width;
    }size;
}type_t;

void hunter_init(hunter_t *hunter, const type_t *type);

void hunter_spawn(hunter_t *hunter, player_t *player,const type_t *type,  const int eva_lvl, const int eva_time);

void hunter_update(hunter_t *hunter, player_t *player);

void hunter_dmg(hunter_t *hunter, player_t *player);

void hunter_move(const hunter_t *hunter, const player_t *player);

//
// Created by Kacper Marciniak on 11/7/25.
//

#ifndef SWALLOW_GAME_PROJ_1_PLAYER_H
#define SWALLOW_GAME_PROJ_1_PLAYER_H
#define PLAYER_SPRITE_SIZE 4
#define MAX_CONFIG_SIZE 1000
#define MAX_LINE_SIZE 32
#define STATUS_LINE_SIZE 3
#endif //SWALLOW_GAME_PROJ_1_PLAYER_H

typedef struct {
    int health; // CONFIG FILE
    int current_heading; // 0 down 1 up 2 left 3 right
    int current_speed;
    int stars_collected;
    int max_speed; // CONFIG FIlLE 0 1 2 3 4 5 ...
    int min_speed;
    int time_survived;
    char frame_one[PLAYER_SPRITE_SIZE + 1];
    char frame_two[PLAYER_SPRITE_SIZE + 1];
    int current_amm_of_hunters_on_board;
    struct coordinates {
        float x;
        float y;
    } coordinates;
} player_t;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} directions_t;

typedef enum {
    LOW_HEALTH = 1,
    MEDIUM_HEALTH = 2,
    MAX_HEALTH = 3
} swallow_colors_t;

void init_player(player_t *player);

void render_player(player_t *player);

void destroy_player(player_t *player);

void out_of_bounds_check_player(float *new_x, float *new_y);

void move_player(player_t *player);


//void velocity_update(player_t *player);
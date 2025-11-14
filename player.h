//
// Created by Kacper Marciniak on 11/7/25.
//

#ifndef SWALLOW_GAME_PROJ_1_PLAYER_H
#define SWALLOW_GAME_PROJ_1_PLAYER_H
#define PLAYER_SPRITE_SIZE 4
#define MAX_CONFIG_SIZE 1000
#define MAX_LINE_SIZE 32
#endif //SWALLOW_GAME_PROJ_1_PLAYER_H

typedef struct {
    int health; // CONFIG FILE
    int current_heading; // 0 down 1 up 2 left 3 right
    int current_speed;
    int stars_collected;
    float max_speed; // CONFIG FIlLE 0 1 2 3 4 5 ...
    int time_survived;
    char frame_one[PLAYER_SPRITE_SIZE + 1];
    char frame_two[PLAYER_SPRITE_SIZE + 1];
    struct coordinates {
        float x;
        float y;
    } coordinates;
    char debug_spite[PLAYER_SPRITE_SIZE + 1]; // V^V ^V^
    // float coordinates_x;
    // float coordinates_y;
    // float velocity_x;
    // float velocity_y;
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

void load_config_player(player_t *player);

void init_player(player_t *player);

void render_player(player_t *player);

void destroy_player(player_t *player);

void move_player(player_t *player);

//void velocity_update(player_t *player);
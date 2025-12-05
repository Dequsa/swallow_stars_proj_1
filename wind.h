#ifndef SWALLOW_GAME_PROJ_1_WIND_H
#define SWALLOW_GAME_PROJ_1_WIND_H
#define WIND_PARTICLE_COUNT 20

typedef struct {
    float current_x; // The actual wind force applied this frame
    float current_y; 
    float target_x;  // The random target the wind is shifting towards
    float target_y;
    int change_timer; // Countdown until we pick a new target direction
} wind_t;

void update_wind(wind_t *wind);

void apply_wind( wind_t *wind, player_t *player, hunter_t *hunters);

#endif //SWALLOW_GAME_PROJ_1_WIND_H
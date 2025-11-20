#include <iostream>
#include "board.h" // connects all libs together
#define TIME_LEFT 7200 // 2min

#define HUNTER_TYPE_AMM 5
#include <cstring>
#define CONFIG_PATH_STATS "../CONFIGS/stats.cfg"
#define CONFIG_PATH_HUNTERS "../CONFIGS/hunters.cfg"
#define CONFIG_SUCCESS 0
#define CONFIG_ERR (-1)
#define CONFIG_ERR_NULLPTR (-2)
#define LAST_LEVEL_NUMBER 5;


int load_config_player(player_t *player) {
    FILE *fptr = nullptr;
    fptr = fopen(CONFIG_PATH_STATS, "r");

    if (fptr == nullptr) {
        mvwprintw(game_window, LINES / 2 - 1, COLS / 2 - 1, "Config stats.cfg not found...");
        return CONFIG_ERR;

    }

    int result = 0;
    char current_line[MAX_LINE_SIZE];

    while (fgets( current_line, MAX_LINE_SIZE, fptr)) {

        char name_of_variable_config[MAX_LINE_SIZE];

        if (const int temp_line = sscanf(current_line, "%d @%s", &result, name_of_variable_config); strcmp(name_of_variable_config, "MAX_HEALTH") == 0 && temp_line == 2) {

            player->health = result;

        }else if (strcmp(name_of_variable_config, "MAX_SPEED") == 0 && temp_line == 2) {

            player->max_speed = result;

        }else if (strcmp(name_of_variable_config, "MIN_SPEED") == 0 && temp_line == 2) {

            player->min_speed = result;

        }
    }

    fclose(fptr);
    return CONFIG_SUCCESS;
}

int load_config_hunter(hunter_t *hunter, type_t *type) {

    FILE *fptr = nullptr;
    fptr = fopen(CONFIG_PATH_HUNTERS, "r");
    if (fptr == nullptr) {
        return CONFIG_ERR_NULLPTR;
    }

    int result = 0;

    char current_line[MAX_LINE_SIZE];
    int current_assign_number = -1;
    while (fgets( current_line, MAX_LINE_SIZE, fptr)) {

        char name_of_variable_hunter[MAX_LINE_SIZE];

        int temp_line = sscanf(current_line, "%d @%s", &result, name_of_variable_hunter);

        if (temp_line != 2) {
            continue;
        }

        if (strcmp(name_of_variable_hunter, "HUNTER_TYPE_ID") == 0) {
            current_assign_number = result;
        }

        if (current_assign_number == -1) {
            return CONFIG_ERR;
        }


        if (strcmp(name_of_variable_hunter, "HUNTER_DMG") == 0) {
            type[current_assign_number].dmg = result;
        } else if (strcmp(name_of_variable_hunter, "MAX_BOUNCES") == 0) {
            type[current_assign_number].bounces_max = result;
        }else if (strcmp(name_of_variable_hunter, "WIDTH") == 0) {
            type[current_assign_number].size.width = result;
        }else if (strcmp(name_of_variable_hunter, "HEIGHT") == 0) {
            type[current_assign_number].size.height = result;
        }else if (strcmp(name_of_variable_hunter, "SPAWN_CHANCE_PERCENT") == 0) {
            type[current_assign_number].spawn_chance = result;
        }
    }
    fclose(fptr);
    return CONFIG_SUCCESS;
}



int main() {
    star_t stars[MAX_AMM_STARS];
    hunter_t hunters[MAX_AMM_HUNTERS];
    type_t hunter_types[HUNTER_TYPE_AMM];
    board_t board;
    player_t player;
    timespec req;
    timespec rem;
    unsigned short current_level = 0;

    const unsigned long seed = time(nullptr);
    srand(seed);

    if (load_config_player(&player)) {
        printf("Failed to load config player\n");
        return 1;
    } else {
        printf("Config loaded successfully\n");
    }

    if (load_config_hunter(hunters, hunter_types) == CONFIG_ERR) {
        printf("Failed to load config hunters\n");
        return 1;
    }
    if (load_config_hunter(hunters, hunter_types) == CONFIG_ERR_NULLPTR) {
        printf("Failed to load config hunters NULL POINTER\n");
    }

    init_board(&board);
    init_player(&player);
    stars_init(stars);
    hunter_init(hunters, hunter_types);

    req.tv_nsec = 16666666;
    req.tv_sec = 0;

    // main game loop
    board.star_quota = 10;
    int stars_count = 0;
    int time_left = TIME_LEFT;
    while (!board.is_over) {

        //----------------STARS-----------------
        stars_spawn(&stars[stars_count], &stars_count);
        if (stars_count == MAX_AMM_STARS) stars_count = 0;
        stars_update(stars, &stars_count);
        stars_collect(stars, &player , &stars_count);
        //--------------------------------------

        //----------------HUNTERS---------------
        hunter_spawn(hunters);
        hunter_update(hunters, &player);
        //--------------------------------------


        //----------------PLAYER----------------
        move_player(&player);
        //--------------------------------------
        // log input into a file must add

        update_screen(&player, stars, hunters,stars_count, time_left);

        nanosleep(&req, &rem);

        time_left--;

        if (time_left <= 0) {
            board.is_over = TRUE;
        }else if (player.stars_collected == board.star_quota) {
            board.is_over = TRUE;
        }

        // if (level_complete()) {
        //     current_level++;
        // }
        // if (current_level > LAST_LEVEL_NUMBER) {
        //
        // }
    }
    game_over(); //- saving score end menu etc. // save seed

    return 0;
}
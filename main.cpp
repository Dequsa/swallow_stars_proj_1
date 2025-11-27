#include <iostream>
#include "board.h" // connects all libs together
#include <cstring>
#define CONFIG_PATH_STATS "../CONFIGS/stats.cfg"
#define CONFIG_PATH_HUNTERS "../CONFIGS/hunters..cfg"
#define CONFIG_PATH_BOARD "../CONFIGS/board.cfg"
#define CONFIG_SUCCESS 0
#define CONFIG_ERR (-1)
#define CONFIG_ERR_NULLPTR (-2)
#define LEVEL_AMM 5


int load_config_player(player_t *player) {
    FILE *fptr = nullptr;
    fptr = fopen(CONFIG_PATH_STATS, "r");

    if (fptr == nullptr) {
        mvwprintw(game_window, LINES / 2 - 1, COLS / 2 - 1, "Config stats.cfg not found...");
        return CONFIG_ERR_NULLPTR;

    }

    int result = 0;
    char current_line[MAX_LINE_SIZE];

    while (fgets( current_line, MAX_LINE_SIZE, fptr)) {

        char name_of_variable_config[MAX_LINE_SIZE];

        const int temp_line = sscanf(current_line, "%d @%s", &result, name_of_variable_config);

        if (strcmp(name_of_variable_config, "MAX_HEALTH") == 0 && temp_line == 2) {

            player->max_health = result;

        }else if (strcmp(name_of_variable_config, "MAX_SPEED") == 0 && temp_line == 2) {

            player->max_speed = result;

        }else if (strcmp(name_of_variable_config, "MIN_SPEED") == 0 && temp_line == 2) {
            player->min_speed = result;
        }
    }

    fclose(fptr);
    return CONFIG_SUCCESS;
}

int load_config_hunter(type_t *type) {

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
        }else if (strcmp(name_of_variable_hunter, "MAX_SPEED") == 0) {
            type[current_assign_number].max_speed = result;
        }else if (strcmp(name_of_variable_hunter, "MIN_SPEED") == 0) {
            type[current_assign_number].min_speed = result;
        }
    }
    fclose(fptr);
    return CONFIG_SUCCESS;
}

int load_config_board(board_t *boards_cache) {

    FILE *fptr = nullptr;
    fptr = fopen(CONFIG_PATH_BOARD, "r");
    if (fptr == nullptr) {
        return CONFIG_ERR_NULLPTR;
    }

    int result = 0;

    char current_line[MAX_LINE_SIZE];
    int current_assign_number = -1;
    while (fgets( current_line, MAX_LINE_SIZE, fptr)) {

        char name_of_variable_board[MAX_LINE_SIZE];

        int temp_line = sscanf(current_line, "%d @%s", &result, name_of_variable_board);

        if (temp_line != 2) {
            continue;
        }

        if (strcmp(name_of_variable_board, "LEVEL_ID") == 0) {
            current_assign_number = result;
        }

        if (current_assign_number == -1) {
            return CONFIG_ERR;
        }


        if (strcmp(name_of_variable_board, "MAX_HUNTERS") == 0) {

            boards_cache[current_assign_number].max_hunters = result;

        // } else if (strcmp(name_of_variable_board, "EVALUATION_TIME_INTERVAL") == 0) {
        //
        //     boards_cache[current_assign_number].eva_time_interval = result;

        // }else if (strcmp(name_of_variable_board, "EVALUATION_LEVEL") == 0) {
        //
        //     boards_cache[current_assign_number].eva_lvl = result;

        }else if (strcmp(name_of_variable_board, "STAR_QUOTA") == 0) {

            boards_cache[current_assign_number].star_quota = result;

        }else if (strcmp(name_of_variable_board, "TIME") == 0) {

            boards_cache[current_assign_number].time_left = result;

        }
    }
    fclose(fptr);
    return CONFIG_SUCCESS;
}

void level_complete(board_t *board, const board_t *boards_cache ,player_t *player, const int current_lvl, hunter_t *hunters, const type_t *types_hunter) {
    board->is_over = FALSE;

    board->max_hunters = boards_cache[current_lvl].max_hunters;
    board->star_quota = boards_cache[current_lvl].star_quota;
    board->eva_lvl = boards_cache[current_lvl].eva_lvl;
    // board->eva_time_interval = boards_cache[current_lvl].eva_time_interval;
    board->time_left = boards_cache[current_lvl].time_left * FPS;

    player->health = player->max_health;
    player->coordinates.x = COLS / 2;
    player->coordinates.y = LINES / 2;
    player->max_hunters_on_board = board->max_hunters;
    player->stars_collected = 0;

    hunter_spawn(hunters, player, types_hunter, board->eva_time_interval);

    if (current_lvl > 0) {
        show_lvl_complete(current_lvl);
    }
}


int main() {
    star_t stars[MAX_AMM_STARS];
    hunter_t hunters[MAX_AMM_HUNTERS];
    type_t hunter_types[HUNTER_TYPE_AMM];
    board_t board;
    board_t boards_cache[LEVEL_AMM];
    player_t player;
    taxi_t taxi;
    timespec req{};
    timespec rem{};

    const unsigned long seed = time(nullptr);
    srand(seed);

    if (load_config_player(&player)) {
        printf("Failed to load config player\n");
        return 1;
    }
    // if (load_config_player(&player) == CONFIG_ERR_NULLPTR) {
    //     printf("Failed to load config players NULL POINTER\n");
    //     return 1;
    // }

    if (load_config_hunter(hunter_types) == CONFIG_ERR || load_config_hunter(hunter_types) == CONFIG_ERR_NULLPTR) {
        printf("Failed to load config hunters\n");
        return 1;
    }
    // if (load_config_hunter(hunters, hunter_types) == CONFIG_ERR_NULLPTR) {
    //     printf("Failed to load config hunters NULL POINTER\n");
    //     return 1;
    // }

    if (load_config_board(boards_cache) == CONFIG_ERR || load_config_board(boards_cache) == CONFIG_ERR_NULLPTR) {
        printf("Failed to load config board\n");
        return 1;
    }
    // if (load_config_board(hunters, hunter_types) == CONFIG_ERR_NULLPTR) {
    //     printf("Failed to load config hunters NULL POINTER\n");
    //     return 1;
    // }

    init_board(&board);
    init_player(&player);
    stars_init(stars);
    hunter_init(hunters, hunter_types);

    req.tv_nsec = 16666666; // approx. 60fps
    req.tv_sec = 0;

    // main game loop
    int stars_count = 0;
    char player_name[MAX_PLAYER_NAME_LENGTH];

    get_player_name(player_name);

    for (int i = 0 ; i < LEVEL_AMM; i++) {

        level_complete(&board, boards_cache, &player, i, hunters, hunter_types);

        while (!board.is_over) {
            //----------------PLAYER----------------
            move_player(&player, hunters, &taxi);
            //--------------------------------------
            // log input into a file must add


            //----------------STARS-----------------
            stars_spawn(&stars[stars_count], &stars_count);
            if (stars_count == MAX_AMM_STARS) stars_count = 0;
            stars_update(stars, &stars_count);
            stars_collect(stars, &player , &stars_count);
            //--------------------------------------

            //----------------HUNTERS---------------
            const int total_level_frames = boards_cache[i].time_left * FPS;
            const int frames_passed = total_level_frames - board.time_left;
            const int difficulty_adder = (frames_passed * 9) / total_level_frames;
            const int eva_time = 1 + difficulty_adder;

            hunter_spawn(hunters, &player, hunter_types, eva_time);
            hunter_update(hunters, &player, eva_time);
            //--------------------------------------

            update_screen(&player, stars, hunters, player_name, board.time_left, i);

            nanosleep(&req, &rem);

            board.time_left--;

            if (board.time_left <= 0 || player.health <= 0) {
                board.is_over = TRUE;
            }else if (player.stars_collected == board.star_quota) {
                break;
            }

        }
        if (board.is_over) {
            break;
        }
    }

    game_over(); //- saving score end menu etc.

    return 0;
}
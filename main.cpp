#include <iostream>
#include "board.h" // connects all libs together
#include <cstring>
#include <sys/time.h> // normal time.h is not working must look for one that is in sys category
#define CFG_S "../CONFIGS/stats.cfg"
#define CFG_H "../CONFIGS/hunters.cfg"
#define CFG_B "../CONFIGS/board.cfg"
#define ERR_S 0
#define CONFIG_ERR (-1)
#define ERR_NPTR (-2)
#define LEVEL_AMM 5
#define TAXI_KEY 'x'
#define OCC_EMPTY -1
#define OCC_PLAYER -2
#define OCC_STAR -3
#define DIFF_MULTIPLIER_MAX 3

int check_string_validity(const char *str) {
    if (strlen(str) == 0 || isspace(str[0])) {
        return FALSE;
    }
    return TRUE;
}


void check_player_name(char *player_name) {
    if (!check_string_validity(player_name)) {
        strcpy(player_name, "guest");
    }
}


int check_null_pointer(const FILE* fptr) {
    if (fptr == nullptr) {
        return ERR_NPTR;
    }
    return ERR_S;
}


int load_config_player(FILE* fptr ,player_t *player) {

    fptr = fopen(CFG_S, "r");

    if (check_null_pointer(fptr))
        return ERR_NPTR;

    int r = 0;
    char current_line[MAX_LINE_SIZE];

    while (fgets( current_line, MAX_LINE_SIZE, fptr)) {

        char current_var[MAX_LINE_SIZE]; // name of variable config

        const int temp_line = sscanf(current_line, "%d @%s", &r, current_var);

        if (strcmp(current_var, "MAX_HEALTH") == 0 && temp_line == 2) {

            player->max_health = r;

        }else if (strcmp(current_var, "MAX_SPEED") == 0 && temp_line == 2) {

            player->max_speed = r;

        }else if (strcmp(current_var, "MIN_SPEED") == 0 && temp_line == 2) {
            player->min_speed = r;
        }
    }

    fclose(fptr);
    return ERR_S;
}


int load_config_hunter(FILE* fptr, type_t *t) {

    fptr = fopen(CFG_H, "r");

    if (check_null_pointer(fptr)) return ERR_NPTR;

    int r = 0;
    char current_line[MAX_LINE_SIZE];
    int type_num = -1;

    while (fgets( current_line, MAX_LINE_SIZE, fptr)) {

        char current_var[MAX_LINE_SIZE]; // name of line variable hunter e.g HUNTER_DMG

        int temp_line = sscanf(current_line, "%d @%s", &r, current_var);

        if (temp_line != 2) {
            continue;
        }

        if (strcmp(current_var, "HUNTER_TYPE_ID") == 0) {
            type_num = r;
        }

        if (type_num == -1 || type_num >= HUNTER_TYPE_AMM) {
            return CONFIG_ERR;
        }

        if (strcmp(current_var, "HUNTER_DMG") == 0) {

            t[type_num].dmg = r;

        } else if (strcmp(current_var, "MAX_BOUNCES") == 0) {

            t[type_num].bounces_max = r;

        }else if (strcmp(current_var, "WIDTH") == 0) {

            t[type_num].size.width = r;

        }else if (strcmp(current_var, "HEIGHT") == 0) {

            t[type_num].size.height = r;

        }else if (strcmp(current_var, "SPAWN_CHANCE_PERCENT") == 0) {

            t[type_num].spawn_chance = r;

        }else if (strcmp(current_var, "MAX_SPEED") == 0) {

            t[type_num].max_speed = r;

        }else if (strcmp(current_var, "MIN_SPEED") == 0) {

            t[type_num].min_speed = r;

        }else if (strcmp(current_var, "COLOR") == 0) {

            t[type_num].color = r;

        }
    }
    fclose(fptr);
    return ERR_S;
}


int load_config_board(FILE* fptr, board_t *boards_cache) {

    fptr = fopen(CFG_B, "r");

    if (check_null_pointer(fptr)) return ERR_NPTR;

    int r = 0;

    char current_line[MAX_LINE_SIZE];
    int lvl_num = -1; // current assign number
    while (fgets( current_line, MAX_LINE_SIZE, fptr)) {

        char current_var[MAX_LINE_SIZE];

        int temp_line = sscanf(current_line, "%d @%s", &r, current_var);

        if (temp_line != 2) {
            continue;
        }

        if (strcmp(current_var, "LEVEL_ID") == 0) {
            lvl_num = r;
        }

        if (lvl_num == -1 || lvl_num >= LEVEL_AMM) {
            return CONFIG_ERR;
        }


        if (strcmp(current_var, "MAX_HUNTERS") == 0) {

            boards_cache[lvl_num].max_hunters = r;

        }else if (strcmp(current_var, "STAR_QUOTA") == 0) {

            boards_cache[lvl_num].star_quota = r;

        }else if (strcmp(current_var, "TIME") == 0) {

            boards_cache[lvl_num].time_left = r;

        }
    }
    fclose(fptr);
    return ERR_S;
}


int load_configs(FILE *fptr, player_t *player, type_t *hunter_types, board_t *boards_cache ) {

    int config_err = load_config_player(fptr, player);

    if (config_err != ERR_S) {
        printf("Failed to load config player\n");
        return 1;
    }

    config_err = load_config_hunter(fptr, hunter_types);

    if (config_err != ERR_S ) {
        printf("Failed to load config hunters\n");
        return 1;
    }

    config_err = load_config_board(fptr, boards_cache);

    if (config_err != ERR_S) {
        printf("Failed to load config board\n");
        return 1;
    }


    return 0;
}


void hunter_delete_all(hunter_t *hunters, player_t *player) {

    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {
        if (hunters[i].is_active == TRUE) {
            hunters[i].is_active = FALSE;
            hunters[i].bounces_done = 0;
            player->current_amm_of_hunters_on_board--;
        }
    }
}


double get_current_time_seconds() {

    timeval current_time{};

    gettimeofday(&current_time, nullptr);

    return current_time.tv_sec + (current_time.tv_usec / 1000000);

}


int calculate_time_left_frames(const board_t *board) {

    const double current_time = get_current_time_seconds();
    const double start_time_lvl = board->level_start_time;
    const double time_limit_lvl = board->time_limit_seconds;

    const double passed_time = current_time - start_time_lvl;

    const double time_remaining = time_limit_lvl - passed_time;

    return (int)(time_remaining * FPS);

}


void update_occupancy_map(board_t *board, const hunter_t *hunters) {
    
    for (int y = 0; y < LINES; y++) {
        for (int x = 0; x < COLS; x++) {

            board->occupancy_map[y][x] = OCC_EMPTY;

        }
    }

    for (int i = 0; i < MAX_AMM_HUNTERS; i++) {

        if (hunters[i].is_active == TRUE) {
            
            int start_x = (int)hunters[i].hunter_pos.x;
            int start_y = (int)hunters[i].hunter_pos.y;
            
            for (int h = 0; h < hunters[i].height; h++) {
                for (int w = 0; w < hunters[i].width; w++) {
                    
                    int y = start_y + h;
                    int x = start_x + w;

                    if (y >= 0 && y < LINES && x >= 0 && x < COLS) {

                        board->occupancy_map[y][x] = i;  // store hunter index on the map

                    }
                }
            }
        }
    }
}


void level_complete(board_t *board, const board_t *boards_cache ,player_t *player, const int current_lvl, hunter_t *hunters, const type_t *types_hunter, taxi_t *taxi) {

    board->time_limit_seconds = boards_cache[current_lvl].time_left;
    board->level_start_time = get_current_time_seconds();
    board->time_left = boards_cache[current_lvl].time_left * FPS;

    board->max_hunters = boards_cache[current_lvl].max_hunters;
    board->star_quota = boards_cache[current_lvl].star_quota;
    board->eva_lvl = boards_cache[current_lvl].eva_lvl;
    board->is_over = FALSE;

    player->health = player->max_health;
    player->coordinates.x = COLS / 2;
    player->coordinates.y = LINES / 2;
    player->max_hunters_on_board = board->max_hunters;
    player->stars_collected = 0;

    player->has_called_taxi = FALSE;
    player->in_taxi = FALSE;
    taxi->is_active = FALSE;
    taxi->visible = FALSE;

    hunter_delete_all(hunters, player);
    hunter_spawn(hunters, player, types_hunter, board->eva_time_interval);

    if (current_lvl > 0) {
        show_lvl_complete(current_lvl);
    }
}


void seed_set() {
    const unsigned long seed = time(nullptr);
    srand(seed);
}


void stars_all(int *stars_count, player_t *player, star_t *stars ){

    stars_spawn(&stars[*stars_count], stars_count);

    if (*stars_count == MAX_AMM_STARS) *stars_count = 0;

    stars_update(stars, stars_count);
}


void hunters_movement_all(hunter_t *hunters, player_t *player, const type_t *hunter_types, const int board_cache_time_left, const int i, const int time_left, int **map ) {

    const int total_level_frames = board_cache_time_left * FPS;

    const int frames_passed = total_level_frames - time_left; // how many frames have passed since level started

    const int difficulty_adder = (DIFF_MULTIPLIER_MAX * frames_passed) / total_level_frames; // goes from 0 to DIFF_MULTIPLIER_MAX

    const int eva_time = 1 + difficulty_adder; // evaluation over time increases as level progresses from 1 to 3



    hunter_spawn(hunters, player, hunter_types, eva_time);
    hunter_update(hunters, player, eva_time, map);
}


void collision_all(board_t *board, hunter_t *hunters, player_t *player, star_t *stars, int *stars_count) {
    
    update_occupancy_map(board, hunters); // handels the occupation amp

    stars_collect(stars, player , stars_count);
    hunter_dmg(hunters , player, board->occupancy_map);
}


void taxi_all(player_t *player, taxi_t *taxi, const int input_key) {

    if (input_key == TAXI_KEY && !player->has_called_taxi && taxi->cooldown <= 0) {
                
        taxi->dropped = FALSE;
        taxi->found_drop = FALSE;
        taxi->is_active = TRUE;

        player->has_called_taxi = TRUE;
            
    }
            
    if (taxi->cooldown > 0) {
        taxi->cooldown--;
    }

    if(taxi->is_active) {
        taxi_update(taxi, player, input_key);
    }
    
}


int check_over(const int time_left, const int health, int* game_over, const int collected_stars, const unsigned int star_quota, int input_key) {

    if (time_left <= 0 || health <= 0 || input_key == 'q') {

        *game_over = TRUE;

        return 1;

    }else if (collected_stars == star_quota || input_key == 'n') {

        return 1;

    }

    return 0;

}


void save_score(int *player_score, const int *collected_stars, const int *time_left) {

    const int points_from_stars = (*collected_stars) * 100;

    const int seconds_left = (*time_left) / FPS;
    const int points_from_time = seconds_left * 10;

    *player_score += (points_from_stars + points_from_time);
}


void main_game_loop(board_t *board, board_t *boards_cache, player_t *player, hunter_t *hunters, type_t *hunter_types, star_t *stars, char* player_name, taxi_t *taxi, wind_t *wind) {

    timespec req{};
    timespec rem{};
    req.tv_nsec = 16666666; // approx. 60fps
    req.tv_sec = 0;

    int stars_count = 0;
    int current_lvl;

    player->score = 0;

    for (current_lvl = 0 ; current_lvl < LEVEL_AMM; current_lvl++) {

        level_complete(board, boards_cache, player, current_lvl, hunters, hunter_types, taxi);
        taxi_spawn(taxi);

        while (!board->is_over) {

            int input_key = -1;

            update_wind(wind);
            apply_wind(wind, player, hunters);


            move_player(player, &input_key);

            taxi_all(player, taxi, input_key);

            stars_all(&stars_count, player, stars);

            hunters_movement_all(hunters, player, hunter_types, boards_cache[current_lvl].time_left, current_lvl, board->time_left, board->occupancy_map);

            collision_all(board, hunters, player, stars, &stars_count);

            update_screen(player, stars, hunters, player_name, board->time_left, current_lvl, taxi);

            board->time_left = calculate_time_left_frames(board);

            nanosleep(&req, &rem);


            if (check_over(board->time_left, player->health, &board->is_over, player->stars_collected, board->star_quota, input_key)) {
            
                save_score(&player->score, &player->stars_collected, &board->time_left);
                break;
            }

        }

        if (current_lvl == LEVEL_AMM - 1 && !board->is_over) {
            
            show_win_screen();
        
        }
        
        if (board->is_over) {
            break;
        }
    }

    game_over(player_name, player->score);

}


void allocate_mem_map(int **&map) { // reference to * to * adress of the board.occupancy_map
    map = new int *[LINES];

    for (int i = 0; i < LINES; i++) {
        map[i] = new int[COLS];
    }
}


void free_mem_map(int **map) { // no need to reference here we just free the mem
    for (int i = 0 ; i < LINES; i++) {
        delete [] map[i];
    }
    delete [] map;
}



int main() {

    star_t stars[MAX_AMM_STARS];
    hunter_t hunters[MAX_AMM_HUNTERS];
    type_t hunter_types[HUNTER_TYPE_AMM];
    board_t board;
    board_t boards_cache[LEVEL_AMM];
    player_t player;
    taxi_t taxi;
    FILE *fptr = nullptr;
    wind_t wind = {};

    seed_set();

    load_configs(fptr, &player, hunter_types, boards_cache);

    init_board(&board);
    allocate_mem_map(board.occupancy_map);
    init_player(&player);
    stars_init(stars);
    hunter_init(hunters, hunter_types);
    taxi_spawn(&taxi);

    char player_name[MAX_PLAYER_NAME_LENGTH];

    get_player_name(player_name);
    check_player_name(player_name);

    main_game_loop(&board, boards_cache, &player, hunters, hunter_types, stars, player_name, &taxi, &wind);


    free_mem_map(board.occupancy_map);
    return 0;
}
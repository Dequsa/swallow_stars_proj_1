#include <iostream>
#include "board.h" // connects all libs together
#include <cstring>
#define CFG_S "../CONFIGS/stats.cfg"
#define CFG_H "../CONFIGS/hunters..cfg"
#define CONFIG_PATH_BOARD "../CONFIGS/board.cfg"
#define ERR_S 0
#define CONFIG_ERR (-1)
#define ERR_NPTR (-2)
#define LEVEL_AMM 5


int check_null_pointer(const FILE* fptr) {
    if (fptr == nullptr) {
        return ERR_NPTR;
    }
    return ERR_S;
}

int load_config_player(FILE* fptr ,player_t *player) {

    fptr = fopen(CFG_S, "r");

    if (check_null_pointer(fptr)) return ERR_NPTR;

    int r = 0;
    char c_l[MAX_LINE_SIZE]; // current line

    while (fgets( c_l, MAX_LINE_SIZE, fptr)) {

        char n_o_v_c[MAX_LINE_SIZE]; // name of variable config

        const int temp_line = sscanf(c_l, "%d @%s", &r, n_o_v_c);

        if (strcmp(n_o_v_c, "MAX_HEALTH") == 0 && temp_line == 2) {

            player->max_health = r;

        }else if (strcmp(n_o_v_c, "MAX_SPEED") == 0 && temp_line == 2) {

            player->max_speed = r;

        }else if (strcmp(n_o_v_c, "MIN_SPEED") == 0 && temp_line == 2) {
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

    char c_l[MAX_LINE_SIZE];
    int c_a_n = -1;
    while (fgets( c_l, MAX_LINE_SIZE, fptr)) {

        char n_o_v_h[MAX_LINE_SIZE]; // name of line variable hunter

        int temp_line = sscanf(c_l, "%d @%s", &r, n_o_v_h);

        if (temp_line != 2) {
            continue;
        }

        if (strcmp(n_o_v_h, "HUNTER_TYPE_ID") == 0) {
            c_a_n = r;
        }

        if (c_a_n == -1) {
            return CONFIG_ERR;
        }

        if (strcmp(n_o_v_h, "HUNTER_DMG") == 0) {

            t[c_a_n].dmg = r;

        } else if (strcmp(n_o_v_h, "MAX_BOUNCES") == 0) {

            t[c_a_n].bounces_max = r;

        }else if (strcmp(n_o_v_h, "WIDTH") == 0) {

            t[c_a_n].size.width = r;

        }else if (strcmp(n_o_v_h, "HEIGHT") == 0) {

            t[c_a_n].size.height = r;

        }else if (strcmp(n_o_v_h, "SPAWN_CHANCE_PERCENT") == 0) {

            t[c_a_n].spawn_chance = r;

        }else if (strcmp(n_o_v_h, "MAX_SPEED") == 0) {

            t[c_a_n].max_speed = r;

        }else if (strcmp(n_o_v_h, "MIN_SPEED") == 0) {

            t[c_a_n].min_speed = r;

        }
    }
    fclose(fptr);
    return ERR_S;
}

int load_config_board(FILE* fptr, board_t *boards_cache) {

    fptr = fopen(CONFIG_PATH_BOARD, "r");

    if (check_null_pointer(fptr)) return ERR_NPTR;

    int r = 0;

    char c_l[MAX_LINE_SIZE];
    int c_a_n = -1; // current assign number
    while (fgets( c_l, MAX_LINE_SIZE, fptr)) {

        char n_o_v_b[MAX_LINE_SIZE];

        int temp_line = sscanf(c_l, "%d @%s", &r, n_o_v_b);

        if (temp_line != 2) {
            continue;
        }

        if (strcmp(n_o_v_b, "LEVEL_ID") == 0) {
            c_a_n = r;
        }

        if (c_a_n == -1) {
            return CONFIG_ERR;
        }


        if (strcmp(n_o_v_b, "MAX_HUNTERS") == 0) {

            boards_cache[c_a_n].max_hunters = r;

        }else if (strcmp(n_o_v_b, "STAR_QUOTA") == 0) {

            boards_cache[c_a_n].star_quota = r;

        }else if (strcmp(n_o_v_b, "TIME") == 0) {

            boards_cache[c_a_n].time_left = r;

        }
    }
    fclose(fptr);
    return ERR_S;
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


void main_init() {

}


void seed_set() {
    const unsigned long seed = time(nullptr);
    srand(seed);
}


void stars_all(int *stars_count, player_t *player, star_t *stars ){

    stars_spawn(&stars[*stars_count], stars_count);

    if (*stars_count == MAX_AMM_STARS) *stars_count = 0;

    stars_update(stars, stars_count);
    stars_collect(stars, player , stars_count);
}

void hunters_all(hunter_t *hunters, player_t *player, const type_t *hunter_types, const int cache_time_left, const int i, const int time_left ) {

    const int total_level_frames = cache_time_left * FPS;
    const int frames_passed = total_level_frames - time_left;
    const int difficulty_adder = (frames_passed * 9) / total_level_frames;
    const int eva_time = 1 + difficulty_adder;

    hunter_spawn(hunters, player, hunter_types, eva_time);
    hunter_update(hunters, player, eva_time);
}

int check_over(const int time_left, const int health, int* game_over, const int collected_stars, const unsigned int star_quota) {

    if (time_left <= 0 || health <= 0) {

        *game_over = TRUE;

    }else if (collected_stars == star_quota) {

        return 1;
`
    }

    return 0;

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

    seed_set();

    FILE *fptr = nullptr;

    load_configs(fptr, &player, hunter_types, boards_cache);

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
            move_player(&player);
            //--------------------------------------
            // log input into a file must add

            //-----------------TAXI----------------- // WIP

            //--------------------------------------


            //----------------STARS-----------------
            stars_all(&stars_count, &player, stars);
            //--------------------------------------

            //----------------HUNTERS---------------
            hunters_all(hunters, &player, hunter_types,boards_cache[i].time_left, i, board.time_left);
            //--------------------------------------

            update_screen(&player, stars, hunters, player_name, board.time_left, i);

            nanosleep(&req, &rem);

            board.time_left--;

            if (check_over(board.time_left, player.health, &board.is_over, player.stars_collected, board.star_quota)) {
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
#include <iostream>
#include "board.h" // connects all libs together
#define TIME_LEFT 7200 // 2min


void functions_stars() {

}



int main() {
    star_t stars[MAX_AMM_STARS];
    hunter_t hunters[MAX_AMM_HUNTERS];
    board_t board;
    player_t player;
    timespec req;
    timespec rem;

    const unsigned long seed = time(nullptr);
    srand(seed);

    load_config_player(&player);
    init_board(&board);
    init_player(&player);
    stars_init(&*stars);
    hunter_init(&*hunters);

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
        stars_update(&*stars, &stars_count);
        stars_collect(&*stars, &player , &stars_count);
        //--------------------------------------

        //----------------HUNTERS---------------

        //--------------------------------------


        //----------------PLAYER----------------
        move_player(&player);
        //--------------------------------------
        // log input into a file must add

        update_screen(&player, &*stars, stars_count, time_left);

        nanosleep(&req, &rem);

        time_left--;

        if (time_left <= 0) {
            board.is_over = TRUE;
        }else if (player.stars_collected == board.star_quota) {
            board.is_over = TRUE;
        }
    }
    // game_over() - saving score end menu etc. // save seed

    return 0;
}
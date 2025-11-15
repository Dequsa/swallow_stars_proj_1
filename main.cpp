#include <iostream>
#include "board.h" // connects all libs together

int main() {
    star_t stars[MAX_AMM_STARS];
    board_t board;
    player_t player;
    timespec req;
    timespec rem;

    const unsigned long seed = time(nullptr);
    srand(seed);

    load_config_player(&player);
    init_board(&board);
    init_player(&player);

    for (int i = 0; i < MAX_AMM_STARS; i++) {
        stars_init(&stars[i]);
    }

    req.tv_nsec = 16666666;
    req.tv_sec = 0;
    // main game loop

    int stars_count = 0;

    while (!board.is_over) {

        stars_spawn(&stars[stars_count], &stars_count);

        move_player(&player);
        // log input into a file must add

        update_screen(&player);

        for (int i = 0; i < stars_count; i++) {
            update_star(&stars[i]);
        }


        nanosleep(&req, &rem);
    }
    nodelay(stdscr, FALSE);
    getch();
    // game_over() - saving score end menu etc. // save seed

    return 0;
}
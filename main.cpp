#include <iostream>
#include <curses.h>
#include <time.h>
#include "board.h" // it also has player.h in include

int main() {

    board_t board;
    player_t player;
    timespec req;
    timespec rem;
    // config reading and setting every var according to config
    //load_config(&player, &board);
    load_config_player(&player);
    init_board(&board);
    init_player(&player);
    req.tv_nsec = 166666666;

    while (!board.is_over) {
        req.tv_sec = 0;
        //take input
        move_player(&player);
        // display input --> repeat
        update_screen(&player);
        nanosleep(&req, &rem);
    }
    nodelay(stdscr, FALSE);
    getch();
    // game_over() - saving score end menu etc.

    return 0;
}
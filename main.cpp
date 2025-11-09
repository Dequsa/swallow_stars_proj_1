#include <iostream>
#include <curses.h>
#include "board.h" // it also has player.h in include

int main() {
    board_t board;
    player_t player;

    init_board(&board);
    init_player(&player);

    while (!board.is_over) {
        //take input

        // process input

        // display input --> repeat

    }

    // game_over() - saving score end menu etc.

    return 0;
}
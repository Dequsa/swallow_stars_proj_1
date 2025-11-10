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
        move_player(&player);
        // process input

        // display input --> repeat
        update_screen(&player);
    }
    nodelay(stdscr, FALSE);
    getch();
    // game_over() - saving score end menu etc.

    return 0;
}
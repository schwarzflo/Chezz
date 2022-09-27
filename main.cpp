#include <iostream>
#include "chessboard.h"
#include "piece.h"

// check implemented;

// implement check mate; current attempt is incomplete, lacks save by capture and save by blocking.

int main() {
    Chessboard cs;
    std::string move_s, move_e;
    while (true) {
        std::cout << cs.get_color() << " to play." << std::endl;
        std::cin >> move_s;
        std::cin >> move_e;
        cs.move(move_s,move_e);
        if (!cs.get_game_status()) {
            break;
        }
    }

    return 0; 
}
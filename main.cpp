#include <iostream>
#include "chessboard.h"
#include "piece.h"
#include "playing.h"

// implemented 3 move repetition, implemented stalemate, implemented automatic draw (bishop+king v king, king v king, etc..)

int main() {
    
    std::string pl1, pl2;
    char clr;

    std::cout << "Chezz v1.0, a C++ implementation of standard chess." << std::endl;
    std::cout << "Who is player 1 (h/r)? \n> ";
    std::cin >> pl1;
    std::cout << "Who is player 2 (h/r)? \n> ";
    std::cin >> pl2;
    std::cout << std::endl;

    if (pl1 == "h" && pl2 == "h") {
        Playing pl(2,0);
        pl.play();
    }
    else if ((pl1 == "h" && pl2 == "r") || (pl1 == "r" && pl2 == "h")) {
        std::cout << "Which color should the computer take? \n> ";
        std::cin >> clr;
        Playing pl(clr);
        pl.play();
    }
    else if (pl1 == "r" && pl2 == "r") {
        Playing pl(0,2);
        pl.play();
    }
    else {
        std::cout << "Invalid input, retry.." << std::endl;
    }

    return 0; 
}
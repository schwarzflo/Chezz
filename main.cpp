#include <iostream>
#include "chessboard.h"
#include "piece.h"

// add bishop, rook, queen

int main() {
    Chessboard cs;

    //russian game, petrov defense

    std::cout << cs << std::endl;
    cs.move("e2","e4");
    std::cout << cs << std::endl;
    cs.move("e7","e5");
    std::cout << cs << std::endl;
    cs.move("g1","f3");
    std::cout << cs << std::endl;
    cs.move("g8","f6");
    std::cout << cs << std::endl;
    cs.move("f3","e5");
    std::cout << cs << std::endl;
    cs.move("d7","d6");
    std::cout << cs << std::endl;

    return 0; 
}
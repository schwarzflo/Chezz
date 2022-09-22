#include <iostream>
#include "chessboard.h"
#include "piece.h"

// implement check 

int main() {
    Chessboard cs;

    cs.move("e2","e4");
    std::cout << cs << std::endl;
    cs.move("e7","e5");
    std::cout << cs << std::endl;
    cs.move("g1","f3");
    std::cout << cs << std::endl;
    cs.move("g8","f6");
    std::cout << cs << std::endl;
    cs.move("f1","e2");
    std::cout << cs << std::endl;
    cs.move("f8","e7");
    std::cout << cs << std::endl;
    cs.move("e1","g1");
    std::cout << cs << std::endl;
    cs.move("e8","g8");
    std::cout << cs << std::endl;

    return 0; 
}
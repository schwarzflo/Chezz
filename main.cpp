#include <iostream>
#include "chessboard.h"
#include "piece.h"

// add queen

int main() {
    Chessboard cs;

    //ruy lopez

    cs.move("e2","e4");
    std::cout << cs << std::endl;
    cs.move("e7","e5");
    std::cout << cs << std::endl;
    cs.move("d1","h5");
    std::cout << cs << std::endl;
    cs.move("d8","h4");
    std::cout << cs << std::endl;
    cs.move("h5","e5");
    std::cout << cs << std::endl;
    cs.move("h4","e4");
    std::cout << cs << std::endl;

    return 0; 
}
#include <iostream>
#include "chessboard.h"
#include "piece.h"
#include "playing.h"
#include <cstdlib>
#include <ctime>

// implemented human v human, human v comp, comp v comp; visualisation

// implement stalemate; implement automatic draw (bishop+king v king, king v king, etc..)

int main() {
    
    Playing pl(0,2);
    pl.play();

    return 0; 
}
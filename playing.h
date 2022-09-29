#ifndef PLAYING_H
#define PLAYING_H
#include <string>
#include "chessboard.h"

class Chessboard;

class Playing {
    int human_pl;
    int comp_pl;
    char comp_clr;
    public:
        Playing(int, int);
        Playing(char);
        ~Playing() = default;
        void play();
        void human_choice(Chessboard&);
        void computer_choice(Chessboard&);
        int get_human_pl();
};


#endif
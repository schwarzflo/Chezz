#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "piece.h"

class Piece;

class Chessboard {
    std::vector<Piece> all_p = {};
    std::vector<std::string> all_m = {};
    char color;
    int move_nr;
    std::map<char, int> conv1;  // move to external h file
    std::map<char, int> conv2;
    std::map<char, int> conv3;
    std::map<char, char> conv4;
    std::map<char, char> conv5;

    public:
        Chessboard();
        ~Chessboard() = default;
        int cr_to_idx(char, char);
        void move(std::string, std::string);
        bool en_passant(char,char);
        char idx_to_clr(int);

        std::vector<Piece> get_all_p();
        void set_all_p(std::vector<Piece>);
        void add_m(std::string);
        char get_color();
        int get_move_nr();
        std::map<char, int> get_conv1();
        std::map<char, int> get_conv2();
        std::map<char, int> get_conv3();
        std::map<char, char> get_conv4();
        std::map<char, char> get_conv5();


        friend std::ostream& operator<<(std::ostream&, Chessboard);

};
#endif
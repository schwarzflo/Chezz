#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "piece.h"
#include "playing.h"

class Piece;
class Playing;

class Chessboard {
    std::vector<Piece> all_p = {};
    std::vector<std::string> all_m = {};
    char color;
    int move_nr;
    bool game_ongoing;
    std::map<char, int> conv_c1;  // move to external h file 
    std::map<int, char> conv_c2;  // .
    std::map<char, int> conv_r1;  // .
    std::map<int, char> conv_r2;  // .
    std::map<char, int> conv3;    // .
    std::map<char, char> conv4;   // .
    std::map<char, char> conv5;   // .

    public:
        Chessboard();
        ~Chessboard() = default;
        int cr_to_idx(char, char);
        void move(std::string, std::string, Playing*);
        bool en_passant(char,char);
        char idx_to_clr(int);
        std::vector<Piece> get_all_p();
        void set_all_p(std::vector<Piece>);
        void set_sp_p(size_t, char, char);
        void erase_idx(size_t);
        void add_m(std::string);
        char get_color();
        int get_move_nr();
        bool get_game_status();
        bool in_check(std::string, std::string, size_t, char);
        bool check_mate();
        void visualize_board();
        std::string get_king_pos(char);
        std::map<char, int> get_conv_c1();
        std::map<int, char> get_conv_c2();
        std::map<char, int> get_conv_r1();
        std::map<int, char> get_conv_r2();
        std::map<char, int> get_conv3();
        std::map<char, char> get_conv4();
        std::map<char, char> get_conv5();


        friend std::ostream& operator<<(std::ostream&, Chessboard);

};
#endif
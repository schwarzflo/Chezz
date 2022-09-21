#ifndef PIECE_H
#define PIECE_H
#include <string>
#include "chessboard.h"

class Chessboard;

class Piece {
    protected:
        char color;
        char pos_c;
        char pos_r;
        char type;
        std::vector<std::string> m_list = {};
    public:
        Piece() = default;
        Piece(char, char, char, char);
        ~Piece() = default;
        char get_color();
        char get_pos_c();
        char get_pos_r();
        char get_type();
        void move(Chessboard*, std::string&, size_t&);
        void pawn_m(Chessboard&, std::string&, size_t&);
        void king_m(Chessboard&, std::string&, size_t&);
        void knight_m(Chessboard&, std::string&, size_t&);
        void bishop_m(Chessboard&, std::string&, size_t&);
        void rook_m(Chessboard&, std::string&, size_t&);
        void queen_m(Chessboard&, std::string&, size_t&);
        void update_moves(std::vector<Piece>, Chessboard&, std::string, Piece, std::string);
        void check_squares_move(Chessboard&, size_t&, char&, char&);
        void brq(char, char, Chessboard&, size_t&, std::string&);
};

#endif
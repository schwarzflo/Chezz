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
        void set_pos_c(char);
        void set_pos_r(char);
        std::vector<std::string> get_m_list();
        bool move(Chessboard*, std::string&, size_t&, bool);
        bool pawn_m(Chessboard&, std::string&, size_t&, bool);
        bool king_m(Chessboard&, std::string&, size_t&, bool);
        bool knight_m(Chessboard&, std::string&, size_t&, bool);
        bool bishop_m(Chessboard&, std::string&, size_t&, bool);
        bool rook_m(Chessboard&, std::string&, size_t&, bool);
        bool queen_m(Chessboard&, std::string&, size_t&, bool);
        void update_moves(size_t, Chessboard&, char, char, std::string);
        bool check_squares_move(Chessboard&, size_t&, char&, char&, bool);
        bool brq(char, char, Chessboard&, size_t&, std::string&, bool);

        friend std::ostream& operator<<(std::ostream&, Piece);
        friend bool operator!=(Piece&, Piece&);
};

#endif
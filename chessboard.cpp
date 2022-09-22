#include "chessboard.h"

Chessboard::Chessboard() {
    all_p = {
        Piece('w','a','2','p'),Piece('w','b','2','p'),Piece('w','c','2','p'),Piece('w','d','2','p'),
        Piece('w','e','2','p'),Piece('w','f','2','p'),Piece('w','g','2','p'),Piece('w','h','2','p'),
        Piece('b','a','7','p'),Piece('b','b','7','p'),Piece('b','c','7','p'),Piece('b','d','7','p'),
        Piece('b','e','7','p'),Piece('b','f','7','p'),Piece('b','g','7','p'),Piece('b','h','7','p'),
        Piece('w','e','1','k'),Piece('b','e','8','k'),Piece('w','b','1','n'),Piece('w','g','1','n'),
        Piece('b','b','8','n'),Piece('b','g','8','n'),Piece('w','c','1','b'),Piece('w','f','1','b'),
        Piece('b','c','8','b'),Piece('b','f','8','b'),Piece('w','a','1','r'),Piece('w','h','1','r'),
        Piece('b','a','8','r'),Piece('b','h','8','r'),Piece('w','d','1','q'),Piece('b','d','8','q')
    };
    all_m = {};
    color = 'w';    // white is first to move
    move_nr = 1;
    conv1 = {
        { 'a', 1 },{ 'b', 2 },{ 'c', 3 },{ 'd', 4 },{ 'e', 5 },{ 'f', 6 },{ 'g', 7 },{ 'h', 8 },
        { 1, 'a' },{ 2, 'b' },{ 3, 'c' },{ 4, 'd' },{ 5, 'e' },{ 6, 'f' },{ 7, 'g' },{ 8, 'h' }
    };
    conv2 = {
        { '1', 1 },{ '2', 2 },{ '3', 3 },{ '4', 4 },{ '5', 5 },{ '6', 6 },{ '7', 7 },{ '8', 8 },
        { 1, '1' },{ 2, '2' },{ 3, '3' },{ 4, '4' },{ 5, '5' },{ 6, '6' },{ 7, '7' },{ 8, '8' }
    };
    conv3 = {
        { 'w',1 },  { 'b',-1 }
    };
    conv4 = {
        {'w','b'}, {'b','w'}, {'-','-'}
    };
    conv5 = {
        {'w','1'}, {'b','8'},{'g','h'},{'c','a'}
    };
}

int Chessboard::cr_to_idx(char nc, char nr) {   // return index in list of pieces for input of coordinates
    int idx{-1};
    for (size_t i {}; i < all_p.size(); i++) {
        if (all_p[i].get_pos_c() == nc && all_p[i].get_pos_r() == nr) {
            idx = i;
            break;
        }
    }
    return idx;
}

char Chessboard::idx_to_clr(int i) {
    if (i != -1) {
        return all_p[i].get_color();
    }
    return '-'; // no color at position i
}

void Chessboard::move(std::string start, std::string end) {
    bool found{false};
    char col{start[0]}, row{start[1]};
    for (size_t i{}; i < all_p.size(); i++) {   // check whether a piece is on the start position
        if (all_p[i].get_pos_c() == col && all_p[i].get_pos_r() == row && color == all_p[i].get_color()) { // check for position and valid color
            all_p[i].move(this,end,i);  // try to move that piece
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "No valid piece at this initial position." << std::endl;
    }
    else {
        if (color == 'w') {
            color = 'b';
        }
        else {
            color = 'w';
            move_nr++;
        }
    }
}

bool Chessboard::en_passant(char col, char row) { // check if the last move was a pawn moving up (or down) two squares on a given column, in order to make en passant possible
    std::string last_move = all_m.back();
    if (last_move[0] == 'p' && last_move[1] == last_move[3] && last_move[1] == col && abs(conv2[last_move[2]]-conv2[last_move[4]]) == 2 && last_move[4] == row) {
        return true;
    }
    return false;
}

void Chessboard::add_m(std::string new_move) {
    all_m.push_back(new_move);
};

std::vector<Piece> Chessboard::get_all_p() {
    return all_p;
}

void Chessboard::set_all_p(std::vector<Piece> nap) {
    all_p = nap;
}

int Chessboard::get_move_nr() {
    return move_nr;
}

char Chessboard::get_color() {
    return color;
}

std::map<char, int> Chessboard::get_conv1() {
    return conv1;
}

std::map<char, int> Chessboard::get_conv2() {
    return conv2;
}

std::map<char, int> Chessboard::get_conv3() {
    return conv3;
}

std::map<char, char> Chessboard::get_conv4() {
    return conv4;
}

std::map<char, char> Chessboard::get_conv5() {
    return conv5;
}


std::ostream& operator<<(std::ostream& os, Chessboard cs) {
    for (auto piece : cs.all_p) {
        os << piece.get_type() << piece.get_pos_c() << piece.get_pos_r() << " " << piece.get_color() << "   ";
    };
    os << std::endl;
    for (auto move : cs.all_m) {
        os << move << "   ";
    };
    return os;
}

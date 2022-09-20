#include<string>
#include<cmath>
#include <map>
#include <vector>
#include "piece.h"

Piece::Piece(char c, char p_c, char p_r, char t) {
    color = c;
    pos_c = p_c;
    pos_r = p_r;
    type = t;
    m_list = {};
}
char Piece::get_color() {
    return color;
}
char Piece::get_pos_c() {
    return pos_c;
}
char Piece::get_pos_r() {
    return pos_r;
}
char Piece::get_type() {
    return type;
}

void Piece::move(Chessboard* cb, std::string &end, size_t &i) {     // covers all possible piece movements
    switch (type)
    {
    case 'p':   // Pawn 
        pawn_m(*cb,end,i);
        break;
    case 'k':   // King
        king_m(*cb,end,i);
        break;
    case 'n':   // Knight
        knight_m(*cb,end,i);
    }
}

void Piece::update_moves(std::vector<Piece> temp_vec, Chessboard& cb, std::string new_move, Piece p) {   // write move to list and piece
    temp_vec.push_back(p);
    cb.set_all_p(temp_vec);
    cb.add_m(new_move);
    m_list.push_back(new_move.substr(1,5));
}

void Piece::check_squares_move(Chessboard& cb, size_t& i, char& col_e, char& row_e) {   // check the status of the target square and move there if possible
    int j = cb.cr_to_idx(col_e,row_e);
    char clr = cb.idx_to_clr(j);  
    std::vector<Piece> temp_vec = cb.get_all_p();
    std::string new_move = {type, pos_c, pos_r, col_e, row_e};
    Piece p;
    if (j == -1) {  // square is empty, move there
        temp_vec.erase(temp_vec.begin() + i);
        p = Piece(color,col_e,row_e,type);
        update_moves(temp_vec, cb, new_move, p);
    } 
    else if (j != -1 && clr == cb.get_conv4()[color]) {   // opposing piece at target square, capture
        if (i > j) {    // i gets erased first, as to not change index of j
            temp_vec.erase(temp_vec.begin() + i);
            temp_vec.erase(temp_vec.begin() + j);
        }
        else {
            temp_vec.erase(temp_vec.begin() + j);
            temp_vec.erase(temp_vec.begin() + i);
        }
        p = Piece(color,col_e,row_e,type);
        update_moves(temp_vec, cb, new_move, p);
    }
    else {  // friendly piece is blocking square
        std::cout << "Invalid move (Friendly piece is blocking square)." << std::endl;
    }
}

void Piece::pawn_m(Chessboard& cb, std::string &end, size_t &i) {   // covers all possible pawn movements
    char col_e{end[0]}, row_e{end[1]}, temp_pos;  // where to move (row end, column end)
    int col_m{(abs(cb.get_conv1()[col_e] - cb.get_conv1()[pos_c]))}; // move distances in column and row squares on board
    int row_m{(abs(cb.get_conv2()[row_e] - cb.get_conv2()[pos_r]))};
    int j;
    Piece p;
    std::string new_move = {type, pos_c, pos_r, col_e, row_e};
    std::vector<Piece> temp_vec = cb.get_all_p();

    j = cb.cr_to_idx(col_e,row_e);  // check if there is a piece at target square

    if (row_m == 1 && col_m == 0) { // standard move by 1
        if (j == -1) {
            temp_vec.erase(temp_vec.begin() + i);
            p = Piece(color,col_e,row_e,'p');
            update_moves(temp_vec, cb, new_move, p);
        }
        else {
            std:: cout << "Invalid move (Running into piece)." << std::endl;
        }
    }
    else if (row_m == 2 && col_m == 0 && m_list.size() == 0) {   // on first move pawn can move 2 squares
        temp_pos = cb.get_conv2()[cb.get_conv2()[pos_r] + cb.get_conv3()[color]];  // based on color, move up or down the board
        if (j == -1 && cb.cr_to_idx(col_e,temp_pos)) { // check target square and square before that
            temp_vec.erase(temp_vec.begin() + i);
            p = Piece(color,col_e,row_e,'p');
            update_moves(temp_vec, cb, new_move, p);
        } 
        else {
            std:: cout << "Invalid move (Running into piece)." << std::endl;
        }
    }
    else if (row_m == 1 && col_m == 1 && (j != -1 || cb.en_passant(col_e, pos_r))) { // capture diagonally (including en passant)
        if (i > j) {    // i gets erased first, as to not change index of j
            temp_vec.erase(temp_vec.begin() + i);
            temp_vec.erase(temp_vec.begin() + j);
        }
        else {
            temp_vec.erase(temp_vec.begin() + j);
            temp_vec.erase(temp_vec.begin() + i);
        }
        p = Piece(color,col_e,row_e,'p');
        update_moves(temp_vec, cb, new_move, p);

    }
    else {
        std::cout << "Invalid move (Due to rules)." << std::endl;
    }
}

void Piece::king_m(Chessboard& cb, std::string &end, size_t &i) {
    char col_e{end[0]}, row_e{end[1]};  // where to move (row end, column end)
    int col_m{(abs(cb.get_conv1()[col_e] - cb.get_conv1()[pos_c]))}; // move distances in column and row squares on board
    int row_m{(abs(cb.get_conv2()[row_e] - cb.get_conv2()[pos_r]))};
    Piece p;
    std::string new_move = {type, pos_c, pos_r, col_e, row_e};
    if (col_m <= 1 && row_m <= 1) {   // valid king moves are contained here
        check_squares_move(cb, i, col_e, row_e);
    }
    else {
        std::cout << "Invalid move (Due to rules)." << std::endl;
    }
}

void Piece::knight_m(Chessboard& cb, std::string &end, size_t &i) {
    char col_e{end[0]}, row_e{end[1]}, clr;  // where to move (row end, column end)
    int col_m{(abs(cb.get_conv1()[col_e] - cb.get_conv1()[pos_c]))}; // move distances in column and row squares on board
    int row_m{(abs(cb.get_conv2()[row_e] - cb.get_conv2()[pos_r]))};
    int j;
    Piece p;
    std::string new_move = {type, pos_c, pos_r, col_e, row_e};
    std::vector<Piece> temp_vec = cb.get_all_p();
    if ((col_m == 2 && row_m == 1) || (col_m == 1 && row_m == 2)) {
        check_squares_move(cb, i, col_e, row_e);
    }
    else {
        std::cout << "Invalid move (Due to rules)." << std::endl;
    }
}
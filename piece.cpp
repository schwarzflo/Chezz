#include<string>
#include<cmath>
#include <map>
#include <vector>
#include "piece.h"

int sgnm(int);

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
        break;
    case 'b':   // Bishop
        bishop_m(*cb,end,i);
        break;
    case 'r':   // Rook
        rook_m(*cb,end,i);
        break;
    case 'q':   // Queen
        queen_m(*cb,end,i);
        break;
    }
}

void Piece::update_moves(std::vector<Piece>& temp_vec, Chessboard& cb, std::string new_move, Piece p, std::string capture) {   // write move to list and piece
    temp_vec.push_back(p);
    cb.set_all_p(temp_vec);
    cb.add_m(new_move.substr(0,3) + capture + new_move.substr(3,5));
    m_list.push_back(new_move.substr(1,3) + capture + new_move.substr(3,5));
}

void Piece::check_squares_move(Chessboard& cb, size_t& i, char& col_e, char& row_e) {   // check the status of the target square and move there if possible
    int j = cb.cr_to_idx(col_e,row_e);
    char clr{cb.idx_to_clr(j)};
    std::string capture{""};    // writes an "x" in capture moves
    std::vector<Piece> temp_vec = cb.get_all_p();
    std::string new_move = {type, pos_c, pos_r, col_e, row_e};
    Piece p;
    if (j == -1) {  // square is empty, move there
        temp_vec.erase(temp_vec.begin() + i);
        p = Piece(color,col_e,row_e,type);
        update_moves(temp_vec, cb, new_move, p, capture);
    } 
    else if (j != -1 && clr == cb.get_conv4()[color]) {   // opposing piece at target square, capture
        capture = "x";
        if (i > j) {    // i gets erased first, as to not change index of j
            temp_vec.erase(temp_vec.begin() + i);
            temp_vec.erase(temp_vec.begin() + j);
        }
        else {
            temp_vec.erase(temp_vec.begin() + j);
            temp_vec.erase(temp_vec.begin() + i);
        }
        p = Piece(color,col_e,row_e,type);
        update_moves(temp_vec, cb, new_move, p, capture);
    }
    else {  // friendly piece is blocking square
        std::cout << "Invalid move (Friendly piece is blocking square)." << std::endl;
    }
}

void Piece::brq(char col_m, char row_m, Chessboard& cb, size_t& i, std::string& end) {    // used in bishop rook and queen movement functions, to check all squares up to the target square
    int lmt = std::max(abs(col_m),abs(row_m)); 
    char temp_c{pos_c}, temp_r{pos_r}, col_e{end[0]}, row_e{end[1]}; // where to move (row end, column end)
    bool invalid{false};
    for (size_t j{1}; j < lmt; j++) {    // loop does not include the target square
        temp_c = cb.get_conv1()[cb.get_conv1()[temp_c] + sgnm(col_m)];  // shifts temp coordinates according to the direction of the rook move at every step
        temp_r = cb.get_conv2()[cb.get_conv2()[temp_r] + sgnm(row_m)];
        if (cb.cr_to_idx(temp_c,temp_r) != -1) {
            std::cout << "Invalid move (Running into piece)" << std::endl;
            invalid = true;
            break;
        }
    }
    if (!invalid) { // if there where no blocking pieces up to the target position, move is valid
        check_squares_move(cb, i, col_e, row_e);
    }
}

void Piece::pawn_m(Chessboard& cb, std::string &end, size_t &i) {   // covers all possible pawn movements
    char col_e{end[0]}, row_e{end[1]}, temp_pos, p_type;  // where to move (row end, column end); p_type is promotion type
    int col_m{(abs(cb.get_conv1()[col_e] - cb.get_conv1()[pos_c]))}; // move distances in column and row squares on board
    int row_m{(abs(cb.get_conv2()[row_e] - cb.get_conv2()[pos_r]))};
    int j;
    Piece p;
    std::string new_move{type, pos_c, pos_r, col_e, row_e}, capture{""};
    std::vector<Piece> temp_vec = cb.get_all_p();

    j = cb.cr_to_idx(col_e,row_e);  // check if there is a piece at target square

    if (row_m == 1 && col_m == 0) { // standard move by 1
        if (j == -1) {
            temp_vec.erase(temp_vec.begin() + i);
            if (row_e == '8' || row_e == '1') { // promotion rule
                while (true) {
                    std::cout << "Promote to Queen (q), rook (r), knight(n) or bishop(b)?" << std::endl;   
                    std::cin >> p_type;
                    if (p_type != 'q' && p_type != 'r' && p_type != 'n' && p_type != 'b') {
                        std::cout << "Invalid promotion type." << std::endl;
                    }
                    else {
                        p = Piece(color,col_e,row_e,p_type);
                        break;
                    }
                }
            }
            else {
                p = Piece(color,col_e,row_e,'p');
            }
            update_moves(temp_vec, cb, new_move, p, capture);
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
            update_moves(temp_vec, cb, new_move, p, capture);
        } 
        else {
            std:: cout << "Invalid move (Running into piece)." << std::endl;
        }
    }
    else if (row_m == 1 && col_m == 1 && (j != -1 || cb.en_passant(col_e, pos_r))) { // capture diagonally (including en passant)
        capture = "x";
        if (i > j) {    // i gets erased first, as to not change index of j
            temp_vec.erase(temp_vec.begin() + i);
            temp_vec.erase(temp_vec.begin() + j);
        }
        else {
            temp_vec.erase(temp_vec.begin() + j);
            temp_vec.erase(temp_vec.begin() + i);
        }
        p = Piece(color,col_e,row_e,'p');
        update_moves(temp_vec, cb, new_move, p, capture);

    }
    else {
        std::cout << "Invalid pawn move (Due to rules)." << std::endl;
    }
}

void Piece::king_m(Chessboard& cb, std::string &end, size_t &i) {
    char col_e{end[0]}, row_e{end[1]}, r_c, r_r, r_ec, clr{color};  // where to move (row end, column end); rook column, rook row, rook end column
    int col_m{(abs(cb.get_conv1()[col_e] - cb.get_conv1()[pos_c]))}; // move distances in column and row squares on board
    int row_m{(abs(cb.get_conv2()[row_e] - cb.get_conv2()[pos_r]))};
    int j,l;
    bool castling{true};
    Piece p1, p2;
    std::string new_move = {type, pos_c, pos_r, col_e, row_e}, capture{""};
    std::vector<Piece> temp_vec = cb.get_all_p();
    std::vector<char> ctc = {};     // columns to check for castling
    if (col_m <= 1 && row_m <= 1) {   // valid king moves are contained here
        check_squares_move(cb, i, col_e, row_e);
    }
    else if (col_m == 2 && row_m == 0 && m_list.empty()) {    //check for castling possibility
        r_c = cb.get_conv5()[col_e];    // short or long castle
        r_r = cb.get_conv5()[color];
        j = cb.cr_to_idx(r_c,r_r);
        if (temp_vec[j].m_list.empty()) {   // rook has not moved yet
            if (r_c == 'a') {   // long
                ctc = {'b','c','d'};
                r_ec = 'd';
            }
            else {  // short
                ctc = {'f','g'};
                r_ec = 'f';
            } 
            for (size_t k{}; k < ctc.size(); k++) {
                l = cb.cr_to_idx(ctc[k],r_r);
                if (l != -1) {  // a piece is blocking
                    castling = false;
                }
            }
            if (castling) { // castling can go ahead
                if (i > j) {    // i gets erased first, as to not change index of l
                    temp_vec.erase(temp_vec.begin() + i);
                    temp_vec.erase(temp_vec.begin() + j);
                }
                else {
                    temp_vec.erase(temp_vec.begin() + j);
                    temp_vec.erase(temp_vec.begin() + i);
                }
                p1 = Piece(color,col_e,row_e,'k');
                update_moves(temp_vec, cb, new_move, p1, capture);
                p2 = Piece(clr,r_ec,row_e,'r'); // clr instead of color, because due to all_p manip in update_moves, call gets changed!
                temp_vec.push_back(p2);
                cb.set_all_p(temp_vec);
            }
            else {
                std::cout << "Castling failed." << std::endl;
            }
        }
    }
    else {
        std::cout << "Invalid king move (Due to rules)." << std::endl;
    }
}

void Piece::knight_m(Chessboard& cb, std::string &end, size_t &i) {
    char col_e{end[0]}, row_e{end[1]};  // where to move (row end, column end)
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
        std::cout << "Invalid knight move (Due to rules)." << std::endl;
    }
}

void Piece::bishop_m(Chessboard& cb, std::string &end, size_t &i) {
    char col_e{end[0]}, row_e{end[1]}, temp_c{pos_c}, temp_r{pos_r};  // where to move (row end, column end)
    int col_m{cb.get_conv1()[col_e] - cb.get_conv1()[pos_c]}; // move distances in column and row squares on board
    int row_m{cb.get_conv2()[row_e] - cb.get_conv2()[pos_r]};
    bool invalid{false};
    if (abs(col_m) == abs(row_m)) {     // loop does not include the target square
        brq(col_m, row_m, cb, i, end);
    }
    else {
        std::cout << "Invalid bishop move (Due to rules)." << std::endl;
    }
}

void Piece::rook_m(Chessboard& cb, std::string &end, size_t &i) {
    char col_e{end[0]}, row_e{end[1]}, temp_c{pos_c}, temp_r{pos_r};  // where to move (row end, column end)
    int col_m{cb.get_conv1()[col_e] - cb.get_conv1()[pos_c]}; // move distances in column and row squares on board
    int row_m{cb.get_conv2()[row_e] - cb.get_conv2()[pos_r]};
    int lmt;
    bool invalid{false};
    if (col_m == 0 || row_m == 0) {
        brq(col_m, row_m, cb, i, end);
    }
    else {
        std::cout << "Invalid rook move (Due to rules)." << std::endl;
    }
}

void Piece::queen_m(Chessboard& cb, std::string &end, size_t &i) {
    char col_e{end[0]}, row_e{end[1]}, temp_c{pos_c}, temp_r{pos_r};  // where to move (row end, column end)
    int col_m{cb.get_conv1()[col_e] - cb.get_conv1()[pos_c]}; // move distances in column and row squares on board
    int row_m{cb.get_conv2()[row_e] - cb.get_conv2()[pos_r]};
    int lmt;
    bool invalid{false};
    if ((col_m == 0 || row_m == 0) || (abs(col_m) == abs(row_m))) {
        brq(col_m, row_m, cb, i, end);
    }
    else {
        std::cout << "Invalid queen move (Due to rules)." << std::endl;
    }
}

int sgnm(int x) {   // signum function
    int res;
    (x > 0) ? (res = 1) : ((x < 0) ? (res = -1) : (res = 0));
    return res;
}
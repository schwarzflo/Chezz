#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
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
std::vector<std::string> Piece::get_m_list() {
    return m_list;
}
void Piece::set_pos_c(char c) {
    pos_c = c;
}
void Piece::set_pos_r(char r) {
    pos_r = r;
}

bool Piece::move(Chessboard* cb, std::string &end, size_t &i, bool move)  {    // covers all possible piece movements; returns whether move is legal (not based on check)
    switch (type)                                                              // move = true; piece DOES get moved
    {                                                                          // move = false; piece DOES NOT get moved
    case 'p':   // Pawn 
        return pawn_m(*cb,end,i,move);
        break;
    case 'k':   // King
        return king_m(*cb,end,i,move);
        break;
    case 'n':   // Knight
        return knight_m(*cb,end,i,move);
        break;
    case 'b':   // Bishop
        return bishop_m(*cb,end,i,move);
        break;
    case 'r':   // Rook
        return rook_m(*cb,end,i,move);
        break;
    case 'q':   // Queen
        return queen_m(*cb,end,i,move);
        break;
    }
    return false;
}

void Piece::update_moves(size_t i, Chessboard& cb, char col_e, char row_e, std::string capture) {   // write move to list and piece
    std::string new_move = {type, pos_c, pos_r, col_e, row_e};
    cb.set_sp_p(i,col_e,row_e);
    cb.add_m(new_move.substr(0,3) + capture + new_move.substr(3,5));
    m_list.push_back(new_move.substr(1,3) + capture + new_move.substr(3,5));
}

bool Piece::check_squares_move(Chessboard& cb, size_t& i, char& col_e, char& row_e, bool move) {   // check the status of the target square and move there if possible
    int j = cb.cr_to_idx(col_e,row_e);
    char clr{cb.idx_to_clr(j)};
    bool res{false};
    std::string capture{""};    // writes an "x" in capture moves
    if (j == -1) {  // square is empty, move there
        res = true;
        if (move) {
            update_moves(i, cb, col_e, row_e, capture);
        }
    } 
    else if (j != -1 && clr == cb.get_conv4()[color]) {   // opposing piece at target square, capture
        capture = "x";
        res = true;
        if (move) {  // dont execute any of the below if move is false
            if (i > j) {    // i gets changed first, as to not change index of j
                update_moves(i, cb, col_e, row_e, capture);
                cb.erase_idx(j);
            }
            else {
                cb.erase_idx(j);
                update_moves(i, cb, col_e, row_e, capture);
            }
        }
    }
    else {  // friendly piece is blocking square
        //std::cout << "/fpbs/";
    }
    return res;
}

bool Piece::brq(char col_m, char row_m, Chessboard& cb, size_t& i, std::string& end, bool move) {    // used in bishop rook and queen movement functions, to check all squares up to the target square
    int lmt = std::max(abs(col_m),abs(row_m)); 
    char temp_c{pos_c}, temp_r{pos_r}, col_e{end[0]}, row_e{end[1]}; // where to move (row end, column end)
    bool invalid{false};
    for (size_t j{1}; j < lmt; j++) {    // loop does not include the target square
        temp_c = cb.get_conv_c2()[cb.get_conv_c1()[temp_c] + sgnm(col_m)];  // shifts temp coordinates according to the direction of the rook move at every step
        temp_r = cb.get_conv_r2()[cb.get_conv_r1()[temp_r] + sgnm(row_m)];
        if (cb.cr_to_idx(temp_c,temp_r) != -1) {
            //std::cout << "/rip/";
            invalid = true;
            break;
        }
    }
    if (!invalid) { // if there are no blocking pieces up to the target position, move is valid thus far
        return check_squares_move(cb, i, col_e, row_e, move);
    }
    return false;
}

bool Piece::pawn_m(Chessboard& cb, std::string &end, size_t &i,bool move) {   // covers all possible pawn movements
    char col_e{end[0]}, row_e{end[1]}, temp_pos, p_type, clr;  // where to move (row end, column end); p_type is promotion type
    int col_m{cb.get_conv_c1()[col_e] - cb.get_conv_c1()[pos_c]}; // move distances in column and row squares on board
    int row_m{cb.get_conv_r1()[row_e] - cb.get_conv_r1()[pos_r]};
    int j;
    bool res{false}, ep{cb.en_passant(pos_c, pos_r)};
    std::string capture{""};
    j = cb.cr_to_idx(col_e,row_e);  // check if there is a piece at target square
    clr = cb.idx_to_clr(j);

    if (col_m == 0 && ((color == 'w' && row_m == 1) || (color == 'b' && row_m == -1))) { // standard move by 1
        if (j == -1) {
            if (row_e == '8' || row_e == '1') { // promotion rule
                while (true) {
                    std::cout << "Promote to Queen (q), rook (r), knight(n) or bishop(b)?" << std::endl;   
                    std::cin >> p_type;
                    if (p_type != 'q' && p_type != 'r' && p_type != 'n' && p_type != 'b') {
                        std::cout << "Invalid promotion type." << std::endl;
                    }
                    else {
                        type = p_type;
                        break;
                    }
                }
            }
            res = true;
            if (move) {
                update_moves(i, cb, col_e, row_e, capture);
            }
        }
        else {
            //std:: cout << "/rip/";
        }
    }
    else if (abs(row_m) == 2 && col_m == 0 && m_list.size() == 0) {   // on first move pawn can move 2 squares
        temp_pos = cb.get_conv_r2()[cb.get_conv_r1()[pos_r] + cb.get_conv3()[color]];  // based on color, move up or down the board
        if (j == -1 && cb.cr_to_idx(col_e,temp_pos)) { // check target square and square before that
            res = true;
            if (move) {
                update_moves(i, cb, col_e, row_e, capture);
            }
        } 
        else {
            //std:: cout << "/rip/";
        }
    }
    else if (((color == 'w' && row_m == 1 && abs(col_m) == 1) || (color == 'b' && row_m == -1 && abs(col_m) == 1)) && ((j != -1 && clr == cb.get_conv4()[color]) || ep)) { // capture diagonally (including en passant)
        res = true;
        if (move) {  // dont execute any of the below if "only checking whether move is legit"-mode is enabled (aka move is true)
            capture = "x";
            if (ep) {
                j = cb.cr_to_idx(col_e,pos_r);  // j needs to be adjusted in the case of en passant, since the target square is not the square where the opposing pawn resides
            }
            else {
                if (i > j) {    // i gets changed first, as to not change index of j
                    update_moves(i, cb, col_e, row_e, capture);
                    cb.erase_idx(j);
                }   
                else {
                    cb.erase_idx(j);
                    update_moves(i, cb, col_e, row_e, capture);
                }
            }
        }
    }
    else {
        //std::cout << "/ipm/";
    }
    return res;
}

bool Piece::king_m(Chessboard& cb, std::string &end, size_t &i, bool move) {
    char col_e{end[0]}, row_e{end[1]}, r_c, r_r, r_ec, clr{color};  // where to move (row end, column end); rook column, rook row, rook end column
    int col_m{(abs(cb.get_conv_c1()[col_e] - cb.get_conv_c1()[pos_c]))}; // move distances in column and row squares on board
    int row_m{(abs(cb.get_conv_r1()[row_e] - cb.get_conv_r1()[pos_r]))};
    int j,l;
    bool castling{true}, res{false};
    Piece p1, p2;
    std::string new_move = {type, pos_c, pos_r, col_e, row_e}, capture{""};
    std::vector<Piece> temp_vec = cb.get_all_p();
    std::vector<char> ctc = {};     // columns to check for castling
    if (col_m <= 1 && row_m <= 1) {   // valid king moves are contained here
        return check_squares_move(cb, i, col_e, row_e, move);
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
                res = true;
                if (move) {
                    update_moves(i, cb, col_e, row_e, capture);
                    update_moves(j, cb, r_ec, row_e, capture);
                }
            }
            else {
                //std::cout << "Castling failed." << std::endl;
            }
        }
    }
    else {
        //std::cout << "/ikm/";
    }
    return res;
}

bool Piece::knight_m(Chessboard& cb, std::string &end, size_t &i, bool move) {
    char col_e{end[0]}, row_e{end[1]};  // where to move (row end, column end)
    int col_m{(abs(cb.get_conv_c1()[col_e] - cb.get_conv_c1()[pos_c]))}; // move distances in column and row squares on board
    int row_m{(abs(cb.get_conv_r1()[row_e] - cb.get_conv_r1()[pos_r]))};
    int j;
    bool res{false};
    Piece p;
    std::string new_move = {type, pos_c, pos_r, col_e, row_e};
    std::vector<Piece> temp_vec = cb.get_all_p();
    if ((col_m == 2 && row_m == 1) || (col_m == 1 && row_m == 2)) {
        return check_squares_move(cb, i, col_e, row_e, move);
    }
    else {
        //std::cout << "/inm/";
    }
    return res;
}

bool Piece::bishop_m(Chessboard& cb, std::string &end, size_t &i, bool move) {
    char col_e{end[0]}, row_e{end[1]}, temp_c{pos_c}, temp_r{pos_r};  // where to move (row end, column end)
    int col_m{cb.get_conv_c1()[col_e] - cb.get_conv_c1()[pos_c]}; // move distances in column and row squares on board
    int row_m{cb.get_conv_r1()[row_e] - cb.get_conv_r1()[pos_r]};
    bool invalid{false}, res{false};
    if (abs(col_m) == abs(row_m)) {     // loop does not include the target square
        return brq(col_m, row_m, cb, i, end, move);
    }
    else {
        //std::cout << "/ibm/";
    }
    return res;
}

bool Piece::rook_m(Chessboard& cb, std::string &end, size_t &i, bool move) {
    char col_e{end[0]}, row_e{end[1]}, temp_c{pos_c}, temp_r{pos_r};  // where to move (row end, column end)
    int col_m{cb.get_conv_c1()[col_e] - cb.get_conv_c1()[pos_c]}; // move distances in column and row squares on board
    int row_m{cb.get_conv_r1()[row_e] - cb.get_conv_r1()[pos_r]};
    int lmt;
    bool invalid{false}, res{false};
    if (col_m == 0 || row_m == 0) {
        return brq(col_m, row_m, cb, i, end, move);
    }
    else {
        //std::cout << "/irm/";
    }
    return res;
}

bool Piece::queen_m(Chessboard& cb, std::string &end, size_t &i, bool move) {
    char col_e{end[0]}, row_e{end[1]}, temp_c{pos_c}, temp_r{pos_r};  // where to move (row end, column end)
    int col_m{cb.get_conv_c1()[col_e] - cb.get_conv_c1()[pos_c]}; // move distances in column and row squares on board
    int row_m{cb.get_conv_r1()[row_e] - cb.get_conv_r1()[pos_r]};
    int lmt;
    bool invalid{false}, res{false};
    if ((col_m == 0 || row_m == 0) || (abs(col_m) == abs(row_m))) {
        return brq(col_m, row_m, cb, i, end, move);
    }
    else {
        //std::cout << "/iqm/";
    }
    return res;
}

int sgnm(int x) {   // signum function
    int res;
    (x > 0) ? (res = 1) : ((x < 0) ? (res = -1) : (res = 0));
    return res;
}

std::ostream& operator<<(std::ostream& os, Piece p) {
    
    os << "Piece: " << p.type << p.pos_c << p.pos_r << " " << p.color << std::endl;

    return os;
}
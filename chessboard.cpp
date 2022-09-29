#include "chessboard.h"

bool input_valid(std::string, std::string);
bool in_list(char, std::vector<char>);
bool in_check(std::string, std::string, size_t);

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
    game_ongoing = true;
    conv_c1 = {
        { 'a', 1 },{ 'b', 2 },{ 'c', 3 },{ 'd', 4 },{ 'e', 5 },{ 'f', 6 },{ 'g', 7 },{ 'h', 8 },
    };
    conv_c2 = {
        { 1, 'a' },{ 2, 'b' },{ 3, 'c' },{ 4, 'd' },{ 5, 'e' },{ 6, 'f' },{ 7, 'g' },{ 8, 'h' }
    };
    conv_r1 = {
        { '1', 1 },{ '2', 2 },{ '3', 3 },{ '4', 4 },{ '5', 5 },{ '6', 6 },{ '7', 7 },{ '8', 8 },
    };
    conv_r2 = {
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

int Chessboard::cr_to_idx(char nc, char nr) {   // return index in list of pieces for input of coordinates, returns -1 if no piece at coordinates
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

std::string Chessboard::get_king_pos(char clr) {
    for (auto p: all_p) {
        if (p.get_color() == clr && p.get_type() == 'k') {
            return {p.get_pos_c(),p.get_pos_r()};
        }
    }
    return "";
}

void Chessboard::move(std::string start, std::string end, Playing* pl) {
    bool movable{false}, checking{false}, found{false}, dummy;
    char col{start[0]}, row{start[1]};
    size_t k;
    Piece p1;
    std::string king_pos;
    if (!input_valid(start,end)) {
        //std::cout << "Invalid input." << std::endl;
        return;
    }
    for (size_t i{}; i < all_p.size(); i++) {   // check whether a piece is on the start position
        if (all_p[i].get_pos_c() == col && all_p[i].get_pos_r() == row && color == all_p[i].get_color()) { // check for position and valid color
            found = true;
            checking = in_check(start,end,i,color);
            if (!checking) {
                movable = all_p[i].move(this,end,i,true); // try to move that piece; return whether movable
            }
            else {
                //std::cout << "/kic/" << std::endl;
            }
            break;
        }
    }
    if (!found) {
        std::cout << "No valid piece at this position." << std::endl;
    }
    if (movable && !checking) { // i can move the piece like i want, and my move does not put me in check
        if (pl->get_human_pl() != -1) {
            //std::cout << *this << std::endl; // print moves to console
            visualize_board();
        }
        if (check_mate()) {
            std::cout << color << " has delivered check mate to " << conv4[color] << " in " << move_nr << " moves. Game ends." << std::endl;
            game_ongoing = false;
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
}

bool Chessboard::en_passant(char col, char row) { // check if the last move was a pawn moving up (or down) two squares on a given column, in order to make en passant possible
    if (all_m.empty()) {
        return false;
    }
    std::string last_move = all_m.back();
    if (last_move[0] == 'p' && last_move[4] == row && abs(conv_c1[col] - conv_c1[last_move[3]]) == 1 && abs(conv_c1[last_move[2]] - conv_c1[last_move[4]]) == 2) {
        return true;
    }
    return false;
}

bool Chessboard::in_check(std::string start, std::string end, size_t i, char clr) {
    int k{cr_to_idx(end[0],end[1])};
    std::string king_pos;
    bool in_check{false};
    all_p[i].set_pos_c(end[0]); // assume the move to happen [1]
    all_p[i].set_pos_r(end[1]);
    king_pos = get_king_pos(clr);
    if (k != -1) {  // if there is a piece at the position, temporarily remove it from the board [2], by setting its position to off board
        all_p[k].set_pos_c('-');
        all_p[k].set_pos_r('-');
    }
    for (auto p: all_p) {   // attempt to capture the king with all opposing pieces
        if (p.get_color() == get_conv4()[clr]) {    
            if (p.move(this,king_pos,i,false)) {  // capture of king is successful: move is therefore invalid and will be rejected
                //std::cout << "Checked by " << p;
                in_check = true; 
                break;
            }
        }
    }
    all_p[i].set_pos_c(start[0]);  // reset [1], since the move failed
    all_p[i].set_pos_r(start[1]); 
    if (k != -1) {
        all_p[k].set_pos_c(end[0]);
        all_p[k].set_pos_r(end[1]); // reset [2], i.e. add the temporarily removed piece again
    }
    return in_check;
}

bool Chessboard::check_mate() { 
    std::string end, start;
    int tempc, tempr;
    char col, row;
    bool cm{true};
    for (size_t i{}; i < all_p.size(); i++) {
        if (all_p[i].get_color() == conv4[color]) {     // interested in all pieces of the (potentially) losing side
            start = {all_p[i].get_pos_c(),all_p[i].get_pos_r()};
            //std::cout << all_p[i].get_type() << all_p[i].get_pos_c() << all_p[i].get_pos_r() << std::endl;
            for (int c{1}; c <= 8; c++) {   // interested in all legal moves these pieces could make
                for (int r{1}; r <= 8; r++) {
                    end = {conv_c2[c],conv_r2[r]};
                    if (all_p[i].move(this,end,i,false)) {  // legality of move
                        //std::cout << "Attempt " << end << std::endl;
                        if (!in_check(start,end,i,conv4[color])) {  // does the (theoretically legal) move threaten my own king?
                            cm = false;
                            return cm;
                        }
                    }
                }
            }
        }
    }
    return cm;
};

void Chessboard::add_m(std::string new_move) {
    all_m.push_back(new_move);
};

void Chessboard::visualize_board() {
    bool found;
    char type,clr;
    for (size_t r{8}; r >= 1; r--) {
        for (size_t c{1}; c <= 8; c++) {
            found = false;
            for (auto piece: all_p) {
                if (piece.get_pos_c() == conv_c2[c] && piece.get_pos_r() == conv_r2[r]) {
                    found = true;
                    type = piece.get_type();
                    clr = piece.get_color();
                    break;
                }
            }
            if (found) {
                std::cout << type << clr << " ";
                found = false;
            }
            else {
                std::cout << "--" << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<Piece> Chessboard::get_all_p() {
    return all_p;
}

void Chessboard::set_all_p(std::vector<Piece> nap) {
    all_p = nap;
}

void Chessboard::set_sp_p(size_t i, char col, char row) {  // set a specific entry (i) of all_p to a different piece
    all_p[i].set_pos_c(col);
    all_p[i].set_pos_r(row);
}

void Chessboard::erase_idx(size_t i) {
    all_p.erase(all_p.begin()+i);
}

int Chessboard::get_move_nr() {
    return move_nr;
}

char Chessboard::get_color() {
    return color;
}

std::map<char, int> Chessboard::get_conv_c1() {
    return conv_c1;
}

std::map<int, char> Chessboard::get_conv_c2() {
    return conv_c2;
}

std::map<char, int> Chessboard::get_conv_r1() {
    return conv_r1;
}

std::map<int, char> Chessboard::get_conv_r2() {
    return conv_r2;
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

bool Chessboard::get_game_status() {
    return game_ongoing;
}


bool in_list(char in, std::vector<char> list) {
    for (auto item: list) {
        if (item == in) {
            return true;
        }
    }
    return false;
}

bool input_valid(std::string start,std::string end) {
    std::vector<char> list_a = {'a','b','c','d','e','f','g','h'};
    std::vector<char> list_b = {'1','2','3','4','5','6','7','8'};
    if (!in_list(start[0], list_a) || !in_list(start[1], list_b) || !in_list(end[0], list_a) || !in_list(end[1], list_b)) {
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, Chessboard cs) {
    for (auto piece : cs.all_p) {
        os << piece.get_type() << piece.get_pos_c() << piece.get_pos_r() << " " << piece.get_color() << "   ";
    };
    os << std::endl;
    for (auto move : cs.all_m) {
        os << move << "   ";
    };
    os << std::endl;
    return os;
}

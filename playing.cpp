#include "playing.h"
#include "chessboard.h"
#include <string>
#include <random>

Playing::Playing(int h_pl, int c_pl) {  // human vs human; comp vs comp;
    human_pl = h_pl;
    comp_pl = c_pl;
}
Playing::Playing(char cc) { // if computer color is chosen, it is save to assume it will be human vs comp
    comp_clr = cc;
    human_pl = 1;
    comp_pl = 1;
}
int Playing::get_human_pl() {
    return human_pl;
}


void Playing::play() {
    Chessboard cb;
    while (true) {
        if (human_pl == 2 && comp_pl == 0) {
            human_choice(cb);
            if (!cb.get_game_status()) {
                break;
            }
        }
        else if (human_pl == 1 && comp_pl == 1) {
            if (cb.get_color() == comp_clr) {
                computer_choice(cb);
            }
            else {
                human_choice(cb);
            }
            if (!cb.get_game_status()) {
                break;
            }
        }
        else {
            computer_choice(cb);
            if (!cb.get_game_status()) {
                break;
            }
        }
    }
}

void Playing::human_choice(Chessboard& cb) {
    std::string choice;
    std::cout << cb.get_color() << " to play ('{FROM} {TO}')." << std::endl;
    std::getline(std::cin, choice);
    cb.move(choice.substr(0,2),choice.substr(3,2),this);
}

void Playing::computer_choice(Chessboard& cb) {
    std::string choice, end, start;
    std::vector<Piece> temp{cb.get_all_p()};
    bool valid, c_piece{true}, c_square;
    size_t l{temp.size()}, idx, count;
    int r,c;
    srand(time(NULL));
    while (c_piece) {
        c_square = true;
        count = 0;
        idx = rand()%l;
        if (temp[idx].get_color() == cb.get_color()) {
            while (c_square && count <= 20) {   // only try 20 different moves a piece
                c = rand()%8 + 1;
                r = rand()%8 + 1;
                start = {temp[idx].get_pos_c(),temp[idx].get_pos_r()};
                end = {cb.get_conv_c2()[c],cb.get_conv_r2()[r]};
                valid = temp[idx].move(&cb,end,idx,false);  // checks whether bot move is legal
                if (valid) {
                    start = {temp[idx].get_pos_c(),temp[idx].get_pos_r()};
                    choice = start + " " + end;
                    c_square = false;
                    c_piece = false;
                }
                count++;
            }
        }
    } 
    cb.move(start,end,this);
}
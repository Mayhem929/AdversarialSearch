#include "Piece.h"

Piece::Piece(color c, Box b, special_type t, int t_l){
    this->c = c;
    box = b;
    type = t;
    turns_left = t_l;
}

Piece::Piece(color c, Box b){
    this->c = c;
    box = b;
    type = normal_piece;
    turns_left = 0;
}

bool Piece::operator==(const Piece & p) const{
    return (c == p.c && box == p.box && type == p.type && turns_left == p.turns_left);
}

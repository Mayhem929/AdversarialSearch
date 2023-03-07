#ifndef __PIECE_H__
#define __PIECE_H__

# include <vector>
# include <map>
# include "Attributes.h"

using namespace std;


enum special_type {star_piece, boo_piece, none_piece};

class Piece{
    private:
        color c;
        Box box;
        special_type type;
        int turns_left;

    public:
        Piece(color c, Box b, special_type t, int t_l);

        Piece(color c, Box b);

        inline color get_color() const{
            return c;
        };

        inline const Box get_box() const{
            return box;
        };

        inline special_type get_type() const{
            return type;
        };

        inline int get_turns_left() const{
            return turns_left;
        };

        inline void set_color(color c){
            this->c = c;
        };

        inline void set_box(Box b){
            box = b;
        };

        inline void set_type(special_type t){
            type = t;
        };

        inline void set_turns_left(int t_l){
            turns_left = t_l;
        };

        bool operator==(const Piece & p) const;

};


#endif
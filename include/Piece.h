#ifndef __PIECE_H__
#define __PIECE_H__

# include <vector>
# include <map>
# include "Attributes.h"

using namespace std;

//Enumerado de los diferentes tipos de ficha 
enum special_type {star_piece, boo_piece, mega_piece, small_piece, normal_piece, bananed_piece};


class Piece{
    private:
        //Color de la ficha
        color c;
        //Casilla en la que se encuentra la ficha
        Box box;
        //Tipo de ficha
        special_type type;
        //Turnos restantes que le quedan a la ficha siendo del tipo "type"
        int turns_left;

    public:
        /**
         * @brief Constructor de un nuevo objeto Piece
         * 
         * @param c color
         * @param b casilla
         * @param t tipo
         * @param t_l turnos restantes
         */
        Piece(color c, Box b, special_type t, int t_l);

        /**
         * @brief Constructor de un nuevo objeto Piece de tipo normal (por defecto)
         * 
         * @param c color
         * @param b casilla
         */
        Piece(color c, Box b);

        /**
         * @brief Devuelve el color de la ficha
         * 
         * @return color 
         */
        inline color get_color() const{
            return c;
        };

        /**
         * @brief Devuelve la casilla en la que está la ficha
         * 
         * @return const Box 
         */
        inline const Box get_box() const{
            return box;
        };

        /**
         * @brief Devuelve el tipo de la ficha
         * 
         * @return special_type 
         */
        inline special_type get_type() const{
            return type;
        };

        /**
         * @brief Devuelve los turnos restantes que le quedan a la ficha siendo del tipo "type"
         * 
         * @return int 
         */
        inline int get_turns_left() const{
            return turns_left;
        };

        /**
         * @brief Actualiza el color de la ficha con el parámetro c
         * 
         * @param c 
         */
        inline void set_color(color c){
            this->c = c;
        };

        /**
         * @brief Actualiza la casilla de la ficha con el parámetro b
         * 
         * @param b 
         */
        inline void set_box(Box b){
            box = b;
        };

        /**
         * @brief Actualiza el tipo de la ficha con el parámetro t
         * 
         * @param t 
         */
        inline void set_type(special_type t){
            type = t;
        };

        /**
         * @brief Actualiza los turnos restantes de la ficha con el parámetro t_l
         * 
         * @param t_l 
         */
        inline void set_turns_left(int t_l){
            turns_left = t_l;
        };

        /**
         * @brief Sobrecarga del operador ==
         * Se comprueban las 4 variables que definen el struct.
         * 
         * @param p 
         * @return true 
         * @return false 
         */
        bool operator==(const Piece & p) const;

};


#endif
#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

# include <vector>
# include <map>
# include <string>

using namespace std;

//Enumerado con los diferentes colores: Azul, rojo, verde, amarillo y ninguno.
enum color {blue, red, green, yellow, none};

/**
 * @brief Conversión de un objeto de la clase color a su correspondiente string.
 *
 * @param c
 * @return string
 */
inline string str(color c){
    switch(c){
        case blue: return "Azul";
        case red: return "Rojo";
        case green: return "Verde";
        case yellow: return "Amarillo";
        case none: default: return "???";
    }
}

/**
 * @brief Identificación de un color con su compañero
 *
 * @param c
 * @return string
 */
inline color partner_color(color c){
    switch(c){
        case blue: return green;
        case green: return blue;
        case red: return yellow;
        case yellow: return red;
        default: return none;
    }
}

/**
 * @brief Identificación de los colores del oponente
 *
 * @param c
 * @return string
 */
inline color opponent_color(color c){
    switch(c){
        case yellow: return blue;
        case red: return blue;
        case blue: return yellow;
        case green: return yellow;
        default: return none;
    }
}

//Enumerado con los diferentes tipos de casillas
enum box_type {normal, home, final_queue, goal};

//Struct para definir las casillas: número de casilla, tipo y color.
struct Box
{
    //Número de casilla:
    //{1, 2, ..., 68} para casillas normales (normal)
    //{1, 2, ..., 7} para casillas del pasillo a la meta (final_queue)
    //0 en caso contrario
    int num;
    //Tipo de la casilla
    box_type type;
    //Color de la casilla
    color col;

    /**
     * @brief Constructor de un nuevo objeto Box
     *
     * @param num
     * @param type
     * @param col
     */
    inline Box(int num, box_type type, color col){
        this->num = num; this->type = type; this->col = col;
    }

    /**
     * @brief Constructor por defecto de un objeto Box
     *
     */
    inline Box(){};
};


/**
 * @brief Sobrecarga del operador <
 * Se comprueban una a una las 3 variables que definen el struct en orden
 * lexicográfico.
 *
 * @param a Primer elemento
 * @param b Segundo elemento
 * @return true
 * @return false
 */
inline bool operator <(const Box & a, const Box & b){
    return a.num > b.num or (a.num == b.num and a.type > b.type) or
           (a.num == b.num and a.type == b.type and a.col > b.col);
}

/**
 * @brief Sobrecarga del operador ==
 * Se comprueban las 3 variables que definen el struct.
 *
 * @param a
 * @param b
 * @return true
 * @return false
 */
inline bool operator ==(const Box & a, const Box & b){
    return a.num == b.num && a.type == b.type && a.col == b.col;
}

//Enumerado de los diferentes tipos de objetos speciales que aparecen en el tablero.
enum item_type {star = 101,
                boo = 102,
                bullet = 103,
                red_shell = 104,
                blue_shell = 105,
                mushroom = 106,
                mega_mushroom = 107,
                shock = 108,
                horn = 109,
                banana = 110,
                not_an_item = -1};

/**
 * @brief Struct para definir los objetos especiales que aparecen en el tablero.
 * 
 */
struct SpecialItem
{
    //Tipo de objeto especial
    item_type type;
    //Casilla en la que se encuentra el objeto especial
    Box box;
};

#endif
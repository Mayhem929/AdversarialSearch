#ifndef __PIECESPRITE_H__
#define __PIECESPRITE_H__

#include "ClickableSprite.h"
#include "IncludesSFML.h"
#include "Attributes.h"
#include "Piece.h"
// #include "ParchisGUI.h"

using namespace std;

class PieceSprite: public ClickableSprite{
    private:
        int id;  // Deprecated (no se usa) pero no se puede borrar porque se usa en el constructor. ¿Hacer un constructor por defecto? ¿Hacer un constructor con parámetros? ¿Hacer un constructor con parámetros y un constructor por defecto? ¿Hacer un constructor con parámetros y un constructor por defecto y un constructor de copia? ¿Hacer un constructor con parámetros y un constructor por defecto y un constructor de copia y un constructor de movimiento?
        color c; // Deprecated (no se usa) pero no se puede borrar porque se usa en el constructor. ¿Hacer un constructor por defecto? ¿Hacer un constructor con parámetros? ¿Hacer un constructor con parámetros y un constructor por defecto? ¿Hacer un constructor con parámetros y un constructor por defecto y un constructor de copia? ¿Hacer un constructor con parámetros y un constructor por defecto y un constructor de copia y un constructor de movimiento?
        // TODO: Ahora es Piece también. ¿Hacer un puntero? ¿Hacer una copia? ¿Hacer una referencia? ¿Hacer una referencia constante? ¿Hacer una referencia constante y un puntero? ¿Hacer una referencia constante y una copia?
        const Piece * piece; // Va a tener que ser puntero para que se actualice conforme al estado de la ficha creo. ¿Hacer una referencia? ¿Hacer una referencia constante? ¿Hacer una referencia constante y un puntero? ¿Hacer una referencia constante y una copia? ¿Hacer una referencia constante y una copia y un puntero? ¿Hacer una referencia constante y una copia y un puntero y una referencia?

        static const map <color, vector<int>> col2textrec;

        static const map <color, vector<int>> col2selectedrec;

    public:

        PieceSprite(const Texture & t, int id, const Piece & piece);
        PieceSprite(const Texture & t, int id, color c);

        inline int getId(){ return this->id;}
        inline color getModelColor(){return this->c;}

        void onClickAction(Window & container);
        void onEnableAction(Window & container);
        void onSelectionAction(Window & container);
        void onHoverAction(Window & container);
        void onLockAction(Window & container);

        inline const Piece & getPiece() const{
            return *(this->piece);
        }
};

#endif
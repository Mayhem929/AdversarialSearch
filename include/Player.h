#ifndef __PLAYER_H__
#define __PLAYER_H__

# include "Attributes.h"

class Parchis;

class Player{
    protected:
        //Nombre del jugador
        string name;
        //Entero representativo del jugador en el juego
        int jugador;
        //Colores que puede mover
        vector<color> colors;

        //Referencia a Parchis
        Parchis *actual;
    public:
        /**
         * @brief Función abstracta que define la actualiza *actual y asigna al jugador como jugador acutal.
         * 
         * @param p 
         */
        virtual void perceive(Parchis & p);

        /**
         * @brief Constructor de la clase Player
         * 
         * @param name 
         */
        inline Player(const string & name){this->name = name; actual = NULL;}

        /**
         * @brief Devuelve el nombre del jugador
         * 
         * @return string name
         */
        inline string getName() const{return name;}

        /**
         * @brief Devuelve el id jugador
         * 
         * @return int jugador
         */
        inline int getJugador() const{return jugador;}

        /**
         * @brief Función abstracta que define el movimiento devuelto por el jugador.
         * 
         * @param dice 
         * @param id_piece 
         * @param c_piece 
         * @return true 
         * @return false 
         */
        virtual bool move() = 0;

        /**
         * @brief Función abstracta que devuelve si el jugador puede usar o no la GUI.
         * 
         * @return true 
         * @return false 
         */
        inline virtual bool canUseGUI() const{return false;}


        /**
         * @brief Función abstracta que devuelve si el jugador usa alguna IA para la toma de decisiones o no.
         * 
         * @return true 
         * @return false 
         */
        inline virtual bool canThink() const{return false;}

        /**
         * @brief Función abstracta que devuelve si el jugador es remoto o no.
         * 
         * @return true 
         * @return false 
         */
        inline virtual bool isRemote() const{return false;}

        /**
         * @brief Función abstracta que devuelve si el jugador está listo para el siguiente turno o no.
         * 
         * @return true 
         * @return false 
         */
        inline virtual bool readyForNextTurn(){return true;}

};
#endif
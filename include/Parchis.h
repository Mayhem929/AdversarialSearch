#ifndef __PARCHIS_H__
#define __PARCHIS_H__

# include <tuple>
# include "Dice.h"
# include "Board.h"
# include "GUIPlayer.h"
# include "Player.h"
# include "AIPlayer.h"
#include <iostream>
#include <algorithm>
#include <memory>

// Macro que define el parámetro para indicar que no se mueve ficha.
#define SKIP_TURN -9999

using namespace std;

class Player;

class ParchisBros{
    //Clase que almacena los hijos (bros) de un nodo de Parchis (parent).
    private:
        //Nodo padre del cual se generan los hijos.
        shared_ptr<Parchis> parent;
    public:
        //Clase iteradora para iterar sobre los hijos de parent.
        class Iterator{
            private:
                //Puntero al nodo actual.
                shared_ptr<Parchis> actual;
                //Último movimiento realizado (color).
                color last_c_piece;
                //Último movimiento realizado (id de la ficha).
                int last_id_piece;
                //Último dado utilizado (índice).
                int last_dice;
                //Último valor del dado utilizado.
                int dice_value;

                //Puntero al contenedor.
                ParchisBros * container;

            public:
                /**
                 * @brief Constructor de un nuevo objeto de Iterator.
                 *
                 * @param container
                 * @param p
                 * @param last_c_piece
                 * @param last_id_piece
                 * @param last_dice
                 */
                Iterator(ParchisBros & container, shared_ptr<Parchis> &p, color last_c_piece = none, int last_id_piece = -1, int last_dice = -1);

                /**
                 * @brief Método que devuelve el siguiente hijo del nodo actual.
                 *
                 * @return Iterator
                 */
                Iterator & operator++();

                /**
                 * @brief Sobrecarga del operador== para comparar dos iteradores.
                 *
                 * @return true
                 * @return false
                 */
                bool operator==(const Iterator & it) const;

                /**
                 * @brief Sobrecarga del operador!= para comparar dos iteradores.
                 *
                 * @return true
                 * @return false
                 */
                bool operator!=(const Iterator & it) const;

                /**
                 * @brief Sobrecarga del operador* para devolver el puntero al nodo actual.
                 *
                 * @return Parchis&
                 */
                Parchis & operator*() const;

                /**
                 * @brief Método que devuelve el valor del último dado utilizado.
                 *
                 * @return dice_value
                 */
                inline int getMovedDiceValue() const {return dice_value;}

                /**
                 * @brief Método que devuelve el color de la última ficha movida.
                 *
                 * @return last_c_piece
                 */
                inline color getMovedColor() const {return last_c_piece;}

                /**
                 * @brief Método que devuelve el id de la última ficha movida.
                 *
                 * @return last_id_piece
                 */
                inline int getMovedPieceId() const {return last_id_piece;}
        };

        /**
         * @brief Constructor de un nuevo objeto de ParchisBros.
         *
         * @param p
         */
        ParchisBros(const Parchis & p);

        /**
         * @brief Método que devuelve un iterador al primer hijo del nodo.
         *
         * @return Iterator
         */
        Iterator begin();

        /**
         * @brief Método que devuelve un iterador al último hijo del nodo.
         *
         * @return Iterator
         */
        Iterator end();


};

class Parchis{
    private:
        //ParchisBros clase amiga
        friend class ParchisBros;

        //Tablero
        Board board;
        //Dados
        Dice dice;

        static const int MAX_SPECIAL_DICES = 2;

        //Variables para almacenar los últimos movimientos
        //Últimos movimientos identificados por el color.
        vector<tuple <color, int, Box, Box>> last_moves;
        //Última acción identificada por el color.
        tuple <color, int, int> last_action;
        //Último dado utilizado.
        int last_dice;

        //Turno actual
        int turn;

        //Jugadores y colorores actuales
        //0: yellow & red, 1: blue and green.
        int current_player;
        color current_color;
        //Vector de jugadores en juego.
        vector <shared_ptr<Player>> players;
        //Vector de viewers (pueden observar la partida pero no intervenir en ella).
        vector <shared_ptr<Player>> viewers;

        //Variables para controlar acciones de los jugadores
        int illegal_move_player;
        int disconnected_player;

        //Booleanos para recordar movimientos especiales
        bool eating_move;
        bool goal_move;
        bool goal_bounce;
        bool remember_6;
        bool bananed;

        pair <color, int> eaten_piece;

        bool red_shell_move;
        bool blue_shell_move;
        bool star_move;
        bool bullet_move;
        bool horn_move;
        bool shock_move;
        bool boo_move;
        bool mega_mushroom_move;
        bool mushroom_move;
        bool banana_move;

        vector <pair <color, int>> pieces_destroyed_by_star;
        vector <pair <color, int>> pieces_crushed_by_megamushroom;
        vector <pair <color, int>> pieces_destroyed_by_red_shell;
        vector <pair <color, int>> pieces_destroyed_by_blue_shell;
        vector <pair <color, int>> pieces_destroyed_by_horn;

        item_type last_acquired;


        //Definición de casillas especiales
        static const int final_red_box = 34;
        static const int final_blue_box = 17;
        static const int final_green_box = 51;
        static const int final_yellow_box = 68;

        static const int init_red_box = 38;
        static const int init_blue_box = 21;
        static const int init_green_box = 55;
        static const int init_yellow_box = 4;

        //Vector de casillas seguras.
        static const vector<int> safe_boxes;

        //Vectores de casillas finales e iniciales identificadas por cada color.
        static const map<color, int> final_boxes;
        static const map<color, int> init_boxes;

        // Control de rebotes (para evitar partidas infinitas)
        map<color, int> bounces;
        int overbounce_player;

        // Variables de control para sincronización con GUI.
        volatile bool update_board;
        volatile bool update_dice;

        // Indica si se está jugando una partida creativa.
        bool playground_mode;

        /**
         * @brief Método que gestiona el cambio de turno modificando las variables
         * current_player y current_color.
         *
         */
        void nextTurn();

        /**
         * @brief Método que inicializa las partes comunes en los constructores del juego.
         *
         */
        void initGame();

        /**
         * @brief Función que genera el siguiente movimiento siguiendo un orden
         * descendente de los dados.
         *
         * Estos métodos funcionan de la siguiente forma. Dado un estado del juego, a partir de los parámetros
         * de color, id de ficha y dado que se le pasen por referencia, asociados a un determinado movimiento
         * en el tablero, determinará el siguiente hijo que se expandirá en el árbol de búsqueda.
         * Los parámetros se actualizarán de forma que se correspondan con el movimiento necesario para generar
         * el nuevo hijo desarrollado. Inicialmente, para generar el primer hijo de una ramificación, se deben
         * pasar los parámetros inicializados a -1.
         *
         * @param c_piece
         * @param id_piece
         * @param dice
         * @return Parchis
         */
        Parchis generateNextMoveDescending(color & c_piece,  int & id_piece, int & dice) const;

    public:
        /**
         * @brief Vector constante de los colores del juego.
         *
         */
        static const vector<color> game_colors;

        /****************** CONSTRUCTORES ******************/


        /**
         * @brief Constructor por defecto de un nuevo objeto de Parchis.
         * @deprecated Los jugadores se deben pasar desde fuera.
         *
         */
        Parchis();

        /**
         * @brief Constructor de un nuevo objeto de Parchis.
         * @deprecated Los jugadores se deben pasar desde fuera.
         *
         * @param b
         */
        Parchis(const BoardConfig &b);

        /**
         * @brief Constructor de un nuevo objeto de Parchis.
         * @deprecated Los jugadores se deben pasar desde fuera.
         *
         * @param b
         * @param d
         */
        Parchis(const Board & b, const Dice & d);

        /**
         * @brief Constructor de un nuevo objeto de Parchis.
         * @deprecated Los jugadores se deben pasar desde fuera.
         *
         * @param b
         * @param d
         */
        Parchis(const BoardConfig & b, const Dice & d);

        /**
         * @brief Constructor de un nuevo objeto de Parchis.
         *
         * @param b
         * @param d
         * @param p1
         * @param p2
         */
        Parchis(const Board & b, const Dice & d, Player & p1, Player & p2);

        /**
         * @brief Constructor de un nuevo objeto de Parchis.
         *
         * @param b
         * @param d
         * @param p1
         * @param p2
         */
        Parchis(const BoardConfig &b, const Dice &d, shared_ptr<Player> p1,  shared_ptr<Player> p2);

        /**
         * @brief Constructor de un nuevo objeto de Parchis.
         *
         * @param b
         * @param p1
         * @param p2
         */
        Parchis(const BoardConfig &b, shared_ptr<Player> p1,  shared_ptr<Player> p2);

        /****************** GETTERS ******************/

        /**
         * @brief Función que devuelve el atribute dice.
         *
         * @param player
         * @return const vector<int>&
         */
        const Dice & getDice () const;


         /**
         * @brief Función que devuelve el attributo board.
         *
         * @param player
         * @return const vector<int>&
         */
        const Board & getBoard () const;

        /**
         * @brief Función que devuelve el atibuto last_action.
         *
         * @return int
         */
        inline const tuple <color, int, int> & getLastAction() const{
            return last_action;
        }

        /**
         * @brief Función que devuelve el atributo turn
         *
         * @return const int
         */
        inline const int getTurn() const{
            return turn;
        }

        /**
         * @brief Función que devuelve el atributo last_dice.
         *
         * @return int
         */
        int getLastDice() const;

        /**
         * @brief Función que devuelve el valor del atributo eating_move
         *
         * @return true
         * @return false
         */
        inline const bool isEatingMove() const {
            return eating_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo goal_move
         *
         * @return true
         * @return false
         */
        inline const bool isGoalMove() const {
            return goal_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo goal_bounce
         *
         * @return true
         * @return false
         */
        inline const bool goalBounce() const{
            return goal_bounce;
        }

        /**
         * @brief Función que devuelve el valor del atributo red_shell_move
         *
         * @return true
         * @return false
         */
        inline const bool isRedShellMove() const{
            return red_shell_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo horn_move
         *
         * @return true
         * @return false
         */
        inline const bool isHornMove() const{
            return horn_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo blue_shell_move
         *
         * @return true
         * @return false
         */
        inline const bool isBlueShellMove() const{
            return blue_shell_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo star_move
         *
         * @return true
         * @return false
         */
        inline const bool isStarMove() const{
            return star_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo bullet_move
         *
         * @return true
         * @return false
         */
        inline const bool isBulletMove() const{
            return bullet_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo shock_move
         *
         */
        inline const bool isShockMove() const{
            return shock_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo mega_mushroom_move
         *
         */
        inline const bool isMegaMushroomMove() const{
            return mega_mushroom_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo mushroom_move
         *
         */
        inline const bool isMushroomMove() const{
            return mushroom_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo boo_move
         *
         */
        inline const bool isBooMove() const{
            return boo_move;
        }

        /**
         * @brief Función que devuelve el valor del atributo banana_move
         *
         */
        inline const bool isBananaMove() const{
            return banana_move;
        }

        /**
         * @brief Función que devuelve el id del jugador actual.
         *
         * @return int
         */
        inline int getCurrentPlayerId() const{
            return current_player;
        }

        /**
         * @brief Función que devuelve una referencia al jugador actual.
         *
         * @return Player&
         */
        inline Player & getCurrentPlayer(){
            return *players[current_player];
        }

        /**
         * @brief Función que devuelve el vector de jugadores.
         *
         * @return vector<shared_ptr<Player>>&
         */
        inline vector<shared_ptr<Player>> & getPlayers(){
            return players;
        }

        /**
         * @brief Función que devuelve el atributo current_color.
         *
         * @return color
         */
        inline color getCurrentColor() const{
            return this->current_color;
        }

        /**
         * @brief Función que devuelve el atributo last_moves.
         *
         * @return const vector<tuple <color, int, Box>>&
         */
        const vector<tuple <color, int, Box, Box>> & getLastMoves() const;

        /**
         * @brief Get the Children object
         *
         * @return ParchisBros
         */
        ParchisBros getChildren() const;

        /**
         * @brief Get the Initial Box object
         * 
         * @param c 
         * @return const Box 
         */
        inline const Box getInitialBox(color c) const{
            return Box(init_boxes.at(c), normal, none);
        }

        /**
         * @brief Get the Final Box object
         * 
         * @param c
         * @return const Box
         */
        inline const Box getFinalBox(color c) const{
            return Box(final_boxes.at(c), normal, none);
        }


        /****************************************************************/

        /**
         * @brief Sobrecarga del operador == de un parchis pasado como argumento y el actual.
         * Se verifica que se de la igualdad entre el tablero (board) y el turno.
         *
         * @param parchis
         * @return true
         * @return false
         */
        bool operator==(const Parchis & parchis) const;

        /**
         * @brief Función que añade al jugador identificado por p como un nuevo viewer
         * a la lista de viewers.
         *
         * @param p
         */
        inline void addViewer(shared_ptr<Player> p){viewers.push_back(shared_ptr<Player>(p));}

        /**
         * @brief Función que devuelve todas las fichas de player que pueden
         * hacer un movimiento según el valor del dado dice_number.
         *
         * Por ejemplo, si dice_number = 3, las fichas que se encuentran en home
         * no aparecerán como disponibles.
         *
         * También se gestionan las barreras y otros casos particulares.
         *
         * @param player
         * @param dice_number
         * @return const vector<int>&
         */
        const vector<tuple<color,int>> getAvailablePieces (color player, int dice_number) const;

        /**
         * @brief Función que devuelve todas las fichas del jugador número player que pueden
         * hacer un movimiento según el valor del dado dice_number.
         *
         * Por ejemplo, si dice_number = 3, las fichas que se encuentran en home
         * no aparecerán como disponibles.
         *
         * También se gestionan las barreras y otros casos particulares.
         *
         * @param player
         * @param dice_number
         * @return const vector<int>&
         */
        inline const vector<tuple<color, int>> getAvailablePieces(int player, int dice_number) const{
            return getAvailablePieces(this->getPlayerColors(player)[0], dice_number);
        }

        /**
         * @brief Obtener los números del dado disponibles para el jugador de color player.
         * IMPORTANTE: Esta función devuelve solo los dados que se pueden usar en el turno actual,
         * independientemente de que el jugador tenga en su mano más dados.
         * (Por ejemplo, si le toca contarse 10 o 20 solo se devolverán dichos dados).
         *
         * @param player
         * @param dice_number
         * @return const vector<int>&
         */
        inline const vector<int> getAvailableNormalDices (color player) const{
            return dice.getDice(player);
        }

        /**
         * @brief Obtener los números del dado disponibles para el jugador número player.
         * IMPORTANTE: Esta función devuelve solo los dados que se pueden usar en el turno actual,
         * independientemente de que el jugador tenga en su mano más dados.
         * (Por ejemplo, si le toca contarse 10 o 20 solo se devolverán dichos dados).
         *
         * @param player
         * @param dice_number
         * @return const vector<int>&
         */
        inline const vector<int> getAvailableNormalDices(int player) const
        {
            return dice.getDice(this->getPlayerColors(player)[0]);
        }

        /**
         * @brief Obtener los dados especiales disponibles para el jugador de color player.
         * IMPORTANTE: Esta función devuelve solo los dados que se pueden usar en el turno actual,
         * independientemente de que el jugador tenga en su mano más dados.
         * (Por ejemplo, si le toca contarse 10 o 20 no se devolverá ningún dado).
         *
         */
        inline const vector<int> getAvailableSpecialDices (color player) const{
            return (dice.getAllDiceLayers(player).size() < 3)?dice.getSpecialDice(player):vector<int>();
        }

        /**
         * @brief Obtener los dados especiales disponibles para el jugador número player.
         * IMPORTANTE: Esta función devuelve solo los dados que se pueden usar en el turno actual,
         * independientemente de que el jugador tenga en su mano más dados.
         * (Por ejemplo, si le toca contarse 10 o 20 no se devolverá ningún dado).
         *
         */
        inline const vector<int> getAvailableSpecialDices(int player) const{
            return getAvailableSpecialDices(this->getPlayerColors(player)[0]);
        }

        /**
         * Obtener todos los dados disponibles, incluyendo normales y especiales.
         * IMPORTANTE: Esta función devuelve solo los dados que se pueden usar en el turno actual,
         * independientemente de que el jugador tenga en su mano más dados.
         * (Por ejemplo, si le toca contarse 10 o 20 solo se devolverán dichos dados).
         */
        inline const vector<int> getAllAvailableDices (color player) const{
            vector<int> all_dices = dice.getDice(player);
            vector<int> special_dices = getAvailableSpecialDices(player);
            all_dices.insert(all_dices.end(), special_dices.begin(), special_dices.end());
            return all_dices;
        }

        /**
         * @brief Obtener todos los dados disponibles, incluyendo normales y especiales.
         * IMPORTANTE: Esta función devuelve solo los dados que se pueden usar en el turno actual,
         * independientemente de que el jugador tenga en su mano más dados.
         * (Por ejemplo, si le toca contarse 10 o 20 solo se devolverán dichos dados).
         */
        inline const vector<int> getAllAvailableDices(int player) const{
            return getAllAvailableDices(this->getPlayerColors(player)[0]);
        }

        /**
         * @brief Obtener todos los dados normales para el jugador de color player.
         * IMPORTANTE: esto incluye todos los dados normales del 1 al 6 que tenga el jugador en su mano en ese momento,
         * aunque en el turno en cuestión no pueda usarlos (por ejemplo, si tiene que contarse obligatoriamente 10 o 20).
         */
        inline const vector<int> getNormalDices(color player) const{
            return dice.getAllDiceLayers(player)[0];
        }

        /**
         * @brief Obtener todos los dados normales para el jugador número player.
         * IMPORTANTE: esto incluye todos los dados normales del 1 al 6 que tenga el jugador en su mano en ese momento,
         * aunque en el turno en cuestión no pueda usarlos (por ejemplo, si tiene que contarse obligatoriamente 10 o 20).
         */
        inline const vector<int> getNormalDices(int player) const{
            return dice.getAllDiceLayers(this->getPlayerColors(player)[0])[0];
        }

        /**
         * @brief Obtener todos los dados especiales para el jugador de color player.
         * IMPORTANTE: esto incluye todos los dados especiales que tenga el jugador en su mano en ese momento,
         * aunque en el turno en cuestión no pueda usarlos (por ejemplo, si tiene que contarse obligatoriamente 10 o 20).
         */
        inline const vector<int> getSpecialDices(color player) const{
            return dice.getAllDiceLayers(player)[1];
        }

        /**
         * @brief Obtener todos los dados especiales para el jugador número player.
         * IMPORTANTE: esto incluye todos los dados especiales que tenga el jugador en su mano en ese momento,
         * aunque en el turno en cuestión no pueda usarlos (por ejemplo, si tiene que contarse obligatoriamente 10 o 20).
         */
        inline const vector<int> getSpecialDices(int player) const{
            return dice.getAllDiceLayers(this->getPlayerColors(player)[0])[1];
        }

        /**
         * @brief Obtener todos los dados para el jugador de color player.
         * IMPORTANTE: esto incluye todos los dados que tenga el jugador en su mano en ese momento,
         * aunque en el turno en cuestión no pueda usarlos (por ejemplo, si tiene que contarse obligatoriamente 10 o 20).
         */
        inline const vector<int> getAllDices(color player) const{
            vector<int> all_dices = dice.getAllDiceLayers(player)[0];
            vector<int> special_dices = dice.getAllDiceLayers(player)[1];
            all_dices.insert(all_dices.end(), special_dices.begin(), special_dices.end());
            return all_dices;
        }

        /**
         * @brief Obtener todos los dados para el jugador número player.
         * IMPORTANTE: esto incluye todos los dados que tenga el jugador en su mano en ese momento,
         * aunque en el turno en cuestión no pueda usarlos (por ejemplo, si tiene que contarse obligatoriamente 10 o 20).
         */
        inline const vector<int> getAllDices(int player) const{
            return getAllDices(this->getPlayerColors(player)[0]);
        }

        /**
         * @brief Mover la pieza número "piece" del jugador "player" "dice_number" posiciones.
         *
         * @param player
         * @param piece
         * @param dice_number
         */
        void movePiece(color player, int piece, int dice_number);

        /**
         * @brief Función que comprueba si un movimiento es válido para las fichas de un determinado
         * color en una determinada casilla. Tiene en cuenta barreras y otras particularidades.
         *
         *
         * @param player
         * @param box
         * @param dice_number
         * @return true
         * @return false
         */
        bool isLegalMove(const Piece & piece, int dice_number) const;

        /**
         * @brief Comprobar si el jugador puede pasar turno con el dado seleccionado (si no tiene fichas para mover).
         *
         * @param player
         * @param dice_number
         * @return true
         * @return false
         */
        bool canSkipTurn(color player, int dice_number) const;

        /**
         * @brief Función que devuelve la ocupación de una casilla.
         *
         * Se proporciona en formato vector de parejas con identificador el color, y valor
         * el número de fichas de ese color en esa casilla.
         *
         * Por ejemplo, si la  casilla tiene 1 ficha azul y otra amarilla, la función devolverá:
         * {{blue, 1}, {yellow, 1}}
         *
         * @param box
         * @return vector<pair <color, int>>
         */
        const vector<pair <color, int>> boxState(const Box & box) const;

        /**
         * @brief Función auxiliar que calcula la casilla destino tras aplicar el movimiento.
         *
         * @param player
         * @param box
         * @param dice_number
         * @return const Box
         */
        const Box computeMove(const Piece & piece, int dice_number, bool * goal_bounce = NULL) const;

        /**
         * @brief Función auxiliar que devuelve la casilla destino tras aplicar el movimiento.
         *
         * @param c
         * @param box
        */
        inline const Box computeMove(const color & c, const Box & box, int dice_number, bool * goal_bounce = NULL) const{
            return computeMove(Piece(c, box), dice_number, goal_bounce);
        }

        /**
         * @brief Función auxiliar que devuelve la siguiente casilla para una ficha concreta.
         *
         * @param Piece
        */
        inline const Box nextBox(const Piece & piece) const{
            return computeMove(piece , 1);
        }

        inline const Box nextBox(const color & c, const Box & box) const{
            return computeMove(Piece(c, box) , 1);
        }

        /**
         * @brief Función auxiliar que devuelve la casilla desde la que partiría la ficha al contarse el número que se indica como argumento.
         * 
         * @param piece 
         * @param dice_number 
         * @return const Box 
         */
        const Box computeReverseMove(const Piece & piece, int dice_number) const;

        inline const Box computeReverseMove(const color & c, const Box & box, int dice_number) const{
            return computeReverseMove(Piece(c, box), dice_number);
        }

        /**
         * @brief Función auxiliar que devuelve la casilla a la que se llegaría usando un dado especial que provoca movimiento (champiñón o bala).
         *
         * @param piece
         * @param dice_number
         * @return const Box
         */
        const Box computeSpecialMove(const Piece &piece, int dice_number) const;

        inline const Box computeSpecialMove(const color &c, const Box &box, int dice_number) const
        {
            return computeSpecialMove(Piece(c, box), dice_number);
        }

        /**
         * @brief Método que gestiona el bucle principal del juego, mientras este no haya terminado,
         * y de terminarlo en caso de que haya un ganador.
         *
         * Se apoya en la función gameStep()
         *
         */
        void gameLoop();


        /**
         * @brief Método que gestiona un turno de juego. Obtiene el movimiento del jugador actual y notifica
         * al resto de jugadores.
         *
         */
        bool gameStep();

        /**
         * @brief Función para comprobar que todos los jugadores están listos para pasar el turno, y si no esperarlos.
         *
         */
        void waitForPlayers() const;


        /**
         * @brief Termina una partida por la fuerza. Da por ganador al jugador que no esté en el turno actual.
         * Para usar solo para gestionar desconexiones forzadas.
         *
         */
        void endGame();

        /**
         * @brief Indica si la partida ha terminado.
         *
         * @return true
         * @return false
         */
        bool gameOver() const;


        /**
         * @brief Si la partida ha terminado, devuelve el índice del jugador ganador (0 o 1).
         *
         * @return int
         */
        int getWinner() const;

        /**
         * @brief Si la partida ha terminado, devuelve el color del jugador ganador.
         *
         * @return color
         */
        color getColorWinner() const;

        /**
         * @brief Devuelve true si y solo si uno de los jugadores ha hecho un movimiento ilegal.
         * El jugador que ha hecho dicho movimiento pierde automáticamente.
         *
         * @return true
         * @return false
         */
        bool illegalMove() const;

        /**
         * @brief Devuelve true si y solo si uno de los jugadores ha rebotado más de 30 veces con algún color.
         * El jugador que ha rebotado pierde automáticamente.
         * La finalidad de este límite es evitar que se produzcan partidas infinitas.
         *
         * @return true
         * @return false
         */
        bool overBounce() const;

        /**
         * @brief Devuelve el número de veces que han rebotado en la meta las fichas del color indicado por player.
         *
         * @return int
         */
        inline const int getBounces(color player) const{
            return bounces.at(player);
        }

        /**
         * @brief Función que devuelve la variable update_board
         *
         * @return true
         * @return false
         */
        inline const bool updateBoard(){
            return this->update_board;
        }

        /**
         * @brief Función que devuelve la variable update_dice
         *
         * @return true
         * @return false
         */
        inline const bool updateDice(){
            return this->update_dice;
        }

        /**
         * @brief Función que devuelve la variable update_dice
         *
         */
        inline void sendUpdatedBoardSignal(){
            this->update_board = false;
        }

        /**
         * @brief Función que actualiza a false el valor de update_dice
         *
         */
        inline void sendUpdatedDiceSignal(){
            this->update_dice = false;
        }

        /**
         * @brief Activa el modo creativo.
         *
         */
        void setPlaygroundMode();

        /**
         * @brief Indica si la partida es en modo creativo o no.
         *
         */
        inline bool isPlaygroundMode() const { return this->playground_mode; }

        /**************************** MÉTODOS PARA LA HEURÍSTICA *********************/

        /**
         * @brief Devuelve el número de fichas de un color que han llegado a la meta.
         *
         * @return int
         */
        int piecesAtGoal(color player) const;

        /**
        * @brief Devuelve el número de fichas de un color que estan en casa.
        *
        * Thanks Mario :)
        *
        * @return int
        */
        int piecesAtHome(color player) const;

        /**
         * @brief Función que devuelve la distancia a la meta del color "player" desde
         * la casilla "box".
         *
         * La distancia se entiende como el número de casillas que hay que avanzar hasta
         * la meta.
         *
         * @param player
         * @param box
         * @return int
         */
        int distanceToGoal(color player, const Box & box) const;

        /**
         * @brief Función que devuelve la distancia a la meta de la ficha identificada
         * por id_piece del jugador identificado por player.
         *
         * La distancia se entiende como el número de casillas que hay que avanzar hasta
         * la meta.
         *
         * @param player
         * @param id_piece
         * @return int
         */
        int distanceToGoal(color player, int id_piece) const;

        /**
         * @brief  Función que devuelve el número de casillas que hay que pasar para llegar desde
         * box1 hasta box2 para un determinado color especificado por el parámetro player.
         *
         * Devuelve -1 si la casilla box2 no es alcanzable desde box1 para una ficha del
         * color player.
         *
         * No se tienen en cuenta rebotes.
         * Para ver la distancia entre dos casillas teniendo en cuenta rebotes o no (para el color player), podría hacer:
         * distanceBoxtobox(player, box1, box2) (si no me devuelve -1 podré llegar a la casilla sin rebotar).
         * distanceToGoal(player, box1) + distanceToGoal(player, box2) (distancia entre las dos casillas con rebote).
         *
         * @param player
         * @param box1
         * @param box2
         * @return int
         */
        int distanceBoxtoBox(color player, const Box & box1, const Box & box2) const;

        /**
         * @brief Función que devuelve el número de casillas que tiene que avanzar la ficha del
         * color player1 identificada con id_p1 para alcanzar la ficha del color player2
         * identificada con id_p2. No se tienen en cuenta rebotes.
         *
         * Devuelve -1 si la segunda pieza es inalcanzable por la primera.
         *
         * @param player1
         * @param id_p1
         * @param player2
         * @param id_p2
         * @return int
         */
        int distanceBoxtoBox(color player1, int id_p1, color player2, int id_p2) const;


        /**
         * @brief Función que devuelve si una determinada casilla es segura o no.
         *
         * @param box
         * @return true
         * @return false
         */
        bool isSafeBox(const Box & box) const;

        /**
         * @brief Función que devuelve si una determinada ficha de un determinado está
         * en una casilla segura o no.
         *
         * @param player
         * @param piece
         * @return true
         * @return false
         */
        bool isSafePiece(const color & player, const int & piece) const;

        /**
         * @brief Función que devuelve el color de la barrera (en caso de haberla) en la casilla "b".
         * Es decir, si en la casilla "b" hay dos fichas de un mismo color devuelve este color.
         *
         * @param b
         * @return const color
         */
        const color isWall(const Box & b) const;

        /**
         * @brief Función que devuelve el color de la mega barrera (en caso de haberla) en la casilla "b".
         * Es decir, si hay una barrera formada por una mega ficha devuelve el color de dicha ficha.
         *
         * @param b
         * @return const color
         */
        const color isMegaWall(const Box & b) const;

        /**
         * @brief Función que devuelve el vector de colores de las barreras (en caso de haberlas) del
         * camino entre b1 y b2.
         *
         * Esto es, se va recorriendo todas las casillas que habría que recorrer para ir de b1 y b2,
         * y siempre que se encuentran dos fichas de un mismo color en una misma casilla se añade ese
         * color al vector que se devuelve.
         *
         * Por ejemplo: si en la casilla 2 hay una barrera amarilla y en la 4 una azul, el anywalls(1,6)
         * devuelve {yellow, blue}
         *
         * @param b1
         * @param b2
         * @return const vector<color>
         */
        const vector<color> anyWall(const Box & b1, const Box & b2) const;

        /**
         * @brief Función que devuelve un vector de trampas entre b1 y b2, en caso de haberlas.
         * En caso contrario, devuelve un vector vacío.
         *
         * @param b1
         * @param b2
         * @return const vector<BoardTrap>
         */
        const vector<BoardTrap> anyTrap(const Box & b1, const Box & b2) const;

        /**
         * @brief Función que devuelve el vector de colores de las mega barreras (en caso de haberlas) del
         * camino entre b1 y b2.
         *
         * Esto es, se va recorriendo todas las casillas que habría que recorrer para ir de b1 y b2,
         * y siempre que se encuentran una mega ficha en una casilla se añade ese color al vector que se devuelve.
         *
         *
         * @param b1
         * @param b2
         * @return const vector<color>
         */
        const vector<color> anyMegaWall(const Box & b1, const Box & b2) const;

        /**
         * @brief Función que devuelve todas las fichas que hay entre la casilla b1 y la b2.
         * Va recorriendo todas las casillas por las que habría que pasar para ir de b1 a b2 y
         * añadiendo al vector pares con la ocupación de las casillas de la forma {color, num_ficha}.
         *
         * @param b1
         * @param b2
         * @return const vector<pair <color, int>>
         */
        const vector<pair <color, int>> allPiecesBetween(const Box & b1, const Box & b2) const;

        /**
         * @brief Devuelve los colores asociados a cada jugador (amarillo y rojo para el 0 y verde y azul para el 1).
         *
         * @param player
         * @return vector<color>
         */
        vector<color> getPlayerColors(int player) const;

        // -- CONSULTAS DE FICHAS ESPECIALES -- //

        /**
         * @brief Función que devuelve si se adquirió un objeto en el último turno.
         *
         * @return true
         * @return false
         */
        bool itemAcquired() const;

        /**
         * @brief Función que devuelve el objeto adquirido en el último turno.
         * Devuelve -1 en caso de que no se haya adquºirido ningún objeto.
         *
         * @return item_type
         */
        item_type getItemAcquired() const;

        /**
         * @brief Función que indica si el número de dado especificado está asociado a dado especial o no.
         *
         * @param dice
         * @return true
         * @return false
         */
        bool isSpecialDice(int dice) const;

        /**
         * Función que indica si el número de dado especificado está asociado a dado normal o no.
         *
         * @param dice
         * @return true
         * @return false
         */
        bool isNormalDice(int dice) const;

        /**
         * @brief Función que devuelve las fichas que hayan sido destruidas por una estrella en el último turno.
         * Se devuelve un color con los {color, id} de las fichas destruidas.
         */
        const vector<pair<color, int>> piecesDestroyedByStar() const;

        /**
         * @brief Función que devuelve las fichas que hayan sido destruidas por un megachampiñón en el último turno.
         * Se devuelve un color con los {color, id} de las fichas destruidas.
         */
        const vector<pair<color, int>> piecesCrushedByMegamushroom() const;

        /**
         * @brief Función que devuelve las fichas que hayan sido destruidas por un caparazón rojo en el último turno.
         * Se devuelve un color con los {color, id} de las fichas destruidas.
         */
        const vector<pair<color, int>> piecesDestroyedByRedShell() const;

        /**
         * @brief Función que devuelve las fichas que hayan sido destruidas por un caparazón azul en el último turno.
         * Se devuelve un color con los {color, id} de las fichas destruidas.
         */
        const vector<pair<color, int>> piecesDestroyedByBlueShell() const;

        /**
         * @brief Función que devuelve las fichas que hayan sido destruidas por una bocina en el último turno.
         * Se devuelve un color con los {color, id} de las fichas destruidas.
         */
        const vector<pair<color, int>> piecesDestroyedByHorn() const;

        /**
         * @brief Función que devuelve las fichas que hayan sido destruidas por cualquier movimiento especial en el último turno.
         * Se devuelve un color con los {color, id} de las fichas destruidas.
         * IMPORTANTE: Las fichas comidas normales no se cuentan aquí, para eso se usa eatenPiece().
         */
        const vector<pair<color, int>> piecesDestroyedLastMove() const;

        /**
         * @brief Devuelve la ficha comida por un movimiento normal en el último turno.
         *
         * @return const pair<color, int>
         */
        const pair<color, int> eatenPiece() const;
};


#endif
#include "Parchis.h"

//Definición de las casillas seguras
const vector<int> Parchis::safe_boxes = {4, 13, 17, 21, 30, 34, 38, 47, 51, 55, 64, 68};

//Definición de los colores del juego
const vector<color> Parchis::game_colors = vector<color>{yellow, blue, red, green};

//Definición de las casillas finales para cada color
const map<color, int> Parchis::final_boxes = {
    {yellow, final_yellow_box},
    {blue, final_blue_box},
    {red, final_red_box},
    {green, final_green_box}
};

//Definición de las casillas de inicio para cada jugador
const map<color, int> Parchis::init_boxes = {
    {yellow, init_yellow_box},
    {blue, init_blue_box},
    {red, init_red_box},
    {green, init_green_box}
};

/****************** MÉTODOS PRIVADOS ******************/

void Parchis::nextTurn(){
    // Quitamos un turn_left del current color y su partner.
    color partner = partner_color(this->current_color);
    for(int i = 0; i < this->board.getPieces(this->current_color).size(); i++){
        this->board.decreasePieceTurnsLeft(this->current_color, i);
        if(this->board.getPiece(this->current_color, i).get_turns_left() == 0){
            this->board.setPieceType(this->current_color, i, normal_piece);
        }
    }
    for(int i = 0; i < this->board.getPieces(partner).size(); i++){
        this->board.decreasePieceTurnsLeft(partner, i);
        if(this->board.getPiece(partner, i).get_turns_left() == 0){
            this->board.setPieceType(partner, i, normal_piece);
        }
    }

    if (last_dice != 6 && !eating_move && !goal_move && !remember_6 || bananed){
        this->current_player = (current_player+1)%2;
        switch(this->current_color){
            case yellow:
                this->current_color = blue;
                break;
            case blue:
                this->current_color = yellow;
                break;
        }
    }


}


/****************** CONSTRUCTORES ******************/

void Parchis::initGame(){
    this->last_dice = -1;

    this->current_player = 0;
    this->current_color = yellow;

    this->illegal_move_player = -1;
    this->disconnected_player = -1;
    this->goal_move = false;
    this->eating_move = false;
    this->goal_bounce = false;
    this->remember_6 = false;
    this->bananed = false;

    this->red_shell_move = false;
    this->blue_shell_move = false;
    this->star_move = false;
    this->bullet_move = false;
    this->horn_move = false;
    this->shock_move = false;
    this->boo_move = false;
    this->mega_mushroom_move = false;
    this->mushroom_move = false;
    this->banana_move = false;

    this->turn = 1;

    bounces = {
        {yellow, 0},
        {blue, 0},
        {red, 0},
        {green, 0}
    };
    this->overbounce_player = -1;

    this->update_board = true;
    this->update_dice = true;
    this->last_acquired = not_an_item;

    this->playground_mode = false;
}

Parchis::Parchis()
{
    this->board = Board();
    this->dice = Dice();
    initGame();

    players.push_back(make_shared<AIPlayer>("J1"));
    players.push_back(make_shared<AIPlayer>("J2"));
}

Parchis::Parchis(const BoardConfig & b){
    this->board = Board(b);
    this->dice = Dice();
    initGame();

    players.push_back(make_shared<AIPlayer>("J1"));
    players.push_back(make_shared<AIPlayer>("J2"));
}

Parchis::Parchis(const Board & b, const Dice & d){
    this->board = board;
    this->dice = dice;
    initGame();

    players.push_back(make_shared<AIPlayer>("J1"));
    players.push_back(make_shared<AIPlayer>("J2"));
}

Parchis::Parchis(const BoardConfig &b, const Dice &d)
{
    this->board = Board(b);
    this->dice = dice;
    initGame();

    players.push_back(make_shared<AIPlayer>("J1"));
    players.push_back(make_shared<AIPlayer>("J2"));
}

Parchis::Parchis(const Board & b, const Dice & d, Player & p1, Player & p2){
    this->board = board;
    this->dice = dice;
    initGame();

    players.push_back(shared_ptr<Player>(std::move(&p1)));
    players.push_back(shared_ptr<Player>(std::move(&p2)));
}

Parchis::Parchis(const BoardConfig &b, const Dice &d, shared_ptr<Player> p1,  shared_ptr<Player> p2)
{
    this->board = Board(b);
    this->dice = dice;
    initGame();

    players.push_back(shared_ptr<Player>(p1));
    players.push_back(shared_ptr<Player>(p2));
}

Parchis::Parchis(const BoardConfig &b, shared_ptr<Player> p1,  shared_ptr<Player> p2){
    this->board = Board(b);
    this->dice = Dice();
    initGame();

    players.push_back(shared_ptr<Player>(p1));
    players.push_back(shared_ptr<Player>(p2));
}

/****************** GETTERS ******************/

const Dice & Parchis::getDice() const{
    return this->dice;
}

const Board & Parchis::getBoard() const{
    return this->board;
}

int Parchis::getLastDice() const{
    return this->last_dice;
}

const vector<tuple<color, int, Box, Box>> & Parchis::getLastMoves() const{
    return this->last_moves;
}

vector<color> Parchis::getPlayerColors(int player) const{
    if (player == 0){
        return {yellow, red};
    }
    else{
        return {blue, green};
    }
}

/**************************************************/


bool Parchis::operator==(const Parchis & parchis) const{
    return this->board == parchis.board && this->turn == parchis.turn;
}

const vector<tuple<color,int>> Parchis::getAvailablePieces(color player, int dice_number) const{
    vector<Piece> player_pieces = board.getPieces(player);
    vector<Piece> partner_color_pieces = board.getPieces(partner_color(player));
    vector<tuple<color,int>> available_pieces;


    //Para cada ficha del jugador
    for(int i = 0; i < player_pieces.size(); i++){
        //Compruebo si el movimiento es legal
        if(isLegalMove(this->board.getPiece(player, i), dice_number)){
            available_pieces.push_back({player,i});
        }
    }

    for(int i = 0; i < partner_color_pieces.size(); i++){
        //Compruebo si el movimiento es legal
        if(isLegalMove(this->board.getPiece(partner_color(player), i), dice_number)){
            available_pieces.push_back({partner_color(player),i});
        }
    }

    return available_pieces;

}

void Parchis::movePiece(color player, int piece, int dice_number){
    if(!gameOver()){
        // Si elijo pasar turno compruebo que efectivamente puedo hacerlo.
        // Si sí, pongo el turno en el siguiente jugador. Si no, el jugador ha hecho un movimiento ilegal.
        if(piece == SKIP_TURN){
            if(canSkipTurn(player, dice_number)){
                eating_move = false;
                goal_move = false;

                remember_6 = (dice_number==6 or (remember_6 and (dice_number == 10 or dice_number == 20)));

                this->last_dice = dice_number;
                this->last_moves.clear();
                if(!playground_mode) this->dice.removeNumber(player, dice_number);
                this->nextTurn();

                turn++;
                last_action = tuple<color, int, int>(player, piece, dice_number);
            }else{
                turn++;
                illegal_move_player = current_player;
                cout << "ILLEGALLY TRIED TO SKIP TURN" << endl;
            }
            return;
        }
        // Switch por colores
        Box piece_box = board.getPiece(player, piece).get_box();
        Piece current_piece = board.getPiece(player, piece);

        this->last_dice = dice_number;
        this->last_moves.clear();

        this->last_acquired = not_an_item;

        this->pieces_destroyed_by_star.clear();
        this->pieces_crushed_by_megamushroom.clear();
        this->pieces_destroyed_by_red_shell.clear();
        this->pieces_destroyed_by_blue_shell.clear();
        this->pieces_destroyed_by_horn.clear();

        if(isLegalMove(board.getPiece(player, piece), dice_number)){
            if(dice_number < 100){
                goal_bounce = false;
                Box final_box = computeMove(current_piece, dice_number, &goal_bounce);

                /* Gestión de las "comidas"*/
                eating_move = false;
                goal_move = false;
                bool destroyed_by_star = false;

                red_shell_move = false;
                blue_shell_move = false;
                star_move = false;
                bullet_move = false;
                horn_move = false;
                bananed = false;
                shock_move = false;
                boo_move = false;
                mega_mushroom_move = false;
                mushroom_move = false;
                banana_move = false;

                remember_6 = (dice_number==6 or (remember_6 and (dice_number == 10 or dice_number == 20)));

                //Gestion de trampas
                if (current_piece.get_type() != star_piece and current_piece.get_type() != boo_piece and current_piece.get_type() != mega_piece){
                    vector<BoardTrap> any_trap = anyTrap(current_piece.get_box(), final_box);

                    if(any_trap.size() > 0){
                        final_box = any_trap[0].getBox();
                        remember_6 = false;
                        bananed = true;
                        board.deleteTrap(final_box);
                        this->update_board = true;
                        board.setPieceType(player, piece, bananed_piece);
                        board.setPieceTurnsLeft(player, piece, 2);
                    }
                }


                //Comprobar si hay una ficha de otro color en la casilla destino
                vector<pair <color, int>> box_states = boxState(final_box);

                //Comprobar si la ficha en movimiento es una estrella
                if (current_piece.get_type() == star_piece){
                    star_move = true;
                    //Obtenemos todas las fichas en su camino
                    vector <pair<color, int>> destroyed_pieces = allPiecesBetween(piece_box, final_box);
                    Box origin = current_piece.get_box();
                    for (int i = 0; i < destroyed_pieces.size(); i++){
                        special_type destroyed_type = board.getPiece(destroyed_pieces[i].first, destroyed_pieces[i].second).get_type();
                        //Comprobamos que no sean de su color o fantasmas
                        if (destroyed_pieces[i].first != player and destroyed_type != boo_piece and destroyed_type != star_piece){
                            this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, origin, board.getPiece(destroyed_pieces[i].first, destroyed_pieces[i].second).get_box()));
                            origin = board.getPiece(destroyed_pieces[i].first, destroyed_pieces[i].second).get_box();
                            board.movePiece(destroyed_pieces[i].first, destroyed_pieces[i].second, Box(0, home, destroyed_pieces[i].first));
                            this->last_moves.push_back(tuple<color, int, Box, Box>(destroyed_pieces[i].first, destroyed_pieces[i].second, origin, Box(0, home,  destroyed_pieces[i].first)));
                        }
                    }

                    this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, origin, final_box));
                    this->pieces_destroyed_by_star = destroyed_pieces;

                    board.movePiece(player, piece, final_box);
                }

                else if (current_piece.get_type() == mega_piece)
                {
                    mega_mushroom_move = true;
                    // Si la siguiente casilla a la del mega champiñón es la meta, nos movemos ahí directamente.
                    if(nextBox(player, final_box).type == goal){
                        final_box = nextBox(player, final_box);
                    }


                    if(!goal_bounce)
                        this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, final_box));
                    else{
                        this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, Box(0, goal, player)));
                        this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, Box(0, goal, player), final_box));
                        bounces[player]++;
                        if(bounces[player] > 30){
                            overbounce_player = current_player;
                        }
                    }

                    Piece eating_piece = current_piece;
                    //Comprobar que la casilla no es segura
                    for (int i = 0; i < box_states.size(); i++){
                        if (box_states[i].first != player){
                            eating_piece = board.getPiece(box_states[i].first, box_states[i].second);
                            if (eating_piece.get_type() != boo_piece){
                                Box origen_comida = board.getPiece(box_states[i].first, box_states[i].second).get_box();
                                board.movePiece(box_states[i].first, box_states[i].second, Box(0, home, box_states[i].first));
                                this->last_moves.push_back(tuple<color, int, Box, Box>(box_states[i].first, box_states[i].second, origen_comida, Box(0, home, box_states[i].first)));
                                pieces_crushed_by_megamushroom.push_back({box_states[i].first, box_states[i].second});
                            }

                        }
                    }

                    vector<pair <color, int>> next_box_states = boxState(nextBox(player, final_box));
                    for (int i = 0; i < next_box_states.size(); i++){
                        if (next_box_states[i].first != player){
                            eating_piece = board.getPiece(next_box_states[i].first, next_box_states[i].second);
                            if (eating_piece.get_type() != boo_piece){
                                Box origen_comida = board.getPiece(next_box_states[i].first, next_box_states[i].second).get_box();
                                board.movePiece(next_box_states[i].first, next_box_states[i].second, Box(0, home, next_box_states[i].first));
                                this->last_moves.push_back(tuple<color, int, Box, Box>(next_box_states[i].first, next_box_states[i].second, origen_comida, Box(0, home, next_box_states[i].first)));
                                pieces_crushed_by_megamushroom.push_back({next_box_states[i].first, next_box_states[i].second});
                            }
                        }
                    }

                    // Comprobar si se ha obtenido objeto especial. En el caso de la mega ficha, hay que mirar casilla actual y la siguiente.
                    for (int i = 0; i < board.getSpecialItems().size(); i++)
                    {
                        if (final_box == board.getSpecialItems()[i].box)
                        {
                            // Se borra del tablero y se añade a los dados especiales del jugador.
                            if (dice.getSpecialDice(player).size() < MAX_SPECIAL_DICES)
                            {
                                dice.addSpecialDice(player, board.getSpecialItems()[i].type);
                                board.deleteSpecialItem(i);

                                this->update_board = true;
                                this->update_dice = true;
                                this->last_acquired = board.getSpecialItems()[i].type;
                            }
                        }

                        if(nextBox(player, final_box) == board.getSpecialItems()[i].box)
                        {
                            // Se borra del tablero y se añade a los dados especiales del jugador.
                            if (dice.getSpecialDice(player).size() < MAX_SPECIAL_DICES)
                            {
                                dice.addSpecialDice(player, board.getSpecialItems()[i].type);
                                board.deleteSpecialItem(i);

                                this->update_board = true;
                                this->update_dice = true;
                                this->last_acquired = board.getSpecialItems()[i].type;
                            }
                        }
                    }

                    // Si la mega ficha llega a la meta en ese turno, se hace pequeña automáticamente.
                    if(final_box.type == goal){
                        board.setPieceType(player, piece, normal_piece);
                    }
                    // En caso contrario, si es el último turno de ficha grande, acabará el turno en la siguiente posición.
                    else if(board.getPiece(player, piece).get_turns_left() == 1){
                        final_box = nextBox(player, final_box);
                    }
                    board.movePiece(player, piece, final_box);
                }
                else
                {
                    // Comprobar si hay estrellas en mi camino, si las hay muero (salvo que sea boo)
                    bool destroyed_by_star = false;
                    //Los boos, otras estrellas y mega_pieces sí pueden pasar por encima de estrellas
                    if (current_piece.get_type() != boo_piece and current_piece.get_type() != mega_piece and current_piece.get_type() != star_piece){
                        vector <pair<color, int>> all_pieces = allPiecesBetween(piece_box, final_box);
                        Box origin = current_piece.get_box();
                        for (int i = 0; i < all_pieces.size() and !destroyed_by_star; i++){
                            if (all_pieces[i].first != player and board.getPiece(all_pieces[i].first, all_pieces[i].second).get_type() == star_piece){
                                this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, origin, board.getPiece(all_pieces[i].first, all_pieces[i].second).get_box()));
                                origin = board.getPiece(all_pieces[i].first, all_pieces[i].second).get_box();
                                board.movePiece(player, piece, Box(0, home, player));
                                this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, origin, Box(0, home, player)));
                                destroyed_by_star = true;
                                star_move = true;
                                final_box = Box(0, home, player);
                                pieces_destroyed_by_star.push_back({player, piece});
                            }
                        }
                    }
                    if(!destroyed_by_star){
                        if (!box_states.empty() && box_states[0].first != player){
                            //Comprobar que la casilla no es segura
                            vector<int>::const_iterator ci;
                            Piece piece_to_eat = board.getPiece(box_states[0].first, box_states[0].second);
                            Piece eater_piece = board.getPiece(player, piece);
                            if (final_box.type == normal && count(safe_boxes.begin(), safe_boxes.end(), final_box.num) == 0 &&
                                    piece_to_eat.get_type() != boo_piece && eater_piece.get_type() != boo_piece && eater_piece.get_type() != small_piece &&
                                    eater_piece.get_type() != bananed_piece){
                                //Movemos la ficha
                                eating_move = true;
                                eaten_piece = box_states.at(0);
                            }
                        }

                        board.movePiece(player, piece, final_box);

                        if(!goal_bounce)
                            this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, final_box));
                        else{
                            this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, Box(0, goal, player)));
                            this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, Box(0, goal, player), final_box));
                            bounces[player]++;
                            if(bounces[player] > 30){
                                overbounce_player = current_player;
                            }
                        }

                        // Controlar si se come alguna ficha. En ese caso se actualiza también la ficha comida.
                        // La ficha comida se añadiría también al vector last_moves.
                        if(eating_move){
                            Box origen_comida = board.getPiece(box_states[0].first, box_states[0].second).get_box();
                            board.movePiece(box_states[0].first, box_states[0].second, Box(0, home, box_states[0].first));
                            this->last_moves.push_back(tuple<color, int, Box, Box>(box_states[0].first, box_states[0].second, origen_comida, Box(0, home, box_states[0].first)));
                        }
                    }
                }

                // Controlar si la ficha ha llegado a la meta. En ese caso el jugador se cuenta 10 con otra ficha (salvo que sea la última)
                if(final_box.type == goal && !gameOver()){
                    goal_move = true;
                }

                //Comprobar si se ha obtenido objeto especial
                for (int i = 0; i < board.getSpecialItems().size(); i++){
                    if (final_box == board.getSpecialItems()[i].box){
                        // Se borra del tablero y se añade a los dados especiales del jugador.
                        if (dice.getSpecialDice(player).size() < MAX_SPECIAL_DICES)
                        {
                            dice.addSpecialDice(player, board.getSpecialItems()[i].type);
                            board.deleteSpecialItem(i);

                            this->update_board = true;
                            this->update_dice = true;
                            this->last_acquired = board.getSpecialItems()[i].type;
                        }
                    }
                }

                if(!playground_mode) this->dice.removeNumber(player, dice_number);

                if(eating_move){
                    // Añadir al dado de player el valor 20
                    dice.forceNumber(player, 20);
                }
                if(goal_move){
                    // Añadir al dado de player el valor 10
                    dice.forceNumber(player, 10);
                }
            }else{
                if(!playground_mode) this->dice.removeNumber(player, dice_number);
                this->update_dice = true;

                this->red_shell_move = false;
                this->blue_shell_move = false;
                this->star_move = false;
                this->bullet_move = false;
                this->horn_move = false;
                this->shock_move = false;
                this->boo_move = false;
                this->mega_mushroom_move = false;
                this->mushroom_move = false;
                this->banana_move = false;

                remember_6 = false; // Si no, puedo sacar 6 y empezar a tirar dados especiales sin parar

                switch(dice_number){
                    case star:
                    //Convertimos la ficha a especial
                        board.setPieceType(player, piece, star_piece);
                        board.setPieceTurnsLeft(player, piece, 4);
                    break;
                    case mushroom:
                    {
                        this->mushroom_move = true;
                        int move_number = 8;
                        Box final_box;
                        // Si la ficha está a menos de 40 casillas de su meta, del tirón.
                        if(distanceToGoal(player, piece) <= move_number){
                            final_box = Box(0, goal, player);
                        }
                        else{
                            final_box = computeMove(current_piece.get_color(), current_piece.get_box(), move_number);
                            while (this->boxState(final_box).size() > 0)
                            {
                                move_number++;
                                //final_box = computeMove(current_piece, dice_number);
                                final_box = computeMove(current_piece.get_color(), current_piece.get_box(), move_number);
                            }
                        }
                        board.movePiece(player, piece, final_box);
                        this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, final_box));

                        for (int i = 0; i < board.getSpecialItems().size(); i++){
                            if (final_box == board.getSpecialItems()[i].box){
                                // Se borra del tablero y se añade a los dados especiales del jugador.
                                if (dice.getSpecialDice(player).size() < MAX_SPECIAL_DICES)
                                {
                                    dice.addSpecialDice(player, board.getSpecialItems()[i].type);
                                    board.deleteSpecialItem(i);

                                    this->update_board = true;
                                    this->update_dice = true;
                                    this->last_acquired = board.getSpecialItems()[i].type;
                                }
                            }
                        }

                    }
                    break;
                    case boo:
                        this->boo_move = true;
                        //Convertimos la ficha a especial
                        board.setPieceType(player, piece, boo_piece);
                        board.setPieceTurnsLeft(player, piece, 6);

                        //Robamos el último dado especial conseguido por el adversario (siempre que haya)
                        if(!dice.getSpecialDice(opponent_color(player)).empty() and !playground_mode){
                            if (dice.getSpecialDice(player).size() < MAX_SPECIAL_DICES)
                            {
                                dice.addSpecialDice(player, dice.getSpecialDice(opponent_color(player)).back());
                            }
                            dice.removeNumber(opponent_color(player), dice.getSpecialDice(opponent_color(player)).back());
                        }
                        else if(playground_mode){
                            cout << "IMPORTANTE: SI ESTUVIERAS JUGANDO EN MODO NORMAL HABRÍAS ROBADO EL ÚLTIMO DADO ESPECIAL DEL ADVERSARIO" << endl;
                        }
                    break;
                    case bullet:
                    {
                        this->bullet_move = true;
                        board.setPieceType(player, piece, normal_piece);
                        int move_number = 40;
                        // Si la ficha está en su home se saca a su casilla inicial primero.
                        if(current_piece.get_box().type == home){
                            board.movePiece(player, piece, Box(init_boxes.at(player), normal, none));
                            this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, Box(0, home, player), Box(init_boxes.at(player), normal, none)));
                            current_piece = board.getPiece(player, piece);
                        }
                        Box final_box;
                        // Si la ficha está a menos de 40 casillas de su meta, del tirón.
                        if(distanceToGoal(player, piece) <= move_number){
                            final_box = Box(0, goal, player);
                        }
                        else{
                            final_box = computeMove(current_piece.get_color(), current_piece.get_box(), move_number);
                            while (this->boxState(final_box).size() > 0)
                            {
                                move_number++;
                                //final_box = computeMove(current_piece, dice_number);
                                final_box = computeMove(current_piece.get_color(), current_piece.get_box(), move_number);
                            }
                        }

                        board.movePiece(player, piece, final_box);
                        this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, final_box));

                        for (int i = 0; i < board.getSpecialItems().size(); i++){
                            if (final_box == board.getSpecialItems()[i].box){
                                // Se borra del tablero y se añade a los dados especiales del jugador.
                                if (dice.getSpecialDice(player).size() < MAX_SPECIAL_DICES)
                                {
                                    dice.addSpecialDice(player, board.getSpecialItems()[i].type);
                                    board.deleteSpecialItem(i);

                                    this->update_board = true;
                                    this->update_dice = true;
                                    this->last_acquired = board.getSpecialItems()[i].type;
                                }
                            }
                        }
                    }
                    break;
                    case blue_shell:
                    {
                        this->blue_shell_move = true;
                        int min_dist = 1000;
                        vector<pair<color,int>> deleted_pieces;
                        for (int i = 0; i < game_colors.size(); i++){
                            color c = game_colors[i];
                            if (c != player){
                                for (int j = 0; j < board.getPieces(c).size(); j++){
                                    int dist = distanceToGoal(c, j);
                                    Piece current_piece = board.getPiece(c, j);
                                    if(current_piece.get_type() != boo_piece){
                                        if(dist < min_dist and dist > 0){
                                            min_dist = dist;
                                            deleted_pieces.clear();
                                            deleted_pieces.push_back(pair<color,int>(c,j));
                                        }else if(dist == min_dist){
                                            deleted_pieces.push_back(pair<color,int>(c,j));
                                        }
                                    }
                                }
                            }
                        }

                        for (int i = 0; i < deleted_pieces.size(); i++){
                            Piece current_piece = board.getPiece(deleted_pieces[i].first, deleted_pieces[i].second);
                            if (dice.isAvailable(deleted_pieces[i].first, horn) and !playground_mode){
                                dice.removeNumber(deleted_pieces[i].first, horn);
                            }
                            else if(current_piece.get_type() != star_piece and current_piece.get_type() != boo_piece){
                                Box origin = board.getPiece(deleted_pieces[i].first, deleted_pieces[i].second).get_box();
                                board.movePiece(deleted_pieces[i].first, deleted_pieces[i].second, Box(0, home, deleted_pieces[i].first));
                                this->last_moves.push_back(tuple<color, int, Box, Box>(deleted_pieces[i].first, deleted_pieces[i].second, origin, Box(0, home, deleted_pieces[i].first)));
                                this->pieces_destroyed_by_blue_shell.push_back({deleted_pieces[i].first, deleted_pieces[i].second});
                            }
                            if(playground_mode){
                                cout << "IMPORTANTE: SI ESTUVIERAS JUGANDO EN MODO NORMAL TENIENDO LA BOCINA LA PERDERÍAS Y TE HABRÍAS PROTEGIDO DEL CAPARAZÓN" << endl;
                            }
                        }
                    }
                    break;

                    case red_shell:
                    {
                        this->red_shell_move = true;
                        int min_dist = 1000;
                        vector<pair<color,int>> deleted_pieces;
                        for (int i = 0; i < game_colors.size(); i++){
                            color c = game_colors[i];
                            if (c != player){
                                for (int j = 0; j < board.getPieces(c).size(); j++){
                                    int dist = distanceBoxtoBox(player, piece, c, j);
                                    Piece current_piece = board.getPiece(c, j);
                                    if(current_piece.get_type() != boo_piece){
                                        if(dist < min_dist and dist > 0){
                                            min_dist = dist;
                                            deleted_pieces.clear();
                                            deleted_pieces.push_back(pair<color,int>(c,j));
                                        }else if(dist == min_dist){
                                            deleted_pieces.push_back(pair<color,int>(c,j));
                                        }
                                    }
                                }
                            }
                        }

                        for (int i = 0; i < deleted_pieces.size(); i++){
                            Piece current_piece = board.getPiece(deleted_pieces[i].first, deleted_pieces[i].second);
                            if (dice.isAvailable(deleted_pieces[i].first, horn) and !playground_mode){
                                dice.removeNumber(deleted_pieces[i].first, horn);
                            }
                            else if(current_piece.get_type() != star_piece and current_piece.get_type() != mega_piece){
                                Box origin = board.getPiece(deleted_pieces[i].first, deleted_pieces[i].second).get_box();
                                board.movePiece(deleted_pieces[i].first, deleted_pieces[i].second, Box(0, home, deleted_pieces[i].first));
                                this->last_moves.push_back(tuple<color, int, Box, Box>(deleted_pieces[i].first, deleted_pieces[i].second, origin, Box(0, home, deleted_pieces[i].first)));
                                this->pieces_destroyed_by_red_shell.push_back({deleted_pieces[i].first, deleted_pieces[i].second});
                            }
                            if(playground_mode){
                                cout << "IMPORTANTE: SI ESTUVIERAS JUGANDO EN MODO NORMAL TENIENDO LA BOCINA LA PERDERÍAS Y TE HABRÍAS PROTEGIDO DEL CAPARAZÓN" << endl;
                            }
                        }
                    }
                    break;
                    case mega_mushroom:
                    {
                        this->mega_mushroom_move = true;
                        board.setPieceType(player, piece, mega_piece);
                        board.setPieceTurnsLeft(player, piece, 4);

                        Piece eating_piece = current_piece;
                        vector<pair <color, int>> box_states = boxState(current_piece.get_box());
                        for (int i = 0; i < box_states.size(); i++){
                            if (box_states[i].first != player){
                                eating_piece = board.getPiece(box_states[i].first, box_states[i].second);
                                if (eating_piece.get_type() != boo_piece){
                                    Box origen_comida = board.getPiece(box_states[i].first, box_states[i].second).get_box();
                                    board.movePiece(box_states[i].first, box_states[i].second, Box(0, home, box_states[i].first));
                                    this->last_moves.push_back(tuple<color, int, Box, Box>(box_states[i].first, box_states[i].second, origen_comida, Box(0, home, box_states[i].first)));
                                }

                            }
                        }

                        vector<pair <color, int>> next_box_states = boxState(nextBox(player, current_piece.get_box()));
                        for (int i = 0; i < next_box_states.size(); i++){
                            if (next_box_states[i].first != player){
                                eating_piece = board.getPiece(next_box_states[i].first, next_box_states[i].second);
                                if (eating_piece.get_type() != boo_piece){
                                    Box origen_comida = board.getPiece(next_box_states[i].first, next_box_states[i].second).get_box();
                                    board.movePiece(next_box_states[i].first, next_box_states[i].second, Box(0, home, next_box_states[i].first));
                                    this->last_moves.push_back(tuple<color, int, Box, Box>(next_box_states[i].first, next_box_states[i].second, origen_comida, Box(0, home, next_box_states[i].first)));
                                }
                            }
                        }

                        for (int i = 0; i < board.getSpecialItems().size(); i++)
                        {
                            if (current_piece.get_box() == board.getSpecialItems()[i].box)
                            {
                                // Se borra del tablero y se añade a los dados especiales del jugador.
                                if (dice.getSpecialDice(player).size() < MAX_SPECIAL_DICES)
                                {
                                    dice.addSpecialDice(player, board.getSpecialItems()[i].type);
                                    board.deleteSpecialItem(i);

                                    this->update_board = true;
                                    this->update_dice = true;
                                    this->last_acquired = board.getSpecialItems()[i].type;
                                }
                            }

                            if(nextBox(player, current_piece.get_box()) == board.getSpecialItems()[i].box)
                            {
                                // Se borra del tablero y se añade a los dados especiales del jugador.
                                if (dice.getSpecialDice(player).size() < MAX_SPECIAL_DICES)
                                {
                                    dice.addSpecialDice(player, board.getSpecialItems()[i].type);
                                    board.deleteSpecialItem(i);

                                    this->update_board = true;
                                    this->update_dice = true;
                                    this->last_acquired = board.getSpecialItems()[i].type;
                                }
                            }
                        }

                    }
                    break;
                    case shock:
                    {
                        this->shock_move = true;
                        //Convertimos las fichas de otros colores a small_piece
                        for (int i = 0; i < game_colors.size(); i++){
                            color c = game_colors[i];
                            if (c != player){
                                for (int j = 0; j < board.getPieces(c).size(); j++){
                                    //Si la ficha es de tipo mega_piece, se convierte en normal_piece
                                    if (board.getPiece(c, j).get_type() == mega_piece){
                                        board.setPieceType(c, j, normal_piece);
                                        board.setPieceTurnsLeft(c, j, 4);
                                    //Si no es de tipo star_piece o boo_piece, se convierte en small_piece
                                    }else if(board.getPiece(c, j).get_type() != star_piece and board.getPiece(c, j).get_type() != boo_piece){
                                        board.setPieceType(c, j, small_piece);
                                        board.setPieceTurnsLeft(c, j, 4);
                                    }
                                }
                            }
                        }

                        //Eliminamos el último dado especial conseguido por el adversario (siempre que haya)
                        if(!dice.getSpecialDice(opponent_color(player)).empty() and !playground_mode){
                            dice.removeNumber(opponent_color(player), dice.getSpecialDice(opponent_color(player)).back());
                        }else if(playground_mode){
                            cout << "IMPORTANTE: SI ESTUVIERAS JUGANDO EN MODO NORMAL EL ÚLTIMO DADO ESPECIAL DEL ADVERSARIO SE ELIMINARÍA AL USAR EL RAYO" << endl;
                        }
                    }
                    break;

                    case horn:
                    {
                        this->horn_move = true;
                        vector<pair<color,int>> deleted_pieces;
                        for (int i = 0; i < game_colors.size(); i++){
                            color c = game_colors[i];
                            if (c != player){
                                for (int j = 0; j < board.getPieces(c).size(); j++){
                                    Piece horny_piece = board.getPiece(c, j);
                                    if(horny_piece.get_type() != boo_piece and horny_piece.get_type() != mega_piece and horny_piece.get_type() != star_piece and horny_piece.get_box().type != goal){
                                        int dist_paforward = distanceBoxtoBox(player, piece, c, j);
                                        int dist_paantes = distanceBoxtoBox(c, j, player, piece);
                                        if (dist_paforward <= 2 and dist_paforward >= 0 or dist_paantes <= 2 and dist_paantes >= 0){
                                            deleted_pieces.push_back(pair<color, int>(c, j));
                                        }
                                    }
                                }
                            }
                        }

                        this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, piece_box));

                        for (int i = 0; i < deleted_pieces.size(); i++){
                            Piece current_piece = board.getPiece(deleted_pieces[i].first, deleted_pieces[i].second);
                            Box origin = board.getPiece(deleted_pieces[i].first, deleted_pieces[i].second).get_box();
                            board.movePiece(deleted_pieces[i].first, deleted_pieces[i].second, Box(0, home, deleted_pieces[i].first));
                            this->last_moves.push_back(tuple<color, int, Box, Box>(deleted_pieces[i].first, deleted_pieces[i].second, origin, Box(0, home, deleted_pieces[i].first)));
                            this->pieces_destroyed_by_horn.push_back({deleted_pieces[i].first, deleted_pieces[i].second});
                        }
                    }
                    break;

                    case banana:
                    {
                        this->banana_move = true;
                        board.addTrap(banana_trap, current_piece.get_box());
                        this->update_board = true;
                    }
                    break;
                }

            }

            nextTurn();
            turn++;
            last_action = tuple<color, int, int>(player, piece, dice_number);

        }
        else{
            illegal_move_player = current_player;
        }
    }
}

bool Parchis::isLegalMove(const Piece & piece, int dice_number) const{
    color player = piece.get_color();
    Box box = piece.get_box();
    special_type type = piece.get_type();

    if(gameOver())
        return false;
    // Controlar si intento mover una ficha que no es del color del jugador actual.
    if(player != current_color and player != partner_color(current_color))
        return false;
    // Controlar si intento mover un número que no está disponible en mis dados.
    if(!dice.isAvailable(player, dice_number))
        return false;
    // Controlar si intento contar un número distinto de 20 cuando me he comido una ficha.
    if(isEatingMove() && dice_number != 20)
        return false;
    // Controlar si intento contar un número distinto de 10 cuando he llevado una ficha a la meta.
    if(isGoalMove() && dice_number != 10)
        return false;
    // No dejar trampas en meta o pasillo
    if((box.type == goal || box.type == final_queue || box.type == home) and dice_number == banana)
        return false;
    // Control de movimientos
    Box final_box = computeMove(piece, dice_number);
    // Controlar si barreras, si está en la casa el movimiento solo sería legal si dice_number == 5, ...
    // La bala también permite sacar a la ficha de casa. Los caparazones también se pueden aplicar sobre fichas en casa.
    if (box.type == home && dice_number != 5 &&
        dice_number != bullet && dice_number != red_shell && dice_number != blue_shell &&
        dice_number != shock)
        return false;

    // Los objetos especiales con target global (rayo, caparazón azul, ...) solo se pueden usar sobre la ficha con id 0
    // TODO

    //Controlar que ya estés en la meta
    else if(box.type == goal)
        return false;
    // Comprobar que no haya ya dos fichas en la casilla (a menos que la casilla de destino sea home, meta o la misma que la de partida). Si voy en estrella me daría igual salvo que haya dos estrellas en el destino.
    else if (final_box.type != goal && final_box.type != home && !(final_box == box) && boxState(final_box).size() == 2){
        if(type != star_piece)
            return false;
        else{
            vector<pair<color, int>> box_state = boxState(final_box);
            Piece p1 = board.getPiece(box_state[0].first, box_state[0].second);
            Piece p2 = board.getPiece(box_state[1].first, box_state[1].second);
            if(p1.get_type() == star_piece and p2.get_type() == star_piece)
                return false;
        }
    }
    // Comprobar que no haya una mega ficha en la casilla.
    else if (final_box.type != goal && final_box.type != home && !(final_box == box) && boxState(final_box).size() == 1){
        vector<pair<color, int>> box_state = boxState(final_box);
        Piece p = board.getPiece(box_state[0].first, box_state[0].second);
        if(p.get_type() == mega_piece)
            return false;
    }
    //Controlar los muros si la pieza no es boo ni estrella ni mega
    if(type != boo_piece and type != star_piece and type != mega_piece){
        // Comprobar que en el camino del movimiento no hay barreras
        vector<color> walls = anyWall(box, final_box);
        for (int i = 0; i < walls.size(); i++){
            if (walls.at(i) != player){
                return false;
            }
        }
    }

    //Controlar los mega muros si la pieza no es boo
    if(type != boo_piece){
        // Comprobar que en el camino del movimiento no hay mega barreras
        vector<color> walls = anyMegaWall(box, final_box);
        for (int i = 0; i < walls.size(); i++){
            if (walls.at(i) != player){
                return false;
            }
        }
    }

    if (type == mega_piece or dice_number == mega_mushroom){
        //Los mega muros de la siguiente casilla también afectan
        if (isMegaWall(nextBox(player, final_box)) != none) {
            return false;
        }

        //Al hacerse grande no puede compartir casilla con una ficha del mismo color ni con una estrella.
        //Si hay solo una ficha en mi casilla acrtual soy yo, así que no me afecta.
        //Si hay dos fichas en mi casilla actual, solo puedo hacerme grande si la que no soy yo no es de mi color ni estrella.
        if (dice_number == mega_mushroom and boxState(final_box).size() > 1){
            // Si las dos fichas son de mi color, la ficha con la que comparto es de mi color y no puedo hacerme grande.
            if (boxState(final_box)[0].first == player and boxState(final_box)[1].first == player){
                return false;
            }
            // Si una de las fichas es de mi color y la otra no, la ficha que no es de mi color no puede ser estrella.
            else if (boxState(final_box)[0].first == player){
                if (board.getPiece(boxState(final_box)[1].first, boxState(final_box)[1].second).get_type() == star_piece){
                    return false;
                }
            }
            else if (boxState(final_box)[1].first == player){
                if (board.getPiece(boxState(final_box)[0].first, boxState(final_box)[0].second).get_type() == star_piece){
                    return false;
                }
            }
        }
        else if(dice_number < 100){
            // Si se está moviendo, simplemente comprobar que en la casilla de llegada no haya de mi color ni estrella.
            for(int i = 0; i < boxState(final_box).size(); i++){
                if (boxState(final_box)[i].first == player or board.getPiece(boxState(final_box)[i].first, boxState(final_box)[i].second).get_type() == star_piece){
                    return false;
                }
            }
        }

        Box next_box = nextBox(player, final_box);
        for (int i = 0; i < boxState(next_box).size(); i++){
            if (boxState(next_box)[i].first == player or board.getPiece(boxState(next_box)[i].first, boxState(next_box)[i].second).get_type() == star_piece){
                return false;
            }
        }
    }

    // Comprobar que si he sacado un 6 y tengo barreras de mi color he elegido una de las fichas en la barrera.
    if(dice_number == 6){
        bool hay_walls = false;
        for(int i = 0; i < board.getPieces(player).size() && !hay_walls; i++){
            hay_walls = (isWall(board.getPiece(player, i).get_box()) == player);
        }
        for(int i = 0; i < board.getPieces(partner_color(player)).size() && !hay_walls; i++){
            hay_walls = (isWall(board.getPiece(partner_color(player), i).get_box()) == partner_color(player));
        }

        if(hay_walls && isWall(box) != player){
            return false;
        }
    }
    return true;
}

bool Parchis::canSkipTurn(color player, int dice_number) const{
    return dice.isAvailable(player, dice_number) and getAvailablePieces(player, dice_number).size() == 0 and getAvailablePieces(partner_color(player), dice_number).size() == 0;
}

const vector<pair <color, int>> Parchis::boxState(const Box & box) const{
    vector<pair <color, int>> occupation;

    for (int c = color::blue; c < color::none; c++){
        color col = (color)c;
        for (int i = 0; i < board.getPieces(col).size(); i++){
            if (board.getPiece(col, i).get_box() == box ){
                occupation.push_back(pair<color, int>(col, i));
            }
        }
    }

    return occupation;
}

const Box Parchis::computeMove(const Piece & piece, int dice_number, bool * goal_bounce) const{
    Box final_box;
    color player = piece.get_color();
    Box piece_box = piece.get_box();
    special_type type = piece.get_type();

    if(goal_bounce != NULL) *goal_bounce = false;

    if(dice_number > 100) return piece_box;

    //Si sale un 6, comprobar si se da condición para avanzar 7 o 6
    if(dice_number == 6){
        bool pieces_out = true;
        for (int i = 0; i < board.getPieces(player).size() && pieces_out; i++){
            if (board.getPieces(player).at(i).get_box().type == home){
                pieces_out = false;
            }
        }
        if (pieces_out){
            dice_number = 7;
        }
    }
    if (type == star_piece){
        dice_number += 2;
    }else if (type == small_piece){
        dice_number = dice_number/2;
    }
    else if(type == bananed_piece){
        dice_number = 0;
    }
    //Si sale de la casilla de home
    if (piece_box.type == home){
        switch (player){
            case red:
                final_box = Box(init_red_box, normal, none);
                break;
            case blue:
                final_box = Box(init_blue_box, normal, none);
                break;
            case yellow:
                final_box = Box(init_yellow_box, normal, none);
                break;
            case green:
                final_box = Box(init_green_box, normal, none);
                break;
        }
    }
    //Condiciones para empezar a avanzar por pasillo de meta
    else if(piece_box.num <= final_boxes.at(player) && piece_box.num + dice_number > final_boxes.at(player) && piece_box.type == normal){
        int count = piece_box.num + dice_number - final_boxes.at(player);
        if (count <= 7){
            final_box = Box(count, final_queue, player);
        }else if (count == 8){
            final_box = Box(0, goal, player);
        }else{
            int diff = 16 - (count);
            if(diff > 0){
                final_box = Box(diff, final_queue, player);
            }else{
                final_box = Box(final_boxes.at(player) + diff, normal, none);
            }
            if(goal_bounce != NULL) *goal_bounce = true;
        }

    }else if(piece_box.num + dice_number > 68 && piece_box.num + dice_number - 68 > final_boxes.at(player) && piece_box.type == normal){
        int count = piece_box.num + dice_number - 68 - final_boxes.at(player);
        if (count <= 7){
            final_box = Box(count, final_queue, player);
        }else if (count == 8){
            final_box = Box(0, goal, player);
        }else{
            int diff = 16 - (count);
            if(diff > 0){
                final_box = Box(diff, final_queue, player);
            }else{
                final_box = Box(final_boxes.at(player) + diff, normal, none);
            }
            if(goal_bounce != NULL) *goal_bounce = true;
        }
    }
    //Si ya está en pasillo de meta
    else if(piece_box.type == final_queue){
        if (piece_box.num + dice_number <= 7){
            final_box = Box(piece_box.num + dice_number, final_queue, player);
        }else if (piece_box.num + dice_number == 8){
            final_box = Box(0, goal, player);
        }else{
            int diff = 16 - (piece_box.num + dice_number);
            if(diff > 0){
                final_box = Box(diff, final_queue, player);
            }else{
                final_box = Box(final_boxes.at(player) + diff, normal, none);
            }
            if(goal_bounce != NULL) *goal_bounce = true;
        }
    }
    //Por defecto
    else{
        final_box = Box(1 + (piece_box.num+ dice_number - 1) % 68, box_type::normal, color::none);
    }

    return final_box;
}

const Box Parchis::computeReverseMove(const Piece &piece, int dice_number) const
{
    /* REALMENTE EL ESTADO DE LA FICHA NO SE NECESITA, TAMPOCO CONTARSE 7 HACIA ATRÁS 
    Box initial_box;
    color player = piece.get_color();
    Box piece_box = piece.get_box();
    special_type type = piece.get_type();

    if (dice_number > 100)
        return piece_box;

    // Si sale un 6, comprobar si se da condición para avanzar 7 o 6
    if (dice_number == 6)
    {
        bool pieces_out = true;
        for (int i = 0; i < board.getPieces(player).size() && pieces_out; i++)
        {
            if (board.getPieces(player).at(i).get_box().type == home)
            {
                pieces_out = false;
            }
        }
        if (pieces_out)
        {
            dice_number = 7;
        }
    }
    if (type == star_piece)
    {
        dice_number += 2;
    }
    else if (type == small_piece)
    {
        dice_number = dice_number / 2;
    }
    else if (type == bananed_piece)
    {
        dice_number = 0;
    }
    */
    Box initial_box;
    color player = piece.get_color();
    Box piece_box = piece.get_box();

    // Si está en meta o en pasillo de meta (aunque realmente aquí ya estás a salvo?)
    if (piece_box.type == goal){
        if(dice_number <= 7 and dice_number > 0){
            initial_box = Box(8 - dice_number, final_queue, player);
        }
        else if(dice_number == 0){
            initial_box = Box(0, goal, player);
        }
        else{
            int final_pos;
            switch(player){
                case red:
                    // Casilla de entrada al pasillo.
                    final_pos = final_red_box;
                    break;
                case blue:
                    final_pos = final_blue_box;
                    break;
                case yellow:
                    final_pos = final_yellow_box;
                    break;
                case green:
                    final_pos = final_green_box;
                    break;
            }
            // Si hacia atrás no me paso la casilla 1, me muevo hacia atrás normal.
            if(final_pos - (dice_number - 8) > 0){
                initial_box = Box(final_pos - (dice_number - 8), normal, none);
            }
            else{
                // Si me paso la casilla 1, lo que sobra empiezo a contarlo desde la casilla 68.
                initial_box = Box(68 - (dice_number - 8 - final_pos), normal, none);
            }
        }
    }
    else if(piece_box.type == final_queue){
        // Igual pero con el pasillo de meta.
        int final_queue_pos = piece_box.num;
        if(dice_number < final_queue_pos){
            initial_box = Box(final_queue_pos - dice_number, final_queue, player);
        }
        else{
            int final_pos;
            switch(player){
                case red:
                    // Casilla de entrada al pasillo.
                    final_pos = final_red_box;
                    break;
                case blue:
                    final_pos = final_blue_box;
                    break;
                case yellow:
                    final_pos = final_yellow_box;
                    break;
                case green:
                    final_pos = final_green_box;
                    break;
            }
            // Si hacia atrás no me paso la casilla 1, me muevo hacia atrás normal.
            if(final_pos - (dice_number - final_queue_pos) > 0){
                initial_box = Box(final_pos - (dice_number - final_queue_pos), normal, none);
            }
            else{
                // Si me paso la casilla 1, lo que sobra empiezo a contarlo desde la casilla 68.
                initial_box = Box(68 - (dice_number - final_queue_pos - final_pos), normal, none);
            }
        }
    }
    // Si estoy en home, na
    else if(piece_box.type == home){
        initial_box = piece_box;
    }
    else{
        // Si no, muevo hacia atrás normal.
        if(piece_box.num - dice_number > 0){
            initial_box = Box(piece_box.num - dice_number, normal, none);
        }
        else{
            initial_box = Box(68 - (dice_number - piece_box.num), normal, none);
        }
    }
    return initial_box;
}


const Box Parchis::computeSpecialMove(const Piece & piece, int dice_number) const{
    Box final_box;
    color player = piece.get_color();
    Box piece_box = piece.get_box();
    special_type type = piece.get_type();

    // Si el dado es normal, devolver el computeMove normal (por ejemplo)
    if(isNormalDice(dice_number)){
        return computeMove(piece, dice_number);
    }
    else{
        if(dice_number == bullet or dice_number == mushroom){
            int move_number = (dice_number == bullet)? 40:8;

            final_box = computeMove(player, piece_box, move_number);
            while (this->boxState(final_box).size() > 0)
            {
                move_number++;
                // final_box = computeMove(current_piece, dice_number);
                final_box = computeMove(player, piece_box, move_number);
            }
            return final_box;
        }
        else{
            return piece_box;
        }
    }

}


void Parchis::gameLoop(){
    // Incializar el juego para los jugadores por primera vez
    for (int i = 0; i < players.size(); i++)
    {
        players.at(i)->perceive(*this);
    }

    cout << "++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "¡COMIENZA LA PARTIDA!" << endl;
    cout << "Jugador 1: " << players.at(0)->getName() << endl;
    cout << "Jugador 2: " << players.at(1)->getName() << endl;
    cout << "++++++++++++++++++++++++++++++++++++++" << endl;

    // Bucle principal del juego.
    while (!gameOver()){
        // :)
        gameStep();
    }

    // Gestionar final de la partida.
    if (gameOver())
    {
        cout << "++++++++++++++++++++++++" << endl;
        cout << "La partida ha terminado" << endl;
        int winner = getWinner();
        color winner_color = getColorWinner();

        cout << "Ha ganado el jugador " << 1 + winner << " (" << str(winner_color) << ")" << endl;
        cout << "¡¡¡ENHORABUENA, " << getPlayers().at(winner)->getName() << "!!!" << endl;
        //cout << "Ha ganado el jugador " << winner << endl;
        if (illegalMove())
        {
            cout << "El jugador " << 1 + (winner == 1 ? 0 : 1) << " ha hecho un movimiento ilegal" << endl;
        }
        if(overBounce())
        {
            cout << "El jugador " << 1 + (winner == 1 ? 0 : 1) << " ha excedido el límite de rebotes." << endl;
        }
        cout << "++++++++++++++++++++++++" << endl;
    }
}

bool Parchis::gameStep(){
    cout << "----------------" << endl;
    cout << "Turno: " << turn << endl;
    cout << "Jugador actual: " << this->current_player+1 << " (" << this->players.at(current_player)->getName() << ")" << endl;
    cout << "----------------" << endl;

    // El jugador actual hace su movimiento.
    bool move = players.at(this->current_player)->move();

    // Se notifica el movimiento a todos los jugadores.
    for (int i = 0; i < players.size(); i++)
    {
        players.at(i)->perceive(*this);
    }

    //Se notifica el movimiento a todos los viewers.
    for (int i = 0; i < viewers.size(); i++)
    {
        viewers.at(i)->perceive(*this);
    }

    // Se espera a que todos los jugadores estén listos para el siguiente turno.
    waitForPlayers();

    return true;
}

void Parchis::waitForPlayers() const{
    // Sleep 10 milliseconds mientras haya un jugador que aún no esté listo para
    // el siguiente turno.
    // Comprobamos tanto jugadores como viewers.
    vector<bool> ready_players(players.size(), false);
    vector<bool> ready_viewers(viewers.size(), false);
    bool ready = false;
    while (!ready){
        ready = true;
        for (int i = 0; i < players.size(); i++){
            if(!ready_players[i]){
                ready_players[i] = players.at(i)->readyForNextTurn();
                ready = ready && ready_players[i];
            }
        }
        for (int i = 0; i < viewers.size(); i++){
            if(!ready_viewers[i]){
                ready_viewers[i] = viewers.at(i)->readyForNextTurn();
                ready = ready && ready_viewers[i];
            }
        }
        if(!ready) sleep(milliseconds(10));
    }
}

bool Parchis::gameOver() const{
    return getWinner() != -1;
}

void Parchis::endGame(){
    this->disconnected_player = current_player;
}

int Parchis::getWinner() const{
    if(illegal_move_player != -1){
        return (illegal_move_player == 0)?1:0;
    }
    if(disconnected_player != -1){
        return (disconnected_player == 0)?1:0;
    }
    if(overbounce_player != -1){
        return (overbounce_player == 0)?1:0;
    }
    color col = getColorWinner();
    switch(col){
        case yellow:
        case red:
            return 0;
        case blue:
        case green:
            return 1;

        default:
            return -1;

        break;
    }

    //Recorro mis colores
    /*vector<color> my_colors = this->getPlayerColors(0);
    bool he_ganao = true;
    for(int i = 0; i < my_colors.size() && he_ganao; i++){
        color col = my_colors.at(i);
        Box goal(0, box_type::goal, col);

        if(boxState(goal).size() != board.getPieces(col).size()){
            he_ganao = false;
        }
    }

    if(he_ganao){
        return 0;
    }else{
        // Recorro los colores de mi oponente
        vector<color> other_colors = this->getPlayerColors(1);
        bool he_perdio  = true;
        for(int i = 0; i < other_colors.size() && he_perdio; i++){
            color col = other_colors.at(i);
            Box goal(0, box_type::goal, col);

            if(boxState(goal).size() != board.getPieces(col).size()){
                he_perdio = false;
            }
        }
        if(he_perdio){
            return 1;
        }else{
            return -1;
        }
    }*/
}

color Parchis::getColorWinner() const{
    // Recorro todos los colores. En principio da igual el orden, solo debería encontrarse como mucho un ganador.
    for(int i = 0; i < Parchis::game_colors.size(); i++){
        color col = Parchis::game_colors.at(i);
        Box goal(0, box_type::goal, col);

        if(boxState(goal).size() == board.getPieces(col).size()){
            return col;
        }
    }
    return none;
}

bool Parchis::illegalMove() const{
    return this->illegal_move_player != -1;
}

bool Parchis::overBounce() const{
    return this->overbounce_player != -1;
}

Parchis Parchis::generateNextMoveDescending(color & c_piece,  int & id_piece, int & dice) const{
    color main_color = this->getCurrentColor();
    bool change_dice = false;
    vector<int> current_dices = this->getAllAvailableDices(main_color);
    vector<tuple<color,int>> current_pieces;
    int curr_dice_value = -1;

    bool check_skip = false;

    //Si dice tiene valor por defecto, cojo el último.
    if (dice == -1){
        dice = current_dices.size() - 1;
    }
    curr_dice_value = current_dices.at(dice);

    do{
        current_pieces = this->getAvailablePieces(main_color, curr_dice_value);
        change_dice = false;
        check_skip = false;
        // Busco el par (c_piece, id_piece) que moví la última vez.
        // Si current_pieces está vacío, miro si puedo pasar de turno.
        // Si id_piece es -1, me quedo el primero.
        // Si no, me quedo el siguiente.
        if(!current_pieces.empty()){
            if(id_piece == -1){
                c_piece = get<0>(current_pieces.front());
                id_piece = get<1>(current_pieces.front());
            }else{
                for(int i = 0; i < current_pieces.size(); i++){
                    if(get<0>(current_pieces.at(i)) == c_piece && get<1>(current_pieces.at(i)) == id_piece){
                        if(i == current_pieces.size() - 1){
                            check_skip = true;
                        }else{
                            c_piece = get<0>(current_pieces.at(i+1));
                            id_piece = get<1>(current_pieces.at(i+1));
                        }
                        break;
                    }
                }
            }
        }
        else{
            check_skip = true;
        }

        // si tengo que comprobar si puedo pasar de turno, lo hago.
        // Si puedo, id_piece = SKIP_TURN
        // Si no puedo, cambio el dado.
        if(check_skip){
            if(this->canSkipTurn(main_color, curr_dice_value) && id_piece != SKIP_TURN){
                id_piece = SKIP_TURN;
                change_dice = false;
                c_piece = main_color;
            }else{
                change_dice = true;
            }
        }

        // Si tengo que cambiar de dado, lo hago.
        if(change_dice){
            //Los recorro en orden DESCENDENTE
            if(dice == 0){
                return *this;
            }else{
                dice--;
                curr_dice_value = current_dices.at(dice);
                id_piece = -1;
            }
        }
    }while(change_dice);

    Parchis next_move(*this);
    next_move.movePiece(c_piece, id_piece, curr_dice_value);
    return next_move;
}

void Parchis::setPlaygroundMode(){
    this->playground_mode = true;
    // Cambiar el tablero.
    this->board = Board(PLAYGROUND);
    // Add the special dices to the players.
    this->dice.addSpecialDice(yellow, banana);
    this->dice.addSpecialDice(blue, mushroom);
    this->dice.addSpecialDice(yellow, red_shell);
    this->dice.addSpecialDice(blue, horn);
    this->dice.addSpecialDice(yellow, bullet);
    this->dice.addSpecialDice(blue, blue_shell);
    this->dice.addSpecialDice(yellow, boo);
    this->dice.addSpecialDice(blue, shock);
    this->dice.addSpecialDice(yellow, star);
    this->dice.addSpecialDice(blue, mega_mushroom);
}

/***************************************** PARCHISBROS *****************************************/

ParchisBros Parchis::getChildren() const{
    return ParchisBros(*this);
}

ParchisBros::ParchisBros(const Parchis &p){
    this->parent = make_shared<Parchis>(p);
}

ParchisBros::Iterator ParchisBros::begin(){
    color last_c_piece = none;
    int last_id_piece = -1;
    int last_dice = -1;
    shared_ptr<Parchis> nuevo_hijo = make_shared<Parchis>(parent->generateNextMoveDescending(last_c_piece, last_id_piece, last_dice));
    return ParchisBros::Iterator(*this, nuevo_hijo, last_c_piece, last_id_piece, last_dice);
}

ParchisBros::Iterator::Iterator(ParchisBros & out, shared_ptr<Parchis> &p, color last_c_piece, int last_id_piece, int last_dice)
: container(&out){
    this->actual = p;
    this->last_c_piece = last_c_piece;
    this->last_id_piece = last_id_piece;
    this->last_dice = last_dice;
    if (last_dice != -1)
        this->dice_value = container->parent->getAllAvailableDices(container->parent->getCurrentColor()).at(last_dice);
}

ParchisBros::Iterator & ParchisBros::Iterator::operator++(){
    actual = make_shared<Parchis>(container->parent->generateNextMoveDescending(last_c_piece, last_id_piece, last_dice));
    if (last_dice != -1)
        this->dice_value = container->parent->getAllAvailableDices(container->parent->getCurrentColor()).at(last_dice);
    return *this;
}

bool ParchisBros::Iterator::operator==(const ParchisBros::Iterator & it) const{
    return (*(this->actual) == *(it.actual));
}

bool ParchisBros::Iterator::operator!=(const ParchisBros::Iterator & it) const{
    return !(*(this->actual) == *(it.actual));
}

Parchis & ParchisBros::Iterator::operator*() const{
    return *actual;
}

ParchisBros::Iterator ParchisBros::end(){
    return ParchisBros::Iterator(*this, parent, none, -1, -1);
}

/**************************** MÉTODOS PARA LA HEURÍSTICA *********************/

int Parchis::piecesAtGoal(color col) const{
    Box goal(0, box_type::goal, col);
    return boxState(goal).size();
}

int Parchis::piecesAtHome(color col) const{
    Box home(0, box_type::home, col);
    return boxState(home).size();
}

int Parchis::distanceToGoal(color player, const Box & box) const{
    //Calculo número de casillas hasta llegar a la meta
    switch(box.type){
        case normal:
            //En caso de casilla normal, el número de casillas hasta la entrada
            //al pasillo final, + 8 casillas del pasillo.
            if (box.num > final_boxes.at(player)){
                return 68 - box.num + final_boxes.at(player) + 8;
            }else{
                return final_boxes.at(player) - box.num + 8;
            }
        case goal:
            //En caso de estar en la meta, 0.
            return 0;
        case final_queue:
            //En caso de estar en el pasillo final, el número de casillas que te
            //queden hasta el 8.
            return 8 - box.num;
        case home:
            //El máximo, cuando estás en home. 65 de moverte hasta la entrada al pasillo
            //final, + 8 casillas del pasillo, + 1 por tener que salir de la casa.
            return 1 + 65 + 8;
        default:
            return -1;
    }
}

int Parchis::distanceToGoal(color player, int id_piece) const{
    return distanceToGoal(player, this->board.getPiece(player, id_piece).get_box());
}

int Parchis::distanceBoxtoBox(color player, const Box & box1, const Box & box2) const{
    Box ref_box1 = box1;
    Box ref_box2 = box2;

    // Reducimos el problema a distancia entre casillas normales.
    switch(box2.type){
        case normal:
            ref_box2 = box2;
            break;
        case goal:
        case final_queue:
            ref_box2 = Box(final_boxes.at(box2.col), normal, none);
            break;
        case home:
            ref_box2 = Box(init_boxes.at(box2.col), normal, none);
            break;
    }
    switch(box1.type){
        case normal:
            ref_box1 = box1;
            break;
        case goal:
        case final_queue:
            ref_box1 = Box(final_boxes.at(box1.col), normal, none);
            break;
        case home:
            ref_box1 = Box(init_boxes.at(box1.col), normal, none);
            break;
    }

    // Casos inalcanzables (espacios de color único).
    if(box2.type != normal && player != box2.col){
        return -1;
    }

    // Para el resto de casos calculamos la distancia "normal" y luego añadimos los extras.
    int distance = 0;

    // Si mi pasillo está por medio es inalcanzable.
    if(ref_box1.num <= final_boxes.at(player) && final_boxes.at(player) < ref_box2.num){
        return -1;
    }
    // Si mi pasillo está por delante, y la casilla destino después del 68, inalcanzable.
    if(ref_box1.num > ref_box2.num && ref_box1.num <= final_boxes.at(player)){
        return -1;
    }
    // Si mi pasillo está antes de la casilla destino, y parto de algo mayor, inalcanzable.
    if(ref_box1.num > ref_box2.num && final_boxes.at(player) < ref_box2.num){
        return -1;
    }
    // En caso contrario, es alcanzable.
    // Si el destino está por encima, devuelvo la diferencia.
    if(ref_box2.num >= ref_box1.num){
        distance = ref_box2.num - ref_box1.num;
    }
    // Si el destino está por debajo, devuelvo la distancia al 68 más lo que me queda hasta el destino.
    else{
        distance = 68 - box1.num + box2.num;
    }

    // Añadimos los "extras".
    switch(box1.type){
        case home:
            distance += 1;
            break;
        case final_queue:
            distance -= box1.num;
            break;
        case goal:
            distance -= 8;
            break;
    }
    switch(box2.type){
        case home:
            distance -= 1;
            break;
        case final_queue:
            distance += box2.num;
            break;
        case goal:
            distance += 8;
            break;
    }

    // Si ha salido negativo es por cosas como que ambas casillas son del pasillo final y la primera está adelantada
    // --> Inalcanzable (salvo rebote).
    if(distance < 0){
        return -1;
    }

    return distance;
}

int Parchis::distanceBoxtoBox(color player1, int id_p1, color player2, int id_p2) const{
    return distanceBoxtoBox(player1, this->board.getPiece(player1, id_p1).get_box(), this->board.getPiece(player2, id_p2).get_box());
}

bool Parchis::isSafeBox(const Box & box) const{
    return (box.type == normal && find(safe_boxes.begin(), safe_boxes.end(), box.num) != safe_boxes.end());
}

bool Parchis::isSafePiece(const color & player, const int & piece) const{
    return isSafeBox(this->board.getPiece(player, piece).get_box());
}

const color Parchis::isMegaWall(const Box & b) const{
    if(b.type == home || b.type == goal) return none;

    const vector<pair <color, int>> occupation = boxState(b);
    if (occupation.size() == 1 && board.getPiece(occupation.at(0).first, occupation.at(0).second).get_type() == mega_piece){
        return occupation.at(0).first;
    }else{
        return none;
    }
}

const color Parchis::isWall(const Box & b) const{
    if(b.type == home || b.type == goal) return none;

    const vector<pair <color, int>> occupation = boxState(b);
    if (occupation.size() == 2 && occupation.at(0).first == occupation.at(1).first){
        Piece p1 = this->board.getPiece(occupation.at(0).first, occupation.at(0).second);
        Piece p2 = this->board.getPiece(occupation.at(1).first, occupation.at(1).second);

        if (p1.get_type() != boo_piece and p2.get_type() != boo_piece and p1.get_type() != small_piece and p2.get_type() != small_piece){
            return occupation.at(0).first;
        }else{
            return none;
        }
    }else{
        return none;
    }
}

const vector<color> Parchis::anyMegaWall(const Box & b1, const Box & b2) const{
    Box final_box;
    if (b2.type == final_queue || b2.type == goal){
        //Si el casilla destino es meta o pasillo final, la cambiamos por la última casilla
        //antes de entrar al pasillo final.
        switch (b2.col){
            case blue:
                final_box = Box(final_blue_box, normal, none);
            break;
            case red:
                final_box = Box(final_red_box, normal, none);
            break;
            case green:
                final_box = Box(final_green_box, normal, none);
            break;
            case yellow:
                final_box = Box(final_yellow_box, normal, none);
            break;
        }
    }else{
        final_box = b2;
    }
    vector<color> walls;
    bool reached_final_box = false;
    if (b1.type == normal && final_box.num != b1.num){
        for (int i = b1.num % 68 + 1; !reached_final_box; i = i%68 + 1){ //Vamos recorriendo casillas intermedias
            reached_final_box = (final_box.num == i);
            //Si hay un muro, lo añadimos al vector de muros.
            color c = isMegaWall(Box(i, normal, none));
            if(c != none){
                walls.push_back(c);
            }
        }
    }
    return walls;
}

const vector<color> Parchis::anyWall(const Box & b1, const Box & b2) const{
    Box final_box;
    if (b2.type == final_queue || b2.type == goal){
        //Si el casilla destino es meta o pasillo final, la cambiamos por la última casilla
        //antes de entrar al pasillo final.
        switch (b2.col){
            case blue:
                final_box = Box(final_blue_box, normal, none);
            break;
            case red:
                final_box = Box(final_red_box, normal, none);
            break;
            case green:
                final_box = Box(final_green_box, normal, none);
            break;
            case yellow:
                final_box = Box(final_yellow_box, normal, none);
            break;
        }
    }else{
        final_box = b2;
    }
    vector<color> walls;
    bool reached_final_box = false;
    if (b1.type == normal && final_box.num != b1.num){
        for (int i = b1.num % 68 + 1; !reached_final_box; i = i % 68 + 1)
        { // Vamos recorriendo casillas intermedias
            reached_final_box = (final_box.num == i);
            //Si hay un muro, lo añadimos al vector de muros.
            color c = isWall(Box(i, normal, none));
            if(c != none){
                walls.push_back(c);
            }
        }
    }
    return walls;
}

const vector<BoardTrap> Parchis::anyTrap(const Box & b1, const Box & b2) const{
    Box final_box;
    if (b2.type == final_queue || b2.type == goal){
        //Si el casilla destino es meta o pasillo final, la cambiamos por la última casilla
        //antes de entrar al pasillo final.
        switch (b2.col){
            case blue:
                final_box = Box(final_blue_box, normal, none);
            break;
            case red:
                final_box = Box(final_red_box, normal, none);
            break;
            case green:
                final_box = Box(final_green_box, normal, none);
            break;
            case yellow:
                final_box = Box(final_yellow_box, normal, none);
            break;
        }
    }else{
        final_box = b2;
    }
    vector<BoardTrap> traps;
    bool reached_final_box = false;
    if (b1.type == normal && final_box.num != b1.num){
        for (int i = b1.num%68+1; !reached_final_box; i = i%68 + 1){ //Vamos recorriendo casillas intermedias
            reached_final_box = (final_box.num == i);
            //Si hay un muro, lo añadimos al vector de muros.
            for (int j = 0; j < this->board.getTraps().size(); j++){
                if (this->board.getTraps().at(j).getBox() == Box(i, normal, none)){
                    traps.push_back(this->board.getTraps().at(j));
                }
            }
        }
    }
    return traps;
}

const vector<pair <color, int>> Parchis::allPiecesBetween(const Box & b1, const Box & b2) const{
    Box final_box;
    if (b2.type == final_queue || b2.type == goal){
        //Si el casilla destino es meta o pasillo final, la cambiamos por la última casilla
        //antes de entrar al pasillo final.
        switch (b2.col){
            case blue:
                final_box = Box(final_blue_box, normal, none);
            break;
            case red:
                final_box = Box(final_red_box, normal, none);
            break;
            case green:
                final_box = Box(final_green_box, normal, none);
            break;
            case yellow:
                final_box = Box(final_yellow_box, normal, none);
            break;
        }
    }else{
        final_box = b2;
    }
    vector<pair<color, int>> pieces;
    bool reached_final_box = false;
    if (b1.type == normal && final_box.num != b1.num){
        for (int i = b1.num+1; !reached_final_box; i = i%68 + 1){ //Vamos recorriendo casillas intermedias
            reached_final_box = (final_box.num == i);
            //Si hay un muro, lo añadimos al vector de muros.
            vector<pair <color, int>> occupation = boxState(Box(i, normal, none));

            for (int j = 0; j < occupation.size(); j++){
                pieces.push_back({occupation.at(j).first, occupation.at(j).second});
            }
        }
    }
    return pieces;
}

const pair<color, int> Parchis::eatenPiece() const{
    if(eating_move){
        return eaten_piece;
    }else{
        return {none, 0};
    }
}

const vector<pair<color, int>> Parchis::piecesDestroyedByStar() const{
    return pieces_destroyed_by_star;
}

const vector<pair<color, int>> Parchis::piecesCrushedByMegamushroom() const{
    return pieces_crushed_by_megamushroom;
}

const vector<pair<color, int>> Parchis::piecesDestroyedByRedShell() const{
    return pieces_destroyed_by_red_shell;
}

const vector<pair<color, int>> Parchis::piecesDestroyedByBlueShell() const{
    return pieces_destroyed_by_blue_shell;
}

const vector<pair<color, int>> Parchis::piecesDestroyedByHorn() const
{
    return pieces_destroyed_by_horn;
}

const vector<pair<color, int>> Parchis::piecesDestroyedLastMove() const{
    // Append all vectors
    /*
    vector<pair<color, int>> all_pieces_destroyed;
    all_pieces_destroyed.insert(all_pieces_destroyed.end(), pieces_destroyed_by_star.begin(), pieces_destroyed_by_star.end());
    all_pieces_destroyed.insert(all_pieces_destroyed.end(), pieces_crushed_by_megamushroom.begin(), pieces_crushed_by_megamushroom.end());
    all_pieces_destroyed.insert(all_pieces_destroyed.end(), pieces_destroyed_by_red_shell.begin(), pieces_destroyed_by_red_shell.end());
    all_pieces_destroyed.insert(all_pieces_destroyed.end(), pieces_destroyed_by_blue_shell.begin(), pieces_destroyed_by_blue_shell.end());
    all_pieces_destroyed.insert(all_pieces_destroyed.end(), pieces_destroyed_by_horn.begin(), pieces_destroyed_by_horn.end());
    return all_pieces_destroyed;*/
    if(pieces_destroyed_by_star.size() > 0)
        return pieces_destroyed_by_star;
    else if(pieces_crushed_by_megamushroom.size() > 0)
        return pieces_crushed_by_megamushroom;
    else if(pieces_destroyed_by_red_shell.size() > 0)
        return pieces_destroyed_by_red_shell;
    else if(pieces_destroyed_by_blue_shell.size() > 0)
        return pieces_destroyed_by_blue_shell;
    else if(pieces_destroyed_by_horn.size() > 0)
        return pieces_destroyed_by_horn;
    else
        return {};
}

bool Parchis::itemAcquired() const{
    return this->last_acquired != not_an_item;
}

item_type Parchis::getItemAcquired() const{
    return this->last_acquired;
}

bool Parchis::isSpecialDice(int dice) const{
    return dice > 100;
}

bool Parchis::isNormalDice(int dice) const{
    return dice >= 1 and dice <= 6;
}


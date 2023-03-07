# include "Board.h"

Board::Board(){
    this->setFromConfig(ALL_AT_HOME);
}


Board::Board (const map <color, vector <Piece> > & b){
    this->pieces = map<color, vector<Piece> >(b);
}

Board::Board(const BoardConfig & config){
    this->setFromConfig(config);
}

bool Board::operator==(const Board & board) const{
    return this->pieces == board.pieces;
}

const Piece & Board::getPiece(const color c, const int idx) const{
    return pieces.at(c)[idx];
}

void Board::setPieceType(const color c, const int idx, const special_type type){
    pieces.at(c)[idx].set_type(type);
}

void Board::setPieceTurnsLeft(const color c, const int idx, const int turns_left){
    pieces.at(c)[idx].set_turns_left(turns_left);
}

void Board::decreasePieceTurnsLeft(const color c, const int idx){
    pieces.at(c)[idx].set_turns_left(pieces.at(c)[idx].get_turns_left() - 1);
    if(pieces.at(c)[idx].get_turns_left() < 0)
        pieces.at(c)[idx].set_turns_left(0);
}


const vector<Piece> & Board::getPieces(const color c) const{
    return pieces.at(c);
}

const vector<SpecialItem> & Board::getSpecialItems() const{
    return special_items;
}

void Board::deleteSpecialItem(const int pos){
    special_items.erase(special_items.begin() + pos);
}

void Board::movePiece(const color c, const int idx, const Box & final_box){
    pieces[c][idx].set_box(final_box);
}

void Board::setFromConfig(const BoardConfig & config){
    //Definimos las siguientes configuraciones especificando en qué casillas empiezan cada
    //una de las piezas.
    switch(config){
        case ALL_AT_HOME:
            this->pieces = map<color, vector<Piece>>{
                {color::green, {Piece(color::green, {0, box_type::home, color::green}), Piece(color::green,{0, box_type::home, color::green}), Piece(color::green,{0, box_type::home, color::green}), Piece(color::green,{0, box_type::home, color::green})}},
                {color::red, {Piece(color::red, {0, box_type::home, color::red}), Piece(color::red, {0, box_type::home, color::red}), Piece(color::red, {0, box_type::home, color::red}), Piece(color::red, {0, box_type::home, color::red})}},
                {color::blue, {Piece(color::blue, {0, box_type::home, color::blue}), Piece(color::blue, {0, box_type::home, color::blue}), Piece(color::blue, {0, box_type::home, color::blue}), Piece(color::blue, {0, box_type::home, color::blue})}},
                {color::yellow, {Piece(color::yellow, {0, box_type::home, color::yellow}), Piece(color::yellow, {0, box_type::home, color::yellow}), Piece(color::yellow, {0, box_type::home, color::yellow}), Piece(color::yellow, {0, box_type::home, color::yellow})}}};
            break;

        case GROUPED:
            this->pieces = map<color, vector<Piece>>{
                {color::green, {Piece(color::green, {0, box_type::home, color::green}), Piece(color::green, {55, box_type::normal, color::none}), Piece(color::green, {64, box_type::normal, color::none}), Piece(color::green, {68, box_type::normal, color::none})}},
                {color::red, {Piece(color::red, {0, box_type::home, color::red}), Piece(color::red, {38, box_type::normal, color::none}), Piece(color::red, {47, box_type::normal, color::none}), Piece(color::red, {51, box_type::normal, color::none})}},
                {color::blue, {Piece(color::blue, {0, box_type::home, color::blue}), Piece(color::blue, {21, box_type::normal, color::none}),  Piece(color::blue, {30, box_type::normal, color::none}),  Piece(color::blue, {34, box_type::normal, color::none})}},
                {color::yellow, {Piece(color::yellow, {0, box_type::home, color::yellow}), Piece(color::yellow, {4, box_type::normal, color::none}), Piece(color::yellow, {13, box_type::normal, color::none}), Piece(color::yellow, {17, box_type::normal, color::none})}}};
            this->special_items = vector<SpecialItem>{
                {boo, {22, box_type::normal, color::none}},
                {boo, {14, box_type::normal, color::none}}
            };
            break;

        /*
        case ALTERNED:
            this->pieces = map<color, vector<Box>>{
                {color::green, {{0, box_type::home, color::green}, {55, box_type::normal, color::none}, {13, box_type::normal, color::none}, {34, box_type::normal, color::none}}},
                {color::red, {{0, box_type::home, color::red}, {38, box_type::normal, color::none}, {64, box_type::normal, color::none}, {17, box_type::normal, color::none}}},
                {color::blue, {{0, box_type::home, color::blue}, {21, box_type::normal, color::none}, {47, box_type::normal, color::none}, {68, box_type::normal, color::none}}},
                {color::yellow, {{0, box_type::home, color::yellow}, {4, box_type::normal, color::none}, {30, box_type::normal, color::none}, {51, box_type::normal, color::none}}}};
            break;

        case ALMOST_GOAL:
            this->pieces = map<color, vector<Box>>{
                {color::green, {{51, box_type::normal, color::none}, {1, box_type::final_queue, color::green}, {2, box_type::final_queue, color::green}, {3, box_type::final_queue, color::green}}},
                {color::red, {{34, box_type::normal, color::none}, {1, box_type::final_queue, color::red}, {2, box_type::final_queue, color::red}, {3, box_type::final_queue, color::red}}},
                {color::blue, {{17, box_type::normal, color::none}, {1, box_type::final_queue, color::blue}, {2, box_type::final_queue, color::blue}, {3, box_type::final_queue, color::blue}}},
                {color::yellow, {{68, box_type::normal, color::none}, {1, box_type::final_queue, color::yellow}, {2, box_type::final_queue, color::yellow}, {3, box_type::final_queue, color::yellow}}}};
            break;
        case DEBUG:
            this->pieces = map<color, vector<Box>>{
                {color::green, {{56, box_type::normal, color::none}, {1, box_type::final_queue, color::green}, {2, box_type::final_queue, color::green}, {3, box_type::final_queue, color::green}}},
                {color::red, {{34, box_type::normal, color::none}, {1, box_type::final_queue, color::red}, {2, box_type::final_queue, color::red}, {3, box_type::final_queue, color::red}}},
                {color::blue, {{17, box_type::normal, color::none}, {1, box_type::final_queue, color::blue}, {2, box_type::final_queue, color::blue}, {3, box_type::final_queue, color::blue}}},
                {color::yellow, {{55, box_type::normal, color::none}, {1, box_type::final_queue, color::yellow}, {2, box_type::final_queue, color::yellow}, {3, box_type::final_queue, color::yellow}}}};
            break;*/
    }
}


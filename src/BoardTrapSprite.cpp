#include "BoardTrapSprite.h"

const map<trap_type, vector<int>> BoardTrapSprite::trap2textrec = {
    {banana_trap, {60, 0, 30, 30}},
};


BoardTrapSprite::BoardTrapSprite(const Texture & t, trap_type type) : Sprite(t){
    this->type = type;
    this->setTextureRect(IntRect(trap2textrec.at(type).at(0), trap2textrec.at(type).at(1), trap2textrec.at(type).at(2), trap2textrec.at(type).at(3)));
}



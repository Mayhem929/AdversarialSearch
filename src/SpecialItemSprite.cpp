# include "SpecialItemSprite.h"
# include "ParchisGUI.h"

const map<item_type, vector<int>> SpecialItemSprite::item2textrec = {
    {star, {0, 0, 30, 30}}
};


SpecialItemSprite::SpecialItemSprite(const Texture & t, item_type type) : Sprite(t){
    this->type = type;
    this->setTextureRect(IntRect(item2textrec.at(type).at(0), item2textrec.at(type).at(1), item2textrec.at(type).at(2), item2textrec.at(type).at(3)));
}



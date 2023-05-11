# include "SpecialItemSprite.h"
# include "ParchisGUI.h"

const map<item_type, vector<int>> SpecialItemSprite::item2textrec = {
    {star, {0, 0, 30, 30}},
    {boo, {150, 0, 30, 30}},
    {bullet, {210, 0, 30, 30}},
    {blue_shell, {30, 0, 30, 30}},
    {red_shell, {270, 0, 30, 30}},
    {mushroom, {180, 0, 30, 30}},
    {mega_mushroom, {420, 0, 30, 30}},
    {shock, {300, 0, 30, 30}},
    {horn, {450, 0, 30, 30}},
    {banana, {60, 0, 30, 30}},
};


SpecialItemSprite::SpecialItemSprite(const Texture & t, item_type type) : Sprite(t){
    this->type = type;
    this->setTextureRect(IntRect(item2textrec.at(type).at(0), item2textrec.at(type).at(1), item2textrec.at(type).at(2), item2textrec.at(type).at(3)));
}



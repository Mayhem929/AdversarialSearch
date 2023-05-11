#include "DiceSprite.h"
#include "ParchisGUI.h"

const map<int, vector<int>> DiceSprite::num2textrec = {
    {1, {6, 118, 116, 116}},//{1, {2, 120, 120, 120}},
    {2, {122, 118, 116, 116}},//{2, {121, 120, 120, 120}},
    {3, {238, 118, 116, 116}},//{3, {237, 120, 120, 120}},
    {4, {353, 118, 116, 116}},//{4, {352, 120, 120, 120}},
    {5, {469, 118, 116, 116}},//{5, {468, 120, 120, 120}},
    {6, {584, 118, 116, 116}},//{6, {583, 120, 120, 120}},
    {10, {700, 118, 116, 116}},
    {20, {815, 118, 116, 116}},
    {-1, {700, 0, 116, 116}},
    {star, {930,118,116, 116}},
    {boo, {1507, 118, 116, 116}},
    {bullet, {1738, 118, 116, 116}},
    {red_shell, {1970, 118, 116, 116}},
    {blue_shell, {1045, 118, 116, 116}},
    {mushroom, {1621, 118, 116, 116}},
    {mega_mushroom, {2546, 118, 116, 116}},
    {shock, {2085, 118, 116, 116}},
    {horn, {2663, 118, 116, 116}},
    {banana, {1160, 118, 116, 116}}};

const map<int, vector<int>> DiceSprite::num2selectedrec = {
    {1, {6, 236, 116, 116}},
    {2, {122, 236, 116, 116}},
    {3, {238, 236, 116, 116}},
    {4, {353, 236, 116, 116}},
    {5, {469, 236, 116, 116}},
    {6, {584, 236, 116, 116}},
    {10, {700, 236, 116, 116}},
    {20, {815, 236, 116, 116}},
    {-1, {700, 0, 116, 116}},
    {star, {930, 236, 116, 116}},
    {boo, {1507, 236, 116, 116}},
    {bullet, {1738, 236, 116, 116}},
    {red_shell, {1970, 236, 116, 116}},
    {blue_shell, {1045, 236, 116, 116}},
    {mushroom, {1621, 236, 116, 116}},
    {mega_mushroom, {2546, 236, 116, 116}},
    {shock, {2085, 236, 116, 116}},
    {horn, {2663, 236, 116, 116}},
    {banana, {1160, 236, 116, 116}}};

const map<color, Color> DiceSprite::color2Color = {
    {red, Color(204, 102, 102)},
    {blue, Color(102, 102, 204)},
    {green, Color(102, 204, 102)},
    {yellow, Color(255, 255, 127)},
    {none, Color::White}
};

const map<color, Color> DiceSprite::color2DisabledColor = {
    {red, Color(204 / 2, 102 / 2, 102 / 2)},
    {blue, Color(102 / 2, 102 / 2, 204 / 2)},
    {green, Color(102 / 2, 204 / 2, 102 / 2)},
    {yellow, Color(255 / 2, 255 / 2, 127 / 2)},
    {none, Color::White}
};

const map<color, Color> DiceSprite::color2SelectedColor = {
    {red, Color(255, 0, 0)},
    {blue, Color(0, 255, 255)},
    {green, Color(192, 255, 0)},
    {yellow, Color(255, 192, 0)},
    {none, Color::White}};

DiceSprite::DiceSprite(const Texture & t, int n, color c) : ClickableSprite(t){
    this->number = n;
    this-> c = c;
    this->setTextureRect(IntRect(num2textrec.at(n).at(0), num2textrec.at(n).at(1), num2textrec.at(n).at(2), num2textrec.at(n).at(3)));
    this->setColor(color2Color.at(c));
    this->setScale(Vector2f(0.5,0.5));
}

void DiceSprite::setNumber(int n){
    this->number = n;
    if(selected){
        this->setTextureRect(IntRect(num2selectedrec.at(n).at(0), num2selectedrec.at(n).at(1), num2selectedrec.at(n).at(2), num2selectedrec.at(n).at(3)));
    }else{
        this->setTextureRect(IntRect(num2textrec.at(n).at(0), num2textrec.at(n).at(1), num2textrec.at(n).at(2), num2textrec.at(n).at(3)));
    }
}

void DiceSprite::setModelColor(color c){
    this->c = c;
    if(selected){
        this->setColor(color2SelectedColor.at(c));
    }else if(enabled){
        this->setColor(color2Color.at(c));
    }else{
        this->setColor(color2DisabledColor.at(c));
    }
}

void DiceSprite::onClickAction(Window & container) {
    ParchisGUI * gui = static_cast<ParchisGUI*>(&container);

    if(!locked && enabled && clicked && number != -1){
        gui->last_dice = this->number;
        gui->updateSprites();
    }
    else if(clicked && (locked || !enabled)){
        gui->playForbiddenSound();
    }
}

void DiceSprite::onEnableAction(Window & container) {
    if(this->enabled){
        this->setColor(color2Color.at(c));
    }
    else{
        this->setColor(color2DisabledColor.at(c));
    }
}

void DiceSprite::onSelectionAction(Window & container){
    if(this->selected){
        this->setColor(color2SelectedColor.at(c));
    }
    else if(!this->enabled){
        this->setColor(color2DisabledColor.at(c));
    }
    else{
        this->setColor(color2Color.at(c));
    }
}

void DiceSprite::onLockAction(Window & container) {}

void DiceSprite::onHoverAction(Window &container) {
    if (hovered && number != -1)
    {
        //cout << enabled << endl;
        //cout << ClickableSprite::enabled << endl;
        ParchisGUI *gui = dynamic_cast<ParchisGUI *>(&container);
        if(locked || !enabled){
            gui->setForbiddenCursor();
        }
        else{
            this->setTextureRect(IntRect(num2selectedrec.at(number).at(0), num2selectedrec.at(number).at(1), num2selectedrec.at(number).at(2), num2selectedrec.at(number).at(3)));
            gui->setHandCursor();
        }
    }
    else
    {
        this->setTextureRect(IntRect(num2textrec.at(number).at(0), num2textrec.at(number).at(1), num2textrec.at(number).at(2), num2textrec.at(number).at(3)));
    }
}

void DiceSprite::setShaderColors(Shader & s){
    color colorA = c;
    color colorB = partner_color(c);

    Color actual_colorA, actual_colorB;

    if(this->isSelected()){
        actual_colorA = color2SelectedColor.at(colorA);
        actual_colorB = color2SelectedColor.at(colorB);
    }
    else if(!this->isEnabled()){
        actual_colorA = color2DisabledColor.at(colorA);
        actual_colorB = color2DisabledColor.at(colorB);
    }
    else{
        actual_colorA = color2Color.at(colorA);
        actual_colorB = color2Color.at(colorB);
    }

    s.setUniform("colorA", sf::Glsl::Vec3(actual_colorA.r / 255.0, actual_colorA.g / 255.0, actual_colorA.b / 255.0));
    s.setUniform("colorB", sf::Glsl::Vec3(actual_colorB.r / 255.0, actual_colorB.g / 255.0, actual_colorB.b / 255.0));
}

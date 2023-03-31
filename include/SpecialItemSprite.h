#ifndef __SPECIALITEMSPRITE_H__
#define __SPECIALITEMSPRITE_H__

#include "ClickableSprite.h"
#include "IncludesSFML.h"
#include "Attributes.h"
// #include "ParchisGUI.h"

using namespace std;

class SpecialItemSprite: public Sprite{
    private:
        item_type type;
        static const map <item_type, vector<int>> item2textrec;

    public:

        SpecialItemSprite(const Texture & t, item_type type);

        inline item_type getType(){ return this->type;}
};

#endif
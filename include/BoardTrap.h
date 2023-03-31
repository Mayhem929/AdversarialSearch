#ifndef __BOARDTRAP_H_
#define __BOARDTRAP_H_

#include "Attributes.h"

using namespace std;

enum trap_type{banana_trap};

class BoardTrap{
    private:
        trap_type type;
        Box box;
    public:
        inline BoardTrap(trap_type type, Box box){ this->type = type; this->box = box;}
        inline const trap_type & getType() const{ return this->type;}
        inline const Box & getBox() const{ return this->box;}
};


#endif

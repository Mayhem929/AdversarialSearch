#ifndef __EXPLOSIONSPRITE_H__
#define __EXPLOSIONSPRITE_H__

#include "IncludesSFML.h"
#include "SpriteAnimator.h"

using namespace std;

class ExplosionSprite: public Sprite{
    private:
        Color c;
    public:
        inline ExplosionSprite():Sprite(){}
        ExplosionSprite(const Texture & t, Color c);

        inline Color getColor(){return c;}
};

class ExplosionAnimator: public SpriteAnimator{
    private:
        float expansionScale;
        float baseScale;

    public:
        ExplosionAnimator(float baseScale = 1.f, float expansionScale = 3.f, int animationTime = 1000);
        ExplosionAnimator(Transformable &s, float baseScale = 1.f, float expansionScale = 3.f, int animationTime = 1000);
        
        virtual void restart();
        virtual void update();
        virtual bool hasEnded();
        
};

#endif
# include "ExplosionSprite.h"
# include <cmath>

ExplosionSprite::ExplosionSprite(const Texture & t, Color c) : Sprite(t), c(c){
    this->setColor(Color(0, 0, 0, 0));
}

ExplosionAnimator::ExplosionAnimator(float baseScale, float expansionScale, int animationTime) : SpriteAnimator(animationTime), baseScale(baseScale), expansionScale(expansionScale){
}

ExplosionAnimator::ExplosionAnimator(Transformable &s, float baseScale, float expansionScale, int animationTime) : SpriteAnimator(s, s.getPosition(), s.getPosition(), animationTime), baseScale(baseScale), expansionScale(expansionScale){
}

void ExplosionAnimator::restart(){
    this->animation_clock.restart();
    if(sprite != NULL) this->sprite->setScale(1.f, 1.f);
}

void ExplosionAnimator::update(){
    int t = this->animation_clock.getElapsedTime().asMilliseconds();
    if(t > animation_time) t = animation_time;
    float fa_time = animation_time; 
    
    if(sprite == NULL) return;
    // Scale grows linearly from 1 to expansionScale
    float scale = (1.f - t / fa_time) + (t / fa_time) * expansionScale;

    // Transparency decreases linearly from 255 to 0
    //int alpha = (1.f - t / fa_time) * 255;
    float progress = 1.0f - t / fa_time;
    int alpha = 255.0f * (1.0f - powf(1.0f - progress, 2.0f));


    // Set the scale and transparency
    sprite->setScale(scale, scale);
    // Static cast to ExplosionSprite. This is a hack, but it works. Better solution? Yes, but I don't have time to implement it. I'm sorry. I will fix this later.
    ExplosionSprite * es = static_cast<ExplosionSprite*>(sprite);
    es->setColor(Color(es->getColor().r, es->getColor().g, es->getColor().b, alpha));
}

bool ExplosionAnimator::hasEnded(){
    return this->animation_clock.getElapsedTime().asMilliseconds() >= animation_time;
}
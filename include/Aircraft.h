#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "Entity.h"

class Aircraft : public Entity
{
    public:
        enum Type
        {
            Umag,
            Disk,
        };
    public:
        explicit    Aircraft(Type type);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        Type    mType;
        sf::Sprite  mSprite;

};

#endif // AIRCRAFT_H

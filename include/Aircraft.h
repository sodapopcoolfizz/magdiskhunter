#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "Entity.h"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Category.hpp"

class Aircraft : public Entity
{
    public:
        enum Type
        {
            Umag,
            Disk,
            Orb
        };
    public:
        explicit    Aircraft(Type type, const TextureHolder& textures);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual unsigned int getCategory() const;
        void accelerate(sf::Vector2f velocity);
    private:
        Type    mType;
        sf::Sprite  mSprite;

};

#endif // AIRCRAFT_H

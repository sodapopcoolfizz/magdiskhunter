#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Entity.h>
#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>
#include "SFML/System/Time.hpp"


class Projectile : public Entity
{
    public:
        enum Type
        {
            AlliedBullet,
            EnemyBullet,
            Missile,
            TypeCount
        };

    public:
        Projectile(Type type, const TextureHolder& textures);

        void guideTowards(sf::Vector2f position);
        bool isGuided() const;

        virtual unsigned int getCategory() const;
        virtual sf::FloatRect getBoundingRectangle() const;
        float getMaxSpeed() const;
        int getDamage() const;


    private:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        Type mType;
        sf::Sprite mSprite;
        sf::Vector2f mTargetDirection;
};

#endif // PROJECTILE_H

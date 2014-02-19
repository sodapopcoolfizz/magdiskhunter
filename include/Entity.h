#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/System.hpp>
#include "SceneNode.h"
#include "CommandQueue.h"

class Entity : public SceneNode
{
    public:
        explicit Entity(int hitpoints);
        virtual void repair(int points);
        virtual void damage(int points);
        virtual void destroy();

        virtual int getHitpoints() const;
        virtual bool isDestroyed() const;

        void setVelocity(sf::Vector2f velocity);
        void setVelocity(float vx, float vy);
        sf::Vector2f getVelocity() const;


    private:
    sf::Vector2f mVelocity;
    int mHitpoints;

    protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
};

#endif // ENTITY_H

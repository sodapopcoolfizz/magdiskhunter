#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/System.hpp>
#include "SceneNode.h"
#include "CommandQueue.h"

class Entity : public SceneNode
{
    public:
        explicit Entity(int hitpoints);
        void repair(int points);
        void damage(int points);
        void destroy();

        int getHitpoints() const;
        bool isDestroyed();

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

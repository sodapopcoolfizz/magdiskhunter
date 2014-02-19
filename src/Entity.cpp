#include "Entity.h"

Entity::Entity(int hitpoints): mVelocity(), mHitpoints(hitpoints)
{}

void Entity::repair(int points)
{
    if(points>0)
    {
        mHitpoints+=points;
    }
}

void Entity::damage(int points)
{
    if(points>0)
    {
        mHitpoints-=points;
    }
}

void Entity::destroy()
{
    mHitpoints=0;
}

bool Entity::isDestroyed() const
{
    return mHitpoints<=0;
}

int Entity::getHitpoints() const
{
    return mHitpoints;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    move(mVelocity * dt.asSeconds());
}

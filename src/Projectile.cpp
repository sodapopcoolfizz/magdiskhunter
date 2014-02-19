#include "Projectile.h"
#include "DataTables.hpp"
#include "utils.hpp"
#include "Category.hpp"
#include <cmath>
#include <iostream>

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures) : Entity(1), mType(type), mSprite(textures.get(Table[type].texture))
{
    centerOrigin(mSprite);
}

void Projectile::guideTowards(sf::Vector2f position)
{
    assert(isGuided());
    mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
    return mType == Missile;
}


unsigned int Projectile::getCategory() const
{
    if (mType == EnemyBullet)
    {
        return Category::EnemyProjectile;
    }
    else
    {
        return Category::AlliedProjectile;
    }
}

sf::FloatRect Projectile::getBoundingRectangle() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return Table[mType].speed;
}

int Projectile::getDamage() const
{
    return Table[mType].damage;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if(isGuided())
    {
        const float approachRate = 2000.f;
        sf::Vector2f newVelocity = unitVector(getVelocity() + approachRate*dt.asSeconds()*mTargetDirection);

        newVelocity*=getMaxSpeed();
        float angle = std::atan2(newVelocity.y,newVelocity.x);
        setRotation(toDegree(angle)+90.f);
        setVelocity(newVelocity);
    }

    Entity::updateCurrent(dt,commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite,states);
}

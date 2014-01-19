#include "Aircraft.h"
#include "DataTables.hpp"
#include "utils.hpp"

namespace
{
	const std::vector<AircraftData> Table = initializeAircraftData();
}


Textures::ID toTextureID(Aircraft::Type type)
{
    switch (type)
    {
        case Aircraft::Umag:
            return Textures::Umag;
        case Aircraft::Disk:
            return Textures::Disk;
        case Aircraft::Orb:
            return Textures::Orb;
        default:
            return Textures::Umag;
    }

}

Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts): Entity::Entity(Table[type].hitpoints), mType(type), mSprite(textures.get(toTextureID(type))), mDirectionIndex(0)
{
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mSpreadLevel = 1;
    mFireRateLevel = 2;
    mMissileAmmo = 2;

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts,""));
    mHealthDisplay = healthDisplay.get();
    attachChild(std::move(healthDisplay));

    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action = [this, & textures] (SceneNode& node, sf::Time)
    {
        createBullets(node, textures);
    };

    mMissileCommand.category = Category::SceneAirLayer;
    mMissileCommand.action = [this, & textures](SceneNode& node, sf::Time)
    {
        createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
    };
    mIsFiring = false;
    mIsLaunchingMissile = false;
    mFireCountdown = sf::Time::Zero;
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

unsigned int Aircraft::getCategory() const
{
    if(isAllied())
    {
        return Category::PlayerAircraft;
    }
    else
        return Category::EnemyAircraft;

}

void Aircraft::accelerate(sf::Vector2f velocity)
{
    this->setVelocity(getVelocity()+velocity);
}

void Aircraft::updateTexts()
{
    mHealthDisplay->setString(toString(getHitpoints()) +" HP");
    //mHealthDisplay->setString(toString(mType));
    mHealthDisplay->setPosition(0.f, 50.f);
    mHealthDisplay->setRotation(-getRotation());
}

void Aircraft::updateMovementPatterns(sf::Time dt)
{
    const std::vector<Direction>& directions = Table[mType].directions;
    if(!directions.empty())
    {
        float distanceToTravel = directions[mDirectionIndex].distance;
        if(mTravelledDistance>distanceToTravel)
        {
            mDirectionIndex = (mDirectionIndex + 1 ) % directions.size();
            mTravelledDistance = 0.f;
        }

        float radians = toRadian(directions[mDirectionIndex].angle +90.f);
        float vx = getMaxSpeed() * std::cos(radians);
        float vy = getMaxSpeed() * std::sin(radians);
        setVelocity(vx,vy);
        mTravelledDistance += getMaxSpeed() * dt.asSeconds();
    }
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    updateMovementPatterns(dt);
    Entity::updateCurrent(dt,commands);

    checkProjectileLaunch(dt,commands);

    updateTexts();
}

float Aircraft::getMaxSpeed() const
{
    return Table[mType].speed;
}
void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{

    if(!isAllied())
    {
        //change this !!! Add bullet patterns, frequency ...
        fire();
    }

    if (mIsFiring && mFireCountdown <= sf::Time::Zero)
    {
        commands.push(mFireCommand);
        mFireCountdown += sf::seconds(1.f / (mFireRateLevel+1));
        mIsFiring = false;
    }
    else if (mFireCountdown > sf::Time::Zero)
    {
        mFireCountdown -=dt;
    }

    if(mIsLaunchingMissile)
    {
        commands.push(mMissileCommand);
        mIsLaunchingMissile = false;
    }
}


void Aircraft::fire()
{
    mIsFiring = true;
}

void Aircraft::LaunchMissile()
{
    if(mMissileAmmo > 0)
    {
        mIsLaunchingMissile = true;
        mMissileAmmo--;
    }

}

void Aircraft::createBullets(SceneNode& node, const TextureHolder& textures) const
{
    Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

    switch (mSpreadLevel)
    {
        case 1:
            createProjectile(node, type, 0.0f, 0.75f, textures);
            break;

        case 2:
            createProjectile(node,type,-0.33f,0.5f,textures);
            createProjectile(node,type,0.33f,0.5f,textures);
            break;
        case 3:
            createProjectile(node,type,-0.33f,0.33f,textures);
            createProjectile(node,type,0.0f,0.5f,textures);
            createProjectile(node,type,0.33f,0.33f,textures);
            break;
    }
}

void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

    sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
    sf::Vector2f velocity(0,projectile->getMaxSpeed());

    float sign = isAllied() ? -1.f : 1.f;
    projectile->setPosition(getWorldPosition() + offset *sign);
    projectile->setVelocity(velocity*sign+getVelocity()*0.2f);
    node.attachChild(std::move(projectile));
}

bool Aircraft::isAllied() const
{
    return  mType  == Umag;
}

void Aircraft::collectMissiles(unsigned int count)
{
    mMissileAmmo += count;
}

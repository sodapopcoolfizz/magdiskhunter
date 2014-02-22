#include "Aircraft.h"
#include "DataTables.hpp"
#include "utils.hpp"
#include "Pickup.h"
#include "SoundNode.h"

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
    mFireRateLevel = 5;
    mMissileAmmo = 10;

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

    mDropPickupCommand.category = Category::SceneAirLayer;
    mDropPickupCommand.action = [this, & textures] (SceneNode& node, sf::Time)
    {
        createPickup(node, textures);
    };

    mIsFiring = false;
    mIsLaunchingMissile = false;
    mFireCountdown = sf::Time::Zero;
    mIsMarkedForRemoval = false;
    mOutside = false;
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
    if (isDestroyed())
    {
        checkPickupDrop(commands);
        mIsMarkedForRemoval = true;
        if(!getOutside())
        {
            playLocalSound(commands,SoundEffect::Explosion);
        }
        return;
    }

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
        playLocalSound(commands,SoundEffect::AlliedGunfire);
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

void Aircraft::checkPickupDrop(CommandQueue& commands)
{
    int dice = rand()  % 3;
    if (dice == 0)
    {
        commands.push(mDropPickupCommand);
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

        default:
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

void Aircraft::createPickup(SceneNode& node, const TextureHolder& textures) const
{
    auto typec = Pickup::TypeCount ;
    auto type = static_cast<Pickup::Type> (rand() % typec);
    std::unique_ptr<Pickup> drop(new Pickup(type,textures));
    drop->setPosition(getWorldPosition());
    node.attachChild(std::move(drop));
}

bool Aircraft::isAllied() const
{
    return  mType  == Umag;
}

void Aircraft::collectMissiles(unsigned int count)
{
    mMissileAmmo += count;
}

sf::FloatRect Aircraft::getBoundingRectangle() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}


bool Aircraft::isMarkedForRemoval() const
{
    if(getCategory() == Category::PlayerAircraft)
    {
        return false;
    }
    return mIsMarkedForRemoval;
}

void Aircraft::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
    sf::Vector2f position = getWorldPosition();

    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode>
    (
        [effect,position] (SoundNode& node,sf::Time)
        {
            node.play(effect,position);
        }
    );
    commands.push(command);
}

void Aircraft::setOutside()
{
    mOutside = true;
}

bool Aircraft::getOutside()
{
    return mOutside;
}

void Aircraft::increaseFireSpread()
{
    mSpreadLevel++;
}

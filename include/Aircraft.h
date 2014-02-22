#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "Entity.h"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Category.hpp"
#include "TextNode.h"
#include "CommandQueue.h"
#include "Projectile.h"

class Aircraft : public Entity
{
    public:
        enum Type
        {
            Umag,
            Disk,
            Orb,
            TypeCount,
        };
    public:
        explicit    Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual unsigned int getCategory() const;
        void accelerate(sf::Vector2f velocity);
        float getMaxSpeed() const;



        void updateTexts();
        void updateMovementPatterns(sf::Time dt);
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
        void checkPickupDrop(CommandQueue& commands);
        void fire();
        void LaunchMissile();
        bool isAllied() const;
        void collectMissiles(unsigned int count);

        virtual sf::FloatRect getBoundingRectangle() const;
        virtual bool isMarkedForRemoval() const;

        void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);



    private:
        void createBullets(SceneNode& node, const TextureHolder& textures) const;
        void createProjectile(SceneNode& node, Projectile::Type, float xOffset, float yOffset, const TextureHolder& textures) const;
        void createPickup(SceneNode& node, const TextureHolder& textures) const;



        Type    mType;
        sf::Sprite  mSprite;
        TextNode* mHealthDisplay;

        float mTravelledDistance;
		std::size_t mDirectionIndex;
		bool mIsLaunchingMissile;
		bool mIsFiring;
		sf::Time mFireCountdown;
		Command mFireCommand;
		Command mMissileCommand;
		Command mDropPickupCommand;
		int mSpreadLevel;
		int mFireRateLevel;
		int mMissileAmmo;

		bool mIsMarkedForRemoval;

};

#endif // AIRCRAFT_H

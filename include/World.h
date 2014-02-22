#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <cmath>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SceneNode.h"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Aircraft.h"
#include "Pickup.h"
#include "SpriteNode.h"
#include "CommandQueue.h"
#include "utils.hpp"
#include "SoundPlayer.h"



class World : private sf::NonCopyable
{
    public:
        explicit    World(sf::RenderWindow& window, SoundPlayer & player);
        void        update(sf::Time dt);
        void        draw();
        CommandQueue& getCommandQueue();


    private:
        void        loadTextures();
        void        loadFonts();
        void        buildScene();
        sf::FloatRect getBattlefieldBounds() const;
        sf::FloatRect getViewBounds() const;

        void spawnEnemies();
        void addEnemies();
		void addEnemy(Aircraft::Type type, float relX, float relY);

		void guideMissiles();

		void handleCollisions();

		void destroyEntitiesOutsideView();

		void updateSounds();

    private:
        enum Layer
        {
            Background,
            Air,
            LayerCount
        };

    struct SpawnPoint
    {
        SpawnPoint(Aircraft::Type type, float x, float y): mType(type), mx(x), my(y)
        {}

        Aircraft::Type mType;
        float mx;
        float my;
    };

    private:
        sf::RenderWindow&       mWindow;
        sf::View                mWorldView;
        TextureHolder           mTextures;
        FontHolder              mFonts;
        SceneNode               mSceneGraph;
        std::array<SceneNode*, LayerCount> mSceneLayers;

        sf::FloatRect           mWorldBounds;
        sf::Vector2f            mSpawnPosition;
        float                   mScrollSpeed;
        Aircraft*               mPlayerAircraft;
        CommandQueue            mQueue;

        std::vector<SpawnPoint> mEnemySpawnPoints;
        std::vector<Aircraft*> mActiveEnemies;

        SoundPlayer &           mSoundPlayer;


};

bool matchesCategories(SceneNode::Pair & colliders, Category::Type type1, Category::Type type2);


#endif // WORLD_H

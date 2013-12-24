#ifndef WORLD_H
#define WORLD_H

#include <array>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SceneNode.h"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Aircraft.h"
#include "SpriteNode.h"



class World : private sf::NonCopyable
{
    public:
        explicit    World(sf::RenderWindow& window);
        void        update(sf::Time dt);
        void        draw();

    private:
        void        loadTextures();
        void        buildScene();

    private:
        enum Layer
        {
            Background,
            Air,
            LayerCount
        };

    private:
        sf::RenderWindow&       mWindow;
        sf::View                mWorldView;
        TextureHolder           mTextures;
        SceneNode               mSceneGraph;
        std::array<SceneNode*, LayerCount> mSceneLayers;

        sf::FloatRect           mWorldBounds;
        sf::Vector2f            mSpawnPosition;
        float                   mScrollSpeed;
        Aircraft*               mPlayerAircraft;


};


#endif // WORLD_H

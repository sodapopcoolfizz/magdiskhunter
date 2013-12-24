#include "World.h"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mWorldBounds(0.f,0.f,mWorldView.getSize().x,2000.f)
, mSpawnPosition(mWorldView.getSize().x/2.f,mWorldBounds.height - mWorldView.getSize().y/2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);

}

void World::loadTextures()
{
    mTextures.load(Textures::Umag, "../../Media/Textures/UMagnet.png");
    mTextures.load(Textures::Desert, "../../Media/Textures/Desert.png");
}

void World::buildScene()
{
    // Create layer nodes
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        // nouveau noeud
        SceneNode::Ptr layer(new SceneNode());
        // on copie un pointeur vers le noeud dans une couche
        mSceneLayers[i] = layer.get();
        // on transfert la propriété au graph de scène
        mSceneGraph.attachChild(std::move(layer));
    }
    // Create background
    sf::Texture& texture = mTextures.get(Textures::Desert);
    sf::IntRect textureRect(mWorldBounds);
    texture.setRepeated(true);

    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture,textureRect));
    backgroundSprite->setPosition(mWorldBounds.left,mWorldBounds.top);
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    // Create Aircraft
    std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Umag, mTextures));
    mPlayerAircraft = leader.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
    mSceneLayers[Air]->attachChild(std::move(leader));

    std::unique_ptr<Aircraft> escortLeft(new Aircraft(Aircraft::Umag, mTextures));
    escortLeft->setPosition(-80.f,50.f);
    mPlayerAircraft->attachChild(std::move(escortLeft));

    std::unique_ptr<Aircraft> escortRight(new Aircraft(Aircraft::Umag, mTextures));
    escortRight->setPosition(80.f,50.f);
    mPlayerAircraft->attachChild(std::move(escortRight));
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::update(sf::Time dt)
{
    mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());

    sf::Vector2f position = mPlayerAircraft->getPosition();
    sf::Vector2f velocity = mPlayerAircraft->getVelocity();

    if (position.x <= mWorldBounds.left + 150 || position.x >= mWorldBounds.left + mWorldBounds.width -150)
    {
        velocity.x = -velocity.x;
        mPlayerAircraft->setVelocity(velocity);
    }
    // applique les vélocités sur le reste des noeuds
    mSceneGraph.update(dt);
}



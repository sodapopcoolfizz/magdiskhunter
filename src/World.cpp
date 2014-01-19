#include "World.h"
#include <limits.h>


World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mWorldBounds(0.f,0.f,mWorldView.getSize().x,5000.f)
, mSpawnPosition(mWorldView.getSize().x/2.f,mWorldBounds.height - mWorldView.getSize().y/2.f)
, mScrollSpeed(-100.f)
, mPlayerAircraft(nullptr)
,mSceneGraph()
,mSceneLayers()
{
    loadTextures();
    loadFonts();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);

}

void World::loadTextures()
{
    mTextures.load(Textures::Umag, "../../Media/Textures/UMagnet.png");
    mTextures.load(Textures::Desert, "../../Media/Textures/Desert.png");
    mTextures.load(Textures::Orb, "../../Media/Textures/Orb.png");
    mTextures.load(Textures::Disk, "../../Media/Textures/Orb.png");
    mTextures.load(Textures::Bullet,"../../Media/Textures/Bullet.png");
    mTextures.load(Textures::Missile,"../../Media/Textures/Missile.png");
}

void World::loadFonts()
{
    mFonts.load(Fonts::Main, "../../Media/alphbeta.ttf");
}

void World::buildScene()
{
    // Create layer nodes
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        Category::Type category = (i==Air) ? Category::SceneAirLayer : Category::None;
        // nouveau noeud
        SceneNode::Ptr layer(new SceneNode(category));
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
    std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Umag, mTextures, mFonts));
    mPlayerAircraft = leader.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mSceneLayers[Air]->attachChild(std::move(leader));

    // Create Enemies
    addEnemies();
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::update(sf::Time dt)
{
    mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
    mPlayerAircraft->setVelocity(0.f,0.f);

    // envoyer les commandes au graph de scene
    while(!mQueue.isEmpty())
    {
        mSceneGraph.onCommand(mQueue.pop(), dt);
    }

    sf::Vector2f velocity = mPlayerAircraft->getVelocity();

    // adapt velocity if moving diagonnaly
    if(velocity.x != 0.f && velocity.y !=0.f)
        mPlayerAircraft->setVelocity(velocity/std::sqrt(2.f));

    mPlayerAircraft->accelerate(sf::Vector2f(0.f,mScrollSpeed));
    // applique les vélocités sur le reste des noeuds
    mSceneGraph.update(dt,mQueue);

    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());

    const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);

	spawnEnemies();
}

CommandQueue& World::getCommandQueue()
{
    return mQueue;
}


sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::spawnEnemies()
{
    while(!mEnemySpawnPoints.empty() && mEnemySpawnPoints.back().my > getBattlefieldBounds().top)
    {
        SpawnPoint spawn = mEnemySpawnPoints.back();
        std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.mType,mTextures,mFonts));
        enemy->setPosition(spawn.mx,spawn.my);
        enemy->setRotation(180.f);

        mSceneLayers[Air]->attachChild(std::move(enemy));
        mEnemySpawnPoints.pop_back();
    }
}

void World::addEnemies()
{
    addEnemy(Aircraft::Disk, 0.f, 500.f);
    addEnemy(Aircraft::Disk, -75.f, 100.f);
    addEnemy(Aircraft::Disk, 75.f, 200.f);
    addEnemy(Aircraft::Disk, -75.f, 500.f);
    addEnemy(Aircraft::Disk, 75.f, 600.f);
    addEnemy(Aircraft::Disk, -75.f, 800.f);
    addEnemy(Aircraft::Disk, 75.f, 900.f);
    addEnemy(Aircraft::Disk, -75.f, 1000.f);
    addEnemy(Aircraft::Disk, 75.f, 1200.f);
    // add other enemies; maybe try to implement enemies wave patterns ... via script or DataTable

    std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(),
    [] (SpawnPoint p1, SpawnPoint p2)
    {
        return p1.my < p2.my;
    });
}

void World::addEnemy(Aircraft::Type type, float relX, float relY)
{
    SpawnPoint spawn(type,mSpawnPosition.x + relX, mSpawnPosition.y - relY);
    mEnemySpawnPoints.push_back(spawn);
}

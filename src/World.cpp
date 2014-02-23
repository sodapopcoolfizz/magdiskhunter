#include "World.h"
#include <limits.h>
#include "utils.hpp"
#include <set>
#include "ShaderNode.h"
#include "SoundNode.h"

World::World(sf::RenderWindow& window, SoundPlayer& player)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mWorldBounds(0.f,0.f,mWorldView.getSize().x,5000.f)
, mSpawnPosition(mWorldView.getSize().x/2.f,mWorldBounds.height - mWorldView.getSize().y/2.f)
, mScrollSpeed(-100.f)
, mPlayerAircraft(nullptr)
,mSceneGraph()
,mSceneLayers()
,mSoundPlayer(player)
{
    srand(time(0));
    loadTextures();
    loadFonts();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);

}

void World::loadTextures()
{
    mTextures.load(Textures::Umag, "../../Media/Textures/magnet.png");
    mTextures.load(Textures::Desert, "../../Media/Textures/Desert.png");
    mTextures.load(Textures::Orb, "../../Media/Textures/Orb.png");
    mTextures.load(Textures::Disk, "../../Media/Textures/disk.png");
    mTextures.load(Textures::Bullet,"../../Media/Textures/bullet.png");
    mTextures.load(Textures::Missile,"../../Media/Textures/Missile.png");
    mTextures.load(Textures::MissileRefill,"../../Media/Textures/MissileRefill.png");
    mTextures.load(Textures::HealthRefill,"../../Media/Textures/HealthRefill.png");
    mTextures.load(Textures::FireSpread,"../../Media/Textures/FireSpread.png");
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
/*     sf::Texture& texture = mTextures.get(Textures::Desert);
 *     sf::IntRect textureRect(mWorldBounds);
 *     texture.setRepeated(true);
 *
 *     std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture,textureRect));
 *     backgroundSprite->setPosition(mWorldBounds.left,mWorldBounds.top);
 *     mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
 */
    sf::IntRect shaderRect(mWorldBounds);
    std::unique_ptr<ShaderNode> backgroundShader(new ShaderNode("../../Media/shader.frag",shaderRect));
    backgroundShader->setPosition(mWorldBounds.left, mWorldBounds.top);
    mSceneLayers[Background]->attachChild(std::move(backgroundShader));

    // Create Aircraft
    std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Umag, mTextures, mFonts));
    mPlayerAircraft = leader.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mSceneLayers[Air]->attachChild(std::move(leader));

    // Create Enemies
    addEnemies();

    std::unique_ptr<SoundNode> soundSource(new SoundNode(mSoundPlayer));
    mSceneGraph.attachChild(std::move(soundSource));
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

    destroyEntitiesOutsideView();
    guideMissiles();

	handleCollisions();
	mSceneGraph.removeWrecks();
	spawnEnemies();





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

	updateSounds();


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

    addEnemy(Aircraft::Disk, 0.f, 1500.f);
    addEnemy(Aircraft::Disk, -75.f, 1000.f);
    addEnemy(Aircraft::Disk, 75.f, 2000.f);
    addEnemy(Aircraft::Disk, -75.f, 2500.f);
    addEnemy(Aircraft::Disk, 75.f, 2600.f);
    addEnemy(Aircraft::Disk, -75.f, 2800.f);
    addEnemy(Aircraft::Disk, 75.f, 2900.f);
    addEnemy(Aircraft::Disk, -75.f, 11000.f);
    addEnemy(Aircraft::Disk, 75.f, 11200.f);
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

void World::guideMissiles()
{
    Command enemyCollector;
    enemyCollector.category = Category::EnemyAircraft;
    enemyCollector.action = derivedAction<Aircraft>(
    [this] (Aircraft& enemy, sf::Time)
    {
        if(! enemy.isDestroyed())
        {
            mActiveEnemies.push_back(&enemy);
        }
    });

    Command missileGuider;
    missileGuider.category = Category::AlliedProjectile;
    missileGuider.action = derivedAction<Projectile>(
    [this] (Projectile& missile, sf::Time)
    {
        if(!missile.isGuided())
        {
            return;
        }

        float minDistance = std::numeric_limits<float>::max();
        Aircraft* closestEnemy = nullptr;

        for(auto enemy = mActiveEnemies.begin();enemy!=mActiveEnemies.end();++enemy)
        {
            float enemyDistance = distance(missile,*(*enemy));

            if(enemyDistance<minDistance)
            {
                closestEnemy = *enemy;
                minDistance  = enemyDistance;
            }
        }
        if(closestEnemy)
        {
            missile.guideTowards(closestEnemy->getWorldPosition());
        }
    });
    mQueue.push(enemyCollector);
    mQueue.push(missileGuider);
    mActiveEnemies.clear();
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

    for(auto itPair = collisionPairs.begin(); itPair!=collisionPairs.end(); ++itPair)
    {
        SceneNode::Pair pair = *itPair;
        if(matchesCategories(pair,Category::PlayerAircraft, Category::EnemyAircraft))
        {
            auto & player = static_cast<Aircraft&>(*pair.first);
            auto & enemy = static_cast<Aircraft&>(*pair.second);

            player.damage(enemy.getHitpoints());
            enemy.destroy();
        }

        else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile)
        || matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
        {
            auto & aircraft = static_cast<Aircraft&>(*pair.first);
            auto & projectile = static_cast<Projectile&>(*pair.second);

            aircraft.damage(projectile.getDamage());
            projectile.destroy();
        }

        else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
        {
            mPlayerAircraft->playLocalSound(mQueue,SoundEffect::CollectPickup);
            auto & aircraft = static_cast<Aircraft&>(*pair.first);
            auto & pickup = static_cast<Pickup&>(*pair.second);

            pickup.apply(aircraft);
            pickup.destroy();


        }
    }
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.category = Category::Projectile | Category::EnemyAircraft | Category::Pickup;

    command.action = derivedAction<Entity>(
    [this] (Entity& e, sf::Time)
    {
        if(!getBattlefieldBounds().intersects(e.getBoundingRectangle()))
        {
            e.destroy();
            e.setOutside();
        }
    });

    mQueue.push(command);
}

bool matchesCategories(SceneNode::Pair & colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();

    if (type1 & category1 && type2 & category2)
    {
        return true;
    }

    else if (type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}

void World::updateSounds()
{
    mSoundPlayer.setListenerPosition(mPlayerAircraft->getWorldPosition());
    mSoundPlayer.removeStoppedSounds();
}

bool World::hasPlayer()
{
    return !mPlayerAircraft->isDestroyed();
}


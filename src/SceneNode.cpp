#include "SceneNode.h"
#include <cassert>
#include "utils.hpp"
#include "iostream"

SceneNode::SceneNode(Category::Type category) : mChildren(), mParent(nullptr), mDefaultCategory(category)
{
}

void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
    auto found  = std::find_if(mChildren.begin(), mChildren.end(),
    [&] (Ptr& p) -> bool {return p.get() == &node; });

    assert(found != mChildren.end());

    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;

}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
    for(auto itr = mChildren.begin(); itr!=mChildren.end(); ++itr)
    {
        (*itr)->update(dt, commands);
    }
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);

    // draw Children
    for(auto itr = mChildren.begin(); itr!=mChildren.end(); ++itr)
    {
        (*itr)->draw(target,states);
    }
}

void    SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Do nothing by default
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform trans = sf::Transform::Identity;
    for (const SceneNode* node = this; node!=nullptr; node = node->mParent)
    {
        trans = node->getTransform() * trans;
    }
    return trans;
}

unsigned int SceneNode::getCategory() const
{
    return mDefaultCategory;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
    if  (command.category & getCategory())
    {
        command.action(*this, dt);
    }

    for(auto itr = mChildren.begin(); itr!=mChildren.end(); ++itr)
    {
        (*itr)->onCommand(command, dt);
    }
}

sf::FloatRect SceneNode::getBoundingRectangle() const
{
    return sf::FloatRect();
}

bool SceneNode::isDestroyed() const
{
    return false;
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
    if(this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
    {
        collisionPairs.insert(std::minmax(this,&node));
    }

    for(auto child = mChildren.begin(); child != mChildren.end(); ++child)
    {
        (*child)->checkNodeCollision(node, collisionPairs);
    }
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
    checkNodeCollision(sceneGraph, collisionPairs);

    for(auto child = sceneGraph.mChildren.begin(); child != sceneGraph.mChildren.end(); ++child)
    {
        checkSceneCollision(*(*child), collisionPairs);
    }
}

void SceneNode::removeWrecks()
{
    auto wreckfieldBegin = std::remove_if(mChildren.begin(),mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
    mChildren.erase(wreckfieldBegin, mChildren.end());

    std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

bool SceneNode::isMarkedForRemoval() const
{
    return isDestroyed();
}


float distance(const SceneNode& Node1, const SceneNode& Node2)
{
    return length(Node1.getWorldPosition()-Node2.getWorldPosition());
}

bool collision(const SceneNode& Node1, const SceneNode& Node2)
{
    return Node1.getBoundingRectangle().intersects(Node2.getBoundingRectangle());
}




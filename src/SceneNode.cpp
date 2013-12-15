#include "SceneNode.h"
#include <cassert>

SceneNode::SceneNode()
{
    mParent = nullptr;
}

void SceneNode::attachCild(Ptr child)
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

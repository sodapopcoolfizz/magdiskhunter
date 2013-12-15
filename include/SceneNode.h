#ifndef SCENENODE_H
#define SCENENODE_H
#include <memory>
#include <vector>
#include "SFML/Graphics.hpp"

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;
    public:
        SceneNode();

        void    attachCild(Ptr Child);
        Ptr     detachChild(const SceneNode& node);
    private:
        std::vector<Ptr>    mChildren;
        SceneNode*          mParent;

    private:
        virtual void    draw(sf::RenderTarget& target, sf::RenderStates states) const final;
        virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // SCENENODE_H

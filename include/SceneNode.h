#ifndef SCENENODE_H
#define SCENENODE_H
#include <memory>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Command.hpp"
#include "Category.hpp"
#include "CommandQueue.h"


class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;

        enum Layer
        {
            Background,
            Air,
            LayerCount
        };

    public:
        explicit SceneNode(Category::Type category = Category::None);

        void    attachChild(Ptr Child);
        Ptr     detachChild(const SceneNode& node);
        void    update(sf::Time dt, CommandQueue& commands);

        sf::Transform getWorldTransform() const;
        sf::Vector2f getWorldPosition() const;

        virtual unsigned int getCategory() const;
        void onCommand(const Command& command, sf::Time dt);
    private:
        std::vector<Ptr>    mChildren;
        SceneNode*          mParent;
        Category::Type      mDefaultCategory;


    private:
        virtual void    draw(sf::RenderTarget& target, sf::RenderStates states) const final;
        virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

        virtual void    updateCurrent(sf::Time dt, CommandQueue& commands);
        void            updateChildren(sf::Time dt, CommandQueue& commands);

};

#endif // SCENENODE_H

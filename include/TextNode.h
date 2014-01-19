#ifndef TEXTNODE_H
#define TEXTNODE_H
#include "SceneNode.h"
#include "SFML/Graphics/Text.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

class TextNode : public SceneNode
{
public:
        explicit TextNode(const FontHolder& fonts, const std::string& text);

        void setString(const std::string& text);

private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::Text mText;
};

#endif // TEXTNODE_H

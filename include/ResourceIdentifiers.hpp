#ifndef RESOURCEIDENTIFIERS_HPP_INCLUDED
#define RESOURCEIDENTIFIERS_HPP_INCLUDED

#include "ResourceHolder.hpp"

namespace Textures
{
    enum ID
    {
        Desert,
        Umag,
        Disk,
    };

}

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

#endif // RESOURCEIDENTIFIERS_HPP_INCLUDED

#ifndef RESOURCEIDENTIFIERS_HPP_INCLUDED
#define RESOURCEIDENTIFIERS_HPP_INCLUDED

#include "ResourceHolder.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"

namespace Textures
{
    enum ID
    {
        Desert,
        Umag,
        Disk,
        Orb,
        TitleScreen,
        Bullet,
        Missile,
        HealthRefill,
        MissileRefill,
    };

}

namespace Fonts
{
    enum ID
    {
        Main,
        dialog,
    };
}

namespace Music
{
        enum ID
        {
            MenuTheme,
            LevelTheme,
        };
}

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif // RESOURCEIDENTIFIERS_HPP_INCLUDED

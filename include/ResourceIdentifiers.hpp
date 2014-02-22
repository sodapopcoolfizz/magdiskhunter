#ifndef RESOURCEIDENTIFIERS_HPP_INCLUDED
#define RESOURCEIDENTIFIERS_HPP_INCLUDED

#include "ResourceHolder.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

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
        FireSpread,
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

namespace SoundEffect
{
    enum ID
    {
        AlliedGunfire,
        EnemyGunfire,
        Explosion,
        LaunchMissile,
        CollectPickup,
        Text,
    };
}

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;

#endif // RESOURCEIDENTIFIERS_HPP_INCLUDED

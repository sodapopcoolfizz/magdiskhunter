#include "Aircraft.h"

Textures::ID toTextureID(Aircraft::Type type)
{
    switch (type)
    {
        case Aircraft::Umag:
            return Textures::Umag;
        case Aircraft::Disk:
            return Textures::Disk;
        case Aircraft::Orb:
            return Textures::Orb;
        default:
            return Textures::Umag;
    }

}

Aircraft::Aircraft(Type type, const TextureHolder& textures): mType(type), mSprite(textures.get(toTextureID(type)))
{
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

unsigned int Aircraft::getCategory() const
{
    switch (mType)
    {
        case Aircraft::Umag:
            return Category::PlayerAircraft;

        default:
            return Category::EnemyAircraft;
    }

}

void Aircraft::accelerate(sf::Vector2f velocity)
{
    this->setVelocity(getVelocity()+velocity);
}

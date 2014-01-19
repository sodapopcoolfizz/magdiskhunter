#include "utils.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <assert.h>


void centerOrigin(sf::Sprite& sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
}

void centerOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
}

float toDegree(float radian)
{
	return 180.f / 3.141592653589793238462643383f * radian;
}

float toRadian(float degree)
{
	return 3.141592653589793238462643383f / 180.f * degree;
}

float length(sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f unitVector(sf::Vector2f vector)
{
	assert(vector != sf::Vector2f(0.f, 0.f));
	return vector / length(vector);
}

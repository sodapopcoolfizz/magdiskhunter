#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Sprite;
	class Text;
}

template <typename T>
std::string	toString(const T& value);

void centerOrigin(sf::Sprite& sprite);

void centerOrigin(sf::Text& text);

float toDegree(float radian);

float toRadian(float degree);

float length(sf::Vector2f vector);

sf::Vector2f unitVector(sf::Vector2f vector);


#include <../src/utils.inl>
#endif

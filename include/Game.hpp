#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "World.h"
#include "Player.h"

class Game
{
    public:
        Game();
        void run();

    private:
        void processEvents();
        void update(sf::Time deltaTime);
        void render();

        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    private:
        sf::RenderWindow mWindow;
        Player mPlayer;
        sf::Texture mTexture;
        bool mIsPaused;
        World mWorld;


};

// Resource ID for sf::Texture
/*namespace Textures
{
	enum ID
	{
		Landscape,
		Airplane,
	};
}*/

int main()
{
    try
    {
    Game game;
    game.run();
    }
    catch (std::exception e)
    {
    std::cout << "\nEXCEPTION: "<< e.what() <<std::endl;
    }

}

#endif //GAME_HPP

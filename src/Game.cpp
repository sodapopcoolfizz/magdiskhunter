#include "ResourceHolder.hpp"
#include "Game.hpp"

const float playerSpeed = 100.f;
const sf::Time TimePerFrame = sf::seconds(1.f/60.f);



Game::Game() : mWindow(sf::VideoMode(640, 480), "SFML APPLICATION"), mPlayer(), mIsMovingDown(false), mIsMovingLeft(false), mIsMovingRight(false), mIsMovingUp(false)
, mWorld(mWindow)
{

    /***if(!mTexture.loadFromFile("../../Media/Textures/UMagnet.png"))
    {
        // Handle loading error
        std::cerr<<"Error while loading Texture";
    }*/

}

void Game::run()
{

    ResourceHolder<sf::Texture,Textures::ID> textures;
    try{
        textures.load(Textures::Umag,"../../Media/Textures/UMagnet.png");
    }
    catch (std::runtime_error& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
    mPlayer.setTexture(textures.get(Textures::Umag));
    mPlayer.setPosition(100.f,100.f);


    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen())
    {
        timeSinceLastUpdate +=clock.restart();
        while(timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate-=TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                mWindow.close();
                break;
            default :
                break;
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::Z)
         mIsMovingUp = isPressed;
    else if (key == sf::Keyboard::S)
        mIsMovingDown = isPressed;
    else if (key == sf::Keyboard::Q)
        mIsMovingLeft = isPressed;
    else if (key == sf::Keyboard::D)
        mIsMovingRight = isPressed;
}

void Game::update(sf::Time deltaTime)
{
    sf::Vector2f movement(0.f,0.f);
    if (mIsMovingUp)
        movement.y -=playerSpeed;
    if (mIsMovingDown)
        movement.y +=playerSpeed;
    if (mIsMovingLeft)
        movement.x -=playerSpeed;
    if (mIsMovingRight)
        movement.x +=playerSpeed;
    mPlayer.move(movement * deltaTime.asSeconds());
    mWorld.update(deltaTime);
}

void Game::render()
{
    mWindow.clear();
    mWorld.draw();
    mWindow.setView(mWindow.getDefaultView());

    mWindow.display();
}


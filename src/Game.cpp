#include "ResourceHolder.hpp"
#include "Game.hpp"

const float playerSpeed = 100.f;
const sf::Time TimePerFrame = sf::seconds(1.f/60.f);



Game::Game() : mWindow(sf::VideoMode(640, 480), "SFML APPLICATION"), mPlayer(), mIsPaused(false)
, mWorld(mWindow)
{
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen())
    {
        timeSinceLastUpdate +=clock.restart();
        while(timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate-=TimePerFrame;
            processEvents();
            if(!mIsPaused)
            {
                update(TimePerFrame);
            }
        }
        render();
    }
}

void Game::processEvents()
{
    CommandQueue& commands = mWorld.getCommandQueue();

    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        mPlayer.handleEvent(event,commands);
        if (event.type == sf::Event::GainedFocus)
            mIsPaused = false;
        else if (event.type == sf::Event::LostFocus)
            mIsPaused = true;
        else if (event.type == sf::Event::Closed)
            mWindow.close();
    }

    mPlayer.handleRealtimeInput(commands);
}


void Game::update(sf::Time deltaTime)
{
    sf::Vector2f movement(0.f,0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        movement.y -=playerSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y +=playerSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        movement.x -=playerSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x +=playerSpeed;
    mWorld.update(deltaTime);
}

void Game::render()
{
    mWindow.clear();
    mWorld.draw();
    mWindow.setView(mWindow.getDefaultView());

    mWindow.display();
}


#include "Application.h"
#include "TitleState.h"
#include "GameState.h"
#include "PauseState.h"
#include "MenuState.h"
#include "StateStack.h"
#include "GameOverState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(600, 480), "Mag Disk Hunter", sf::Style::Resize)
, mTextures()
, mFonts()
, mPlayer()
, mMusic()
, mSound()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer,mMusic,mSound))
{
    mFonts.load(Fonts::Main, "../../Media/alphbeta.ttf");
    mTextures.load(Textures::TitleScreen, "../../Media/Textures/TitleScreen.png");

    registerStates();
    mStateStack.pushState(States::Title);
}


void Application::processInput()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);
        if (event.type == sf::Event::Closed)
			mWindow.close();
    }
}
void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<GameOverState>(States::GameOver);

}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();


}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while(mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate +=dt;
        while(timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -=TimePerFrame;

            processInput();
            update(TimePerFrame);

            // si apres update, la pile est vide :
            if(mStateStack.isEmpty())
                mWindow.close();
        }
        render();
    }
}

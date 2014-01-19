#include "GameState.h"

GameState::GameState(StateStack& sstack, Context context)
: State(sstack, context)
, mWorld(*context.mWindow)
, mPlayer(*context.mPlayer)
{
}

void GameState::draw()
{
    mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    return true;// on empeche pas les autres etat de update
}

bool GameState::handleEvent(const sf::Event& event)
{
    // input player (non Temps-réel, voir update
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    // si echap, pause
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestStackPush(States::Pause);
    }
    return true; // on empeche pas les autres etats de handleEvent
}


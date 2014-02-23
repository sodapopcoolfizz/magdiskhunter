#include "GameState.h"
#include "ResourceIdentifiers.hpp"

GameState::GameState(StateStack& sstack, Context context)
: State(sstack, context)
, mWorld(*context.mWindow,*context.mSoundPlayer)
, mPlayer(*context.mPlayer)
{
    //context.mMusicPlayer->play(Music::LevelTheme);
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

    if(!mWorld.hasPlayer())
    {
        //requestStackPop();
        requestStackPush(States::GameOver);
    }

    return true; // on empeche pas les autres etats de handleEvent


}


#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "World.h"
#include "Player.h"

class GameState : public State
{
    public:
        GameState(StateStack& sstack, Context context);

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);

    private:
        World mWorld;
        Player mPlayer;
};

#endif // GAMESTATE_H

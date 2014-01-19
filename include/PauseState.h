#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "State.h"

class PauseState : public State
{
    public:
        PauseState(StateStack& sstack, Context context);

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        sf::Sprite mBackgroundSprite;
        sf::Text mPausedText;
        sf::Text mInstructionText;
};

#endif // PAUSESTATE_H

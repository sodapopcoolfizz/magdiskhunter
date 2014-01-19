#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "State.h"

class TitleState : public State
{
    public:
        TitleState(StateStack& sstack, Context context);

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);

    private:

        sf::Sprite mBackgroundSprite;
        sf::Text mText;

        bool mShowText;
        sf::Time mTextEffectTime;
};

#endif // TITLESTATE_H

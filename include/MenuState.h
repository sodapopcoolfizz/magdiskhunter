#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"

class MenuState : public State
{
    public:
        MenuState(StateStack& sstack, Context context);

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);

        void updateOptionText(); //controls highlight

    private:
        enum OptionNames
        {
            Play,
            Exit,
        };

    private:
        sf::Sprite mBackgroundSprite;
        std::vector<sf::Text> mOptions;
        std::size_t mOptionIndex;
};

#endif // MENUSTATE_H

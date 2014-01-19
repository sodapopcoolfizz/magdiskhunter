#include "TitleState.h"
#include "utils.hpp"

TitleState::TitleState(StateStack& sstack, Context context)
: State(sstack,context)
, mText()
, mShowText(true)
, mTextEffectTime(sf::Time::Zero)
{
    mBackgroundSprite.setTexture(context.mTextures->get(Textures::TitleScreen));

    mText.setFont(context.mFonts->get(Fonts::Main));
    mText.setString("Press any key to start");
    centerOrigin(mText);
    mText.setPosition(context.mWindow->getView().getSize()/2.f);
}

void TitleState::draw()
{
    sf::RenderWindow& window = *getContext().mWindow;
    window.draw(mBackgroundSprite);

    if(mShowText)
        window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
    mTextEffectTime +=dt;

    if(mTextEffectTime >= sf::seconds(0.5f))
    {
        mShowText = !mShowText;
        mTextEffectTime = sf::Time::Zero;
    }
    return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }
    return true;
}


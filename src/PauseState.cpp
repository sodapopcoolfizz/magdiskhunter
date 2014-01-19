#include "PauseState.h"
#include "SFML/Graphics/Rect.hpp"
#include "utils.hpp"

PauseState::PauseState(StateStack& sstack, Context context)
: State(sstack,context)
, mBackgroundSprite()
, mPausedText()
, mInstructionText()
{
    sf::Font& font = context.mFonts->get(Fonts::Main);
    sf::Vector2f viewSize = context.mWindow->getView().getSize();

    mPausedText.setFont(font);
    mPausedText.setString("Game Paused");
    mPausedText.setCharacterSize(40);
    centerOrigin(mPausedText);
    mPausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

    mInstructionText.setFont(font);
    mInstructionText.setString("(Press Backspace to return to the main menu)");
    mInstructionText.setCharacterSize(20);
    centerOrigin(mInstructionText);
    mInstructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}

void PauseState::draw()
{
    sf::RenderWindow& window = *getContext().mWindow;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0,0,0,150));
    backgroundShape.setSize(window.getView().getSize()/2.f);
    sf::FloatRect bounds = backgroundShape.getLocalBounds();
    backgroundShape.setOrigin(bounds.width/2.f,bounds.height/2.f);
    backgroundShape.setPosition(window.getView().getSize()/2.f);

    window.draw(backgroundShape);
    window.draw(mPausedText);
    window.draw(mInstructionText);
}

bool PauseState::update(sf::Time dt)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{

    if (event.type != sf::Event::KeyPressed) //sinon, il faut rester appuyé constament
		return false;

    if(event.key.code == sf::Keyboard::Escape)
    {
        //Escape is pressed, go back to previous state(game)
        requestStackPop();
    }

    if(event.key.code == sf::Keyboard::BackSpace)
    {
        //Escape pressed, clear all the things, return to menu
        requestStackClear();
        requestStackPush(States::Menu);
    }

    return false;
}



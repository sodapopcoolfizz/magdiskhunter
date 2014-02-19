#include "MenuState.h"
#include "utils.hpp"
#include "ResourceIdentifiers.hpp"

MenuState::MenuState(StateStack& sstack, Context context)
: State(sstack,context)
, mOptions()
, mOptionIndex(0)
{
    sf::Texture& texture = context.mTextures->get(Textures::TitleScreen);
    sf::Font& font = context.mFonts->get(Fonts::Main);

    mBackgroundSprite.setTexture(texture);

    sf::Text playOption;
    playOption.setFont(font);
    playOption.setString("Play");
    centerOrigin(playOption);
    playOption.setPosition(context.mWindow->getView().getSize() / 2.f);
    mOptions.push_back(playOption);

    sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString("Exit");
    centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	mOptions.push_back(exitOption);

    updateOptionText();

    context.mMusicPlayer->play(Music::MenuTheme);
}

void MenuState::draw()
{
    sf::RenderWindow& window = *getContext().mWindow;

    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);

    for(auto itr=mOptions.begin(); itr!=mOptions.end();++itr)
    {
        window.draw(*itr);
    }
}

bool MenuState::update(sf::Time)
{
    return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{

    if (event.type != sf::Event::KeyPressed)
		return true;

    if(event.key.code == sf::Keyboard::Return)
    {
        if(mOptionIndex == Play)
        {
            requestStackPop();
            requestStackPush(States::Game);
        }

        else if(mOptionIndex == Exit)
        {
            requestStackPop();
        }
    }

    else if(event.key.code == sf::Keyboard::Up)
    {
        if(mOptionIndex > 0)
        {
            mOptionIndex--;
        }
        else
        {
            mOptionIndex = mOptions.size() -1;
        }
        updateOptionText();
    }

    else if (event.key.code == sf::Keyboard::Down)
	{
		if (mOptionIndex < mOptions.size() - 1)
		{
            mOptionIndex++;
		}
		else
		{
            mOptionIndex = 0;
		}

		updateOptionText();
	}

    return true; //other states can handle events
}

void MenuState::updateOptionText()
{
    if(mOptions.empty())
     return;

     // All white
     for(auto itr=mOptions.begin();itr!=mOptions.end();++itr)
     {
        itr->setColor(sf::Color::White);
     }

     // Selected is blue
     mOptions[mOptionIndex].setColor(sf::Color(86, 204, 141));
}


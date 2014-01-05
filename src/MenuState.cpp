#include "MenuState.h"

MenuState::MenuState(StateStack& sstack, Context context)
: mState(sstack,context)
, mOptions()
, mOptionIndex(0)
{
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    sf::Font& font = context.fonts->get(Fonts::Main);

    mBackgroundSprite.setTexture(texture);

    sf::Text playOption;
    playOption.setFont(font);
    playOption.setString("Play");
    playOption.setPosition(context.window->getView().getSize() / 2.f);
    mOptions.push_back(playOption);

    sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString("Exit");
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	mOptions.push_back(exitOption);

    updateOptionText();
}

void MenuState::draw()
{
    sf::RenderWindow& window = *getContext().window;

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
    if(event.key.cpde == sf::Keyboard::Return)
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
            mOptionIndex--:
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
     mOptions[mOptinIndex].setColor(sf::Color::Blue);
}


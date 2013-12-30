#include "Player.h"
#include <iostream>

struct AircraftMover
{
    AircraftMover(float vx, float vy) : velocity(vx,vy)
    {}

    void operator() (Aircraft& aircraft, sf::Time) const
    {
        aircraft.accelerate(velocity);
    }

    sf::Vector2f velocity;
};

Player::Player()
{
    // initialise les controles clavier
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;

	//initialise les actions
	initializeCommands();



}

void Player::handleRealtimeInput(CommandQueue& commands)
{

   for(auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();++itr)
   {
        if(sf::Keyboard::isKeyPressed(itr->first) && isRealtimeAction(itr->second))
        {
            commands.push(mActionBinding[itr->second]);
        }
   }

}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
    {
        Command output;
        output.category = Category::PlayerAircraft;
        output.action = [] (SceneNode& s, sf::Time)
        {
            std::cout << s.getPosition().x << ","
                      << s.getPosition().y << "\n";
        };
        commands.push(output);
    }
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); ++itr)
	{
		if (itr->second == action)
			mKeyBinding.erase(itr);
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for(auto itr = mKeyBinding.begin(); itr!=mKeyBinding.end(); ++itr)
	{
		if (itr->second == action)
			return itr->first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeCommands()
{
	const float playerSpeed = 200.f;

	mActionBinding[MoveLeft].action	 = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));
	mActionBinding[MoveUp].action    = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
	mActionBinding[MoveDown].action  = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));

	for(auto itr = mActionBinding.begin(); itr!=mActionBinding.end();++itr)
	{
        itr->second.category = Category::PlayerAircraft;

	}
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
			return true;

		default:
			return false;
	}
}

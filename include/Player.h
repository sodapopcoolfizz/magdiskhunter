#ifndef PLAYER_H
#define PLAYER_H

#include "CommandQueue.h"
#include "SFML/Window/Event.hpp"
#include "SFML/System/Vector2.hpp"
#include "Aircraft.h"
#include <map>

class Player
{
    public:

        enum Action
        {
            MoveLeft,
            MoveRight,
            MoveUp,
            MoveDown,
            Fire,
            LaunchMissile,
        };

        Player();



        void assignKey(Action action, sf::Keyboard::Key key);

        sf::Keyboard::Key   getAssignedKey(Action action) const;

        void handleEvent(const sf::Event& event, CommandQueue& commands);
        void handleRealtimeInput(CommandQueue& commands);
    private:
        static bool isRealtimeAction(Action action);
        void initializeCommands();

    private:
        std::map<sf::Keyboard::Key, Action> mKeyBinding;
        std::map<Action, Command> mActionBinding;
};

#endif // PLAYER_H

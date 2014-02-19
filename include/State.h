#ifndef STATE_H
#define STATE_H

#include "StateIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Player.h"
#include "SFML/Graphics.hpp"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include <functional>

class StateStack;

class State
{
    public:
        typedef std::unique_ptr<State> Ptr;
        struct Context
        {
            Context(sf::RenderWindow& window,
            TextureHolder& textures,
            FontHolder& fonts,
            Player& player,
            MusicPlayer& music,
            SoundPlayer& sound);

            sf::RenderWindow* mWindow;
            TextureHolder* mTextures;
            FontHolder* mFonts;
            Player* mPlayer;
            MusicPlayer * mMusicPlayer;
            SoundPlayer * mSoundPlayer;

        };

    public:
        State(StateStack& sstack, Context context);
        virtual ~State();

        virtual void draw() = 0;
        virtual bool update(sf::Time dt) = 0;
        virtual bool handleEvent(const sf::Event& event) = 0;

    protected:
        void requestStackPush(States::ID stateID);
        void requestStackPop();
        void requestStackClear();

        Context getContext() const;

    private:
        StateStack* mStack;
        Context mContext;
};

#endif // STATE_H

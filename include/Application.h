#ifndef APPLICATION_H
#define APPLICATION_H

#include "StateStack.h"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "MusicPlayer.h"

class Application
{
    public:
        Application();
        void run();
    private:
        void processInput();
        void update(sf::Time dt);
        void render();
        void registerStates();

    private:
        static const sf::Time	TimePerFrame;

        StateStack mStateStack;

        sf::RenderWindow		mWindow;
        TextureHolder			mTextures;
	  	FontHolder				mFonts;
		Player					mPlayer;
		MusicPlayer             mMusic;

};

#endif // APPLICATION_H

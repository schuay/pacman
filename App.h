#pragma once
#include "Main.h"

class Sounds;

class App
{
public:
	App();
	~App();             //SDL systems are shut down in destructor

    void InitApp();     //SDL systems are initialized
    void InitWindow();  //creates window with parameters from loaded settings file
    void InitSound();

	//////////////
	//Global
	//////////////

	inline bool getQuit() const { return quit; }
	void setQuit( bool q ) { quit = q; }
	SDL_Surface* getScreen() const { return screen; }
	SDL_Surface* getBuf() const { return buf; }
	Sounds* getSnd() { return snd; }

private:

	//////////////
	//Global
	//////////////

	bool				quit;       //if true then quit application
	SDL_Surface         *screen,    //screen surface
                        *buf;       //buffer surface
    Sounds *snd;

};

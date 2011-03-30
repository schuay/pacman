/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#pragma once

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

#include "Sounds.h"

using boost::shared_ptr;

class Sounds;

class App
{
public:
    App();

    void InitApp();     //SDL systems are initialized
    void InitWindow();  //creates window with parameters from loaded settings file
    void InitSound();

    //////////////
    //Global
    //////////////

    inline bool getQuit() const { return quit; }
    void setQuit( bool q ) { quit = q; }
    shared_ptr<sf::RenderWindow> getScreen() const { return screen; }
    shared_ptr<Sounds> getSnd() { return snd; }

private:

    //////////////
    //Global
    //////////////

    bool
            quit;       //if true then quit application

    shared_ptr<sf::RenderWindow>
            screen;       //buffer surface

    shared_ptr<Sounds>
            snd;

};

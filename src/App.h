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
#include <vector>
#include <cstdlib>
#include <sys/stat.h>

#include "Logger.h"
#include "Settings.h"
#include "Sounds.h"
#include "Defines.h"

using boost::shared_ptr;
using std::string;
using std::vector;

class Sounds;

class App
{
public:
    App();

    void initApp();
    void initWindow();
    void initSound();

    inline bool getQuit() const { return quit; }
    void setQuit( bool q ) { quit = q; }
    shared_ptr<sf::RenderWindow> getWindow() const { return window; }
    shared_ptr<Sounds> getSnd() { return snd; }

    /* look for file in search paths and return first instance */
    string findFile(string filename);

private:

    bool quit;
    shared_ptr<sf::RenderWindow> window;
    shared_ptr<Sounds> snd;

    vector<string> searchpaths;
};

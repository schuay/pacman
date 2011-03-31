/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#pragma once

#include <SFML/Audio.hpp>
#include <boost/shared_ptr.hpp>

#include "App.h"
#include "Error.h"
#include "Logger.h"
#include "Defines.h"

using boost::shared_ptr;

#define NUMOFSOUNDS 13

class Sounds
{
public:
    Sounds();

    bool init();
    void play(int i, bool looped=0, int volume=100);
    void modify( int sound, long freq, long volume=100);
    void stop(int i);
    void stop();
    void toggleSounds();
    bool on;

private:
    shared_ptr<sf::SoundBuffer>
            sndbuf[NUMOFSOUNDS];
    shared_ptr<sf::Sound>
            snd[NUMOFSOUNDS];

    std::string
            sndPaths[NUMOFSOUNDS];

    bool
            isinit;
};

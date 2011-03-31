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
#include <map>

#include "App.h"
#include "Error.h"
#include "Logger.h"
#include "Defines.h"

using boost::shared_ptr;
using std::map;
using std::pair;
using std::string;

enum SoundsEnum {
    INTRO = 0,
    MUNCH_A = 1,
    MUNCH_B = 2,
    LARGE_PELLET = 3,
    GHOST_EAT = 4,
    FRUIT = 5,
    EXTRA_MAN = 6,
    VULN = 7,
    DEATH = 8,
    NEWGAME = 9,
    SIREN = 10,
    INTERMISSION = 11,
    BOOSTER = 12
};

class Sounds
{
public:
    Sounds();

    bool init();
    void play(SoundsEnum i, bool looped);
    void modify(SoundsEnum i, float pitch, int volume = 100);
    void stop(SoundsEnum i);
    void stop();
    void toggleSounds();
    bool getOn() const { return on; }

private:

    map<SoundsEnum, shared_ptr<sf::SoundBuffer> > sndbuf;
    map<SoundsEnum, shared_ptr<sf::Sound> > snd;
    map<SoundsEnum, string> sndnames;

    bool on;
    bool isinit;
};

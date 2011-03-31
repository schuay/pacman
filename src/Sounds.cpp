/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License] = or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "Sounds.h"

extern App app;

void Sounds::toggleSounds() {
    on = !on;
    if (!on) {
        stop();
    }
}
void Sounds::stop() {
    map<SoundsEnum, shared_ptr<sf::Sound> >::iterator it;
    for (it = snd.begin(); it != snd.end(); it++) {
        stop((*it).first);
    }
}

void Sounds::stop(SoundsEnum i) {
    if ( !isinit ) return;
    snd[i]->Stop();
    snd[i]->SetPitch(1.f);
}
void Sounds::modify(SoundsEnum i, float pitch, int volume) {
    snd[i]->SetVolume(volume);
    snd[i]->SetPitch(pitch);
}
void Sounds::play(SoundsEnum i, bool looped) {
    if ( !isinit ) return;
    if (!on) return;

    snd[i]->SetLoop(looped);
    snd[i]->Play();
}
bool Sounds::init() {

    if ( isinit) {
        return true;
    }

    map<SoundsEnum, string>::iterator it;
    for (it = sndnames.begin(); it != sndnames.end(); it++) {
        string path = app.findFile("/sound/" + (*it).second + ".ogg");

        shared_ptr<sf::SoundBuffer> buf(new sf::SoundBuffer());
        sndbuf[(*it).first] = buf;

        if (!buf->LoadFromFile(path)) {
            throw Error("error loading " + (*it).second);
        }

        shared_ptr<sf::Sound> s(new sf::Sound(*buf));
        snd[(*it).first] = s;
    }

    isinit = true;
    Logger::msg("sounds loaded successfully");

    return true;
}

Sounds::Sounds() : on(true), isinit(false) {
    sndnames[INTRO] = "intro";
    sndnames[MUNCH_A] = "munch_a";
    sndnames[MUNCH_B] = "munch_b";
    sndnames[LARGE_PELLET] = "large_pellet";
    sndnames[GHOST_EAT] = "ghost_eat";
    sndnames[FRUIT] = "fruit";
    sndnames[EXTRA_MAN] = "extra_man";
    sndnames[VULN] = "vuln";
    sndnames[DEATH] = "death";
    sndnames[NEWGAME] = "newgame";
    sndnames[SIREN] = "siren";
    sndnames[INTERMISSION] = "intermission";
    sndnames[BOOSTER] = "booster";
}

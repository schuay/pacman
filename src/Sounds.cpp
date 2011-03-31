/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "Sounds.h"

extern App app;

void Sounds::toggleSounds() {
    if ( on ) {
        on = false;
        stop();
    }
    else
        on = true;
}
void Sounds::stop() {
    int i;
    for (i=0; i < NUMOFSOUNDS; i++)
        stop(i);
}

void Sounds::stop(int i) {
    if ( !isinit ) return;
    snd[i]->Stop();
    snd[i]->SetPitch(1.f);
}
void Sounds::modify( int sound, long freq, long volume) {
        snd[sound]->SetVolume(volume);
        snd[sound]->SetPitch(freq / 44100.f);
}
void Sounds::play(int i, bool looped, int volume) {
    /* frequency used to be a parameter here until switching to sdl
     * we might want to reenable that functionality sometime */

    if ( !isinit ) return;
    if (!on) return;

    snd[i]->SetLoop(looped);
    snd[i]->SetVolume(volume);

    snd[i]->Play();
}
bool Sounds::init() {

    if ( isinit)
        return true;

    try {
        int i;
        for (i=0; i < NUMOFSOUNDS; i++) {
            string path = APP_PATH "/" + sndPaths[i];
            sf::SoundBuffer *buf = new sf::SoundBuffer();
            sndbuf[i].reset(buf);
            if (!sndbuf[i]->LoadFromFile(path))
                throw Error("Error loading " + sndPaths[i]);
            snd[i].reset(new sf::Sound(*buf));
        }

        isinit = true;
        Logger::Msg("sounds loaded successfully");
    }
    catch ( Error& err ) {
        Logger::Err(err.getDesc());
    }
    catch ( ... ) {
        Logger::Err("Unexpected exception");
    }
    return true;
}

Sounds::Sounds() :
        on(true), isinit(false)
{
    //set sound paths
    sndPaths[0] = "sound/intro.ogg";
    sndPaths[1] = "sound/munch_a.ogg";
    sndPaths[2] = "sound/munch_b.ogg";
    sndPaths[3] = "sound/large_pellet.ogg";
    sndPaths[4] = "sound/ghost_eat.ogg";
    sndPaths[5] = "sound/fruit.ogg";
    sndPaths[6] = "sound/extra_man.ogg";
    sndPaths[7] = "sound/vuln.ogg";
    sndPaths[8] = "sound/death.ogg";
    sndPaths[9] = "sound/newgame.ogg";
    sndPaths[10] = "sound/siren.ogg";
    sndPaths[11] = "sound/intermission.ogg";
    sndPaths[12] = "sound/booster.ogg";
}


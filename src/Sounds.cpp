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
extern Log logtxt;

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
}
//void Sounds::modify( int sound, long freq, long volume, long pan) {
//	snd[sound]->Modify(freq, volume, pan);
//}
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
        logtxt.print("Sounds loaded successfully");
    }
    catch ( Error& err ) {
            std::cerr << (err.getDesc() );
            logtxt.print( err.getDesc() );
    }
    catch ( ... ) {
        std::cerr << "Unexpected exception";
        logtxt.print( "Unexpected exception in App::App()" );
    }
    return true;
}

Sounds::Sounds() :
        on(true), isinit(false)
{
    //set sound paths
    sndPaths[0] = "sound/intro.mp3";
    sndPaths[1] = "sound/munch_a.mp3";
    sndPaths[2] = "sound/munch_b.mp3";
    sndPaths[3] = "sound/large_pellet.mp3";
    sndPaths[4] = "sound/ghost_eat.mp3";
    sndPaths[5] = "sound/fruit.mp3";
    sndPaths[6] = "sound/extra_man.mp3";
    sndPaths[7] = "sound/vuln.mp3";
    sndPaths[8] = "sound/death.mp3";
    sndPaths[9] = "sound/newgame.mp3";
    sndPaths[10] = "sound/siren.mp3";
    sndPaths[11] = "sound/intermission.mp3";
    sndPaths[12] = "sound/booster.mp3";
}


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
    on = !on;
    if (!on) {
        stop();
    }
}
void Sounds::stop() {
    for (int i = 0; i < NUMOFSOUNDS; i++) {
        stop(i);
    }
}

void Sounds::stop(int i) {
    if ( !isinit ) return;
    snd[i]->Stop();
    snd[i]->SetPitch(1.f);
}
void Sounds::modify(int i, float pitch, int volume) {
    snd[i]->SetVolume(volume);
    snd[i]->SetPitch(pitch);
}
void Sounds::play(int i, bool looped) {
    if ( !isinit ) return;
    if (!on) return;

    snd[i]->SetLoop(looped);
    snd[i]->Play();
}
bool Sounds::init() {

    if ( isinit) {
        return true;
    }

    const string paths[] = {"intro", "munch_a", "munch_b", "large_pellet",
                            "ghost_eat", "fruit", "extra_man", "vuln",
                            "death", "newgame", "siren", "intermission", "booster"};

    for (int i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
        string path = APP_PATH "/sound/" + paths[i] + ".ogg";

        sf::SoundBuffer *buf = new sf::SoundBuffer();
        sndbuf[i].reset(buf);
        if (!sndbuf[i]->LoadFromFile(path)) {
            throw Error("Error loading " + paths[i]);
        }

        snd[i].reset(new sf::Sound(*buf));
    }

    isinit = true;
    Logger::msg("sounds loaded successfully");

    return true;
}


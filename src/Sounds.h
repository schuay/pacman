#pragma once

#include "Main.h"

#define NUMOFSOUNDS 13

class Sounds
{
public:
    Sounds();
    ~Sounds();
    bool init();
    void play(int i, bool looped=0, int freq=-1, int volume=128);
//    void modify( int sound, long freq, long volume=0, long pan=0 );
    void stop(int i);
    void stop();
    void toggleSounds();
    bool on;
private:
    Mix_Chunk *snd[NUMOFSOUNDS];
    std::string sndPaths[NUMOFSOUNDS];
    bool isinit;
};

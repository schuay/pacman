#include "Sounds.h"

extern App app;
extern Log logtxt;

void Sounds::toggleSounds() {

	if ( on ) {
		on = false;
		stop();
	}
	else {
		on = true;
	}
}
void Sounds::stop() {
	int i;
	for (i=0;i<NUMOFSOUNDS;i++)
		stop(i);
}

void Sounds::stop(int i) {
	if ( !isinit ) return;
	if (Mix_Playing(i))
	    Mix_HaltChannel(i);
}
//void Sounds::modify( int sound, long freq, long volume, long pan) {
//	snd[sound]->Modify(freq, volume, pan);
//}
void Sounds::play(int i, bool looped, int freq, int volume) {
	if ( !isinit ) return;
	if (!on) return;
//	if ( snd[i]->IsSoundPlaying() ) {
//		snd[i]->Stop();
//		snd[i]->Reset();
//	}
//	snd[i]->Play(0, (DWORD)looped, volume, freq);

	if (Mix_Playing(i))
	    Mix_HaltChannel(i);

    int loop = 0;
    if ( looped )
        loop = -1;

    Mix_Volume(i,volume);
    Mix_PlayChannel(i,snd[i],looped);
}
bool Sounds::init() {

	if ( isinit)
        return true;

	try {
        //initialize SDL mixer
	    int audio_rate = 44100;
	    Uint16 audio_format = AUDIO_S16SYS;
	    int audio_channels = 2;
	    int audio_buffers = 512;

	    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
            throw Error("Error while initializing SDL");
	    }

	    Mix_AllocateChannels(NUMOFSOUNDS);

        //load wav files
	    int i;
	    for (i=0;i<NUMOFSOUNDS;i++) {
	        snd[i] = Mix_LoadWAV(sndPaths[i].c_str());
	        if ( snd[i] == NULL )
                throw Error(Mix_GetError());
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
	on(true),
		isinit(false)
{
	int i;
	for (i=0;i<NUMOFSOUNDS;i++)
        snd[i]=NULL;

    //set sound paths
    sndPaths[0] = "sound/intro.wav";
    sndPaths[1] = "sound/munch_a.wav";
    sndPaths[2] = "sound/munch_b.wav";
    sndPaths[3] = "sound/large_pellet.wav";
    sndPaths[4] = "sound/ghost_eat.wav";
    sndPaths[5] = "sound/fruit.wav";
    sndPaths[6] = "sound/extra_man.wav";
    sndPaths[7] = "sound/vuln.wav";
    sndPaths[8] = "sound/death.wav";
    sndPaths[9] = "sound/newgame.wav";
    sndPaths[10] = "sound/siren.wav";
    sndPaths[11] = "sound/intermission.wav";
    sndPaths[12] = "sound/booster.wav";
}

Sounds::~Sounds()
{
	int i;
	for (i=0;i<NUMOFSOUNDS;i++) {
	    if (snd[i]) Mix_FreeChunk(snd[i]);
	    snd[i]=NULL;

    Mix_CloseAudio();
	}
}

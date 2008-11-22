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
		CheckMenuItem( app.getMenuHandle(), ID_CONFIGURATION_SOUND, MF_CHECKED);
	}
}
void Sounds::stop() {
	int i;

	if ( !isinit ) return;
	for (i=0;i<NUMOFSOUNDS;i++) {
		if ( snd[i]->IsSoundPlaying() ) {
			snd[i]->Stop();
			snd[i]->Reset();
		}
	}
}

void Sounds::stop(int i) {
	if ( !isinit ) return;
	if ( snd[i]->IsSoundPlaying() ) {
		snd[i]->Stop();
		snd[i]->Reset();
	}
}
void Sounds::modify( int sound, long freq, long volume, long pan) {
	snd[sound]->Modify(freq, volume, pan);
}
void Sounds::play(int i, bool looped, int freq, int volume) {
	if ( !isinit ) return;
	if (!on) return;
	if ( snd[i]->IsSoundPlaying() ) {
		snd[i]->Stop();
		snd[i]->Reset();
	}
	snd[i]->Play(0, (DWORD)looped, volume, freq);
}
bool Sounds::init() {
	
	if ( isinit) return true;
	isinit = true;

	try {
		if ( mngr.Initialize( app.getWindowHandle(), DSSCL_PRIORITY) < 0 )
			throw Error("Error initializing SoundManager");
		if ( mngr.SetPrimaryBufferFormat( 2, 44100, 16) < 0 ) 
			throw Error("Error setting primary buffer format");
		logtxt.print("SoundManager initialized successfully");

		if ( mngr.Create( &snd[0], "sound\\intro.wav") < 0 || //
			mngr.Create( &snd[1], "sound\\munch_a.wav", (DWORD)DSBCAPS_CTRLVOLUME) < 0 ||
			mngr.Create( &snd[2], "sound\\munch_b.wav", (DWORD)DSBCAPS_CTRLVOLUME) < 0 ||
			mngr.Create( &snd[3], "sound\\large_pellet.wav") < 0 ||
			mngr.Create( &snd[4], "sound\\ghost_eat.wav") < 0 ||
			mngr.Create( &snd[5], "sound\\fruit.wav") < 0 ||
			mngr.Create( &snd[6], "sound\\extra_man.wav") < 0 ||
			mngr.Create( &snd[7], "sound\\vuln.wav", (DWORD)DSBCAPS_CTRLFREQUENCY) < 0 ||
			mngr.Create( &snd[8], "sound\\death.wav") < 0 || 
			mngr.Create( &snd[9], "sound\\newgame.wav") < 0 ||
			mngr.Create( &snd[10], "sound\\siren.wav", (DWORD)DSBCAPS_CTRLFREQUENCY) < 0 || //
			mngr.Create( &snd[11], "sound\\intermission.wav") < 0 ||
			mngr.Create( &snd[12], "sound\\booster.wav") < 0 )
			throw Error("Error loading sounds");
		logtxt.print("Sounds loaded successfully");
	}
	catch ( Error& err ) {
		MessageBox(NULL, (err.getDesc() ).c_str(), "Sounds::init()", MB_OK | MB_ICONERROR);
		app.setQuit(true);
		logtxt.print( err.getDesc() );
		return false;
	}
	catch ( ... ) {
		MessageBox(NULL, "Unexpected exception", "Sounds::init", MB_OK | MB_ICONERROR);
		app.setQuit(true);
		logtxt.print( "Unexpected error" );
		return false;
	}
	return true;
}

Sounds::Sounds() :
	on(true),
		isinit(false)
{
	int i;

	for (i=0;i<NUMOFSOUNDS;i++) snd[i]=NULL;
}

Sounds::~Sounds()
{
	int i;

	for (i=0;i<NUMOFSOUNDS;i++) if (snd[i]) delete snd[i];
}

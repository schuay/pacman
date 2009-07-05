#include "App.h"

extern Log logtxt;
extern Settings settings;

void App::InitWindow() {
    try {
        int bpp(32);

        if ( !settings.fieldwidth || !settings.fieldheight || !settings.tilesize ) {
            logtxt.print("fieldheight/fieldwidth/tilesize is not set, reverting to default window dimensions");
            std::cerr << "fieldheight/fieldwidth/tilesize is not set, reverting to default window dimensions";
        }
        else {
            settings.height=settings.fieldheight*settings.tilesize;
            settings.width=settings.fieldwidth*settings.tilesize;
        }

        if ( screen )
            SDL_FreeSurface(screen);

        screen = SDL_SetVideoMode( settings.width,
            settings.height+EXTRA_Y_SPACE,
            bpp,         //bits per pixel; todo-make this dynamic
            SDL_NOFRAME | SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_ANYFORMAT );

        if (screen == NULL)
            throw Error("Error while setting video mode");

        logtxt.print("Video mode set successfully");
    }
    catch ( Error& err ) {
		std::cerr << (err.getDesc() );
		setQuit(true);
		logtxt.print( err.getDesc() );
	}
    catch (...) {
        std::cerr << "Unexpected exception";
		setQuit(true);
		logtxt.print( "Unexpected exception in App::App()" );
    }
}

void App::InitApp() {

    try {

        if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
            throw Error("Error while initializing SDL");

        logtxt.print("SDL systems initialized");

        if ( TTF_Init() < 0 )
            throw Error("Error while initializing SDL_ttf");

        logtxt.print("SDL_ttf initialized");
    }
	catch ( Error& err ) {
		std::cerr << (err.getDesc() );
		setQuit(true);
		logtxt.print( err.getDesc() );
	}
    catch (...) {
        std::cerr << "Unexpected exception";
		setQuit(true);
		logtxt.print( "Unexpected exception in App::App()" );
    }
}

void App::InitSound() {

    try {

        snd = new Sounds();
        snd->init();

        logtxt.print("Sound initialized");
    }
	catch ( Error& err ) {
		std::cerr << (err.getDesc() );
		setQuit(true);
		logtxt.print( err.getDesc() );
	}
    catch (...) {
        std::cerr << "Unexpected exception";
		setQuit(true);
		logtxt.print( "Unexpected exception in App::InitSound()" );
    }
}

App::App()
	:	quit(false),
        screen(NULL),
        buf(NULL),
        snd(NULL)

{
}

App::~App(void)
{
    try {

        if (screen) {
            SDL_FreeSurface(screen);
            screen = NULL;
        }
        if (buf) {
            SDL_FreeSurface(buf);
            buf = NULL;
        }

        SDL_Quit();

        if ( TTF_WasInit() )
            TTF_Quit();

        if ( snd ) {
            delete snd;
            snd = NULL;
        }
    }
    catch ( Error& err ) {
		std::cerr << (err.getDesc() );
		setQuit(true);
		logtxt.print( err.getDesc() );
	}
    catch (...) {
        std::cerr << "Unexpected exception";
		setQuit(true);
		logtxt.print( "Unexpected exception in App::~App()" );
    }
}

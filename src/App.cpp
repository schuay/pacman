/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


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

        screen->Create(sf::VideoMode(settings.width, settings.height+EXTRA_Y_SPACE), "pacman_sdl");

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
        screen.reset(new sf::RenderWindow());
        screen->UseVerticalSync(true);
        logtxt.print("SFML initialized");
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

        snd.reset(new Sounds());
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
:   quit(false)
{
}

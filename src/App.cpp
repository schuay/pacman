/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "App.h"

extern Settings settings;

void App::initWindow() {
    settings.height=settings.fieldheight*settings.tilesize;
    settings.width=settings.fieldwidth*settings.tilesize;

    window->Create(sf::VideoMode(settings.width, settings.height+EXTRA_Y_SPACE), "pacman_sdl");

    Logger::Msg("Video mode set successfully");
}

void App::initApp() {
    window.reset(new sf::RenderWindow());
    window->UseVerticalSync(true);
    Logger::Msg("SFML initialized");
}

void App::initSound() {
    snd.reset(new Sounds());
    snd->init();

    Logger::Msg("Sound initialized");
}

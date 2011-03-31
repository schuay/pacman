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

    Logger::msg("video mode set successfully");
}

void App::initApp() {
    window.reset(new sf::RenderWindow());
    window->UseVerticalSync(true);
    Logger::msg("SFML initialized");
}

void App::initSound() {
    snd.reset(new Sounds());
    snd->init();

    Logger::msg("sound initialized");
}

App::App() : quit(false) {
    searchpaths.push_back(".");
    searchpaths.push_back(string(getenv("HOME")) + "/.config/pacman_sdl");
    searchpaths.push_back("/usr/share/pacman_sdl");
}

string App::findFile(string filename) {
    struct stat fileInfo;
    for(int i = 0; i < searchpaths.size(); i++) {
        string path = searchpaths[i] + "/" + filename;
        if (stat(path.c_str(), &fileInfo) == 0) {
            Logger::msg("found file at " + path);
            return path;
        }
    }
    throw new Error("file not found: " + filename);
}

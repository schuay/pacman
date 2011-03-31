/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#pragma once

#include <boost/shared_ptr.hpp>
#include <sstream>

#include "Sounds.h"
#include "BckgrObj.h"
#include "Object.h"
#include "Pacman.h"
#include "Ghost.h"
#include "hScore.h"
#include "Defines.h"

using boost::shared_ptr;

#define NUMOFOBJECTS 6
#define SETTINGSFILE "settings.conf"
#define CFGFILE "level.conf"
#define SMALL_DOTS_SCORE 10
#define LARGE_DOTS_SCORE 50
#define OBJFILE "objmap"
#define MAPFILE "map"
#define FRUITDURATION 10000
#define BOOSTTIME 3000

class Sounds;

enum GameStateEnum {
    STATE_GAME,
    STATE_STOPPED,
    STATE_ENTER_HSCORE,
    STATE_VIEW_HSCORE,
};

enum KeyEnum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    ENTER,
    SPACE
};

class Game
{
public:
    Game();
    ~Game();

    bool loadFont();
    bool loadSprites();
    bool loadMap(std::string file, int* memmap);

    void getMaps( int **m, int **o);
    void render();
    void renderNormal();
    void renderEnterHscore();
    void renderViewHscore();

    std::string getFPS();

    void processInput(KeyEnum k);
    void processLogic();
    void logicGame();
    void logicEnterHscore();

    void resetLvl();
    void nextLvl();
    void gameInit(std::string level="", std::string skin="");

    bool pause();
    void boost();

    GameStateEnum  getState() const { return state; }
    void setState(GameStateEnum st);

    void toggleFps() { showfps = !showfps; }

    void clearHscore();

    bool isinit;

    void toggleSound();
    void emptyMsgPump();
    void changeSkin();

    void prepareShutdown();

private:

    GameStateEnum
            state;

    int
            counter;

    sf::Clock clock;

    std::string
            fps;

    sf::Vector2f
            scorebox,
            floatingscorebox;

    unsigned int
            ticks;

    sf::Font
            font;
    sf::String
            str;

    Object
            *objects[NUMOFOBJECTS];

    ///////////////////////
    // GAME OBJECTS
    //////////////////////

    KeyEnum
            key;

    int
            *map,
            *objmap,
            score,
            deadghostcount,	//used to multiply score for eating ghosts
            lives,
            objscore,
            floatingX,
            floatingY,
            floatingscore,
            floatingscorecounter,
            specialspawntime,
            level,
            namecol[3],
            hscoreselection;

    unsigned int
            soundcounter,
            time,
            oldtime,
            ghosttick,
            fruittick,
            pausetick,
            boosttick;

    bool
            inputwaiting,
            gamestarted,
            vulnflag,
            specialspawned,
            specialeaten,
            specialhasbeenspawned,
            ispaused,
            isboosted,
            boostavailable,
            levelcleared,
            showfps,
            renderisbusy;
    std::string
            num[10],
            name;

    hScore
            hscore;

    Sounds
            *sounds;

};

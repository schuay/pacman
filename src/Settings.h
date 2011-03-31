/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "App.h"
#include "Error.h"
#include "Logger.h"
#include "Defines.h"

using std::string;

#define MODE_LEVELS 2
#define MODE_SKINS 1

class Settings {
public:
    Settings();

    bool loadSettings(string filename);

    //searches for str in level/skinspaths; if successful, sets currently selected path.
    //returns 0 on success, 1 on failure
    int setPath(int mode, string str);

    //////////////////////////////
    // VARIABLES	- APP
    //////////////////////////////
    int
            width,
            height;

    //////////////////////////////
    // VARIABLES	- GAME
    //////////////////////////////

    int
            fieldwidth,
            fieldheight,
            tilesize,
            gatex,
            gatey,
            pacstartx,
            pacstarty,
            pacspeed,
            ghoststartx,
            ghoststarty,
            ghostspeed,
            ghostiq,
            vuln_duration,

            lvlpathcount,
            lvlpathcurrent,
            skinspathcount,
            skinspathcurrent;

    std::vector<string>
            lvlpath,
            skinspath;
};

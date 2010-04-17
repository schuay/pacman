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
#include "Main.h"

class Settings {
public:
    Settings();
    ~Settings();

    bool LoadSettings(std::string filename);

    //searches for str in level/skinspaths; if successful, sets currently selected path.
    //returns 0 on success, 1 on failure
    int setPath(int mode,std::string str);

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
            baddiestartx,
            baddiestarty,
            baddiespeed,
            baddieiq,
            vuln_duration,

            lvlpathcount,
            lvlpathcurrent,
            skinspathcount,
            skinspathcurrent;

    std::vector<std::string>
            lvlpath,
            skinspath;
};

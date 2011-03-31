/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "Settings.h"

extern App app;

int Settings::setPath(int mode,std::string str) {
    int i;

    switch (mode) {
    case MODE_LEVELS:
        for (i=0;i<lvlpathcount;i++) {
            if (lvlpath[i]=="./levels/" + str + "/") {
                lvlpathcurrent=i;
                return 0;
            }
        }
        break;
    case MODE_SKINS:
        for (i=0;i<skinspathcount;i++) {
            if (skinspath[i]=="./skins/" + str + "/") {
                skinspathcurrent=i;
                return 0;
            }
        }
        break;
    default:
        break;
    }

    return 1;
}

bool Settings::loadSettings(std::string filename) {

    filename = app.findFile(filename);

    std::ifstream	file( filename.c_str() );
    std::string		buffer,
    tmpstr;
    char			c=';' ;
    int				pos;

    if (!file) {
        Logger::err("Could not open settings file");
        return false;
    }

    while (file)	{
        do {
            if ( file.eof() ) break;
            c=file.get();
        } while (c < 'A' || c > 'z');

        pos= file.tellg();
        pos--;

        if (pos<0) pos=0;

        file.seekg(pos);

        getline(file, buffer, '=');
        if (! file.eof() ) {
            if (buffer == "FIELDWIDTH") file >> fieldwidth;
            else if (buffer == "FIELDHEIGHT") file >> fieldheight;
            else if (buffer == "TILESIZE") file >> tilesize;
            else if (buffer == "PACSTARTX") file >> pacstartx;
            else if (buffer == "PACSTARTY") file >> pacstarty;
            else if (buffer == "PACSPEED") file >> pacspeed;
            else if (buffer == "GHOSTSTARTX") file >> ghoststartx;
            else if (buffer == "GHOSTSTARTY") file >> ghoststarty;
            else if (buffer == "GHOSTSPEED") file >> ghostspeed;
            else if (buffer == "GHOSTIQ") file >> ghostiq;
            else if (buffer == "VULN_DURATION") file >> vuln_duration;
            else if (buffer == "GATEX") file >> gatex;
            else if (buffer == "GATEY") file >> gatey;
            else if (buffer == "LEVEL_PATH") {
                getline(file, tmpstr, ';');
                lvlpath.push_back("./levels/" + tmpstr + "/");
                lvlpathcount++;
            }
            else if (buffer == "SKINS_PATH") {
                getline(file, tmpstr, ';');
                skinspath.push_back("./skins/" + tmpstr + "/");
                skinspathcount++;
            }
        }
    }

    file.close();

    Logger::msg(filename + " loaded");

    return true;
}

Settings::Settings() {
    width = 640;
    height = 480;

    lvlpathcount = 0;
    lvlpathcurrent = 0;
    skinspathcount = 0;
    skinspathcurrent = 0;

    gatex = 0;
    gatey = 0;
    fieldwidth = 0;
    fieldheight = 0;
    tilesize = 0;
    pacstartx = 0;
    pacstarty = 0;
    pacspeed = 0;
    ghoststartx = 0;
    ghoststarty = 0;
    ghostspeed = 0;
    ghostiq = 0;
    vuln_duration = 0;
}

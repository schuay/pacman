/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "Main.h"

//////////////////////////////////////////////////////
//	GLOBAL VARS
//////////////////////////////////////////////////////

App		app;
Game	game;
Settings settings;

//////////////////////////////////////////////////////
//	MAIN
//////////////////////////////////////////////////////

int main( int argc, char** argv ) {
    std::string str="",level="",skin="";

    for (int i = 1;i<argc;i++) {
        str=argv[i];
        if (str=="--help") {
            std::cout << "pacman usage:\n\ncommandline arguments\n--help:\t\tshow this message\n"
                    << "--level <lvl>:\tstart with selected level\n--skin <skin>:\tstart with selected skin\n"
                    << "ingame\nesc/q:\tquit\narrows:\tmovement\nspace:\tboost\n"
                    << "p:\ttoggle pause\nn:\tnew game\nl:\tswitch level\ns:\tswitch skin\n"
                    << "f:\ttoggle fps display\nh:\tview highscore\n";
            return 0;
        }
        else if (str=="--level") {
            if (++i<argc) {
                str=argv[i];
                if (str[0]=='-') {
                    std::cerr << "no level name given. exiting...\n";
                    return 1;
                }
                else
                    level=str;
            }
            else {
                std::cerr << "no level name given. exiting...\n";
                return 1;
            }
        }
        else  if (str=="--skin")
            if (++i<argc) {
            str=argv[i];
            if (str[0]=='-') {
                std::cerr << "no skin name given. exiting...\n";
                return 1;
            }
            else
                skin=str;
        }
        else {
            std::cerr << "no skin name given. exiting...\n";
            return 1;
        }
        else
            std::cerr << "unrecognized commandline option\n";
    }

    srand( (unsigned int)time(NULL) );

    //init settings
    if ( !app.getQuit() ) settings.LoadSettings(SETTINGSFILE);
    if ( !app.getQuit() ) settings.LoadSettings( (settings.lvlpath[settings.lvlpathcurrent] + CFGFILE) );

    if ( !app.getQuit() ) app.InitApp();
    if ( !app.getQuit() ) app.InitWindow();
    if ( !app.getQuit() ) app.InitSound();

    //init game class
    if ( !app.getQuit() ) game.gameInit(level,skin);


    //main loop
    while ( ! app.getQuit() ) {

        game.emptyMsgPump();

        if ( ! app.getQuit() )
            game.processLogic();

        if ( ! app.getQuit() )
            game.render();
    }

    Logger::Msg("shutdown");

    return 0;
}

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "Game.h"

#define PAC	1
#define GHOST1 2
#define GHOST2 3
#define GHOST3 4
#define GHOST4 5

extern App app;
extern Settings settings;

void Game::changeSkin() {
    for (int i = 0; i < NUMOFOBJECTS; i++) {
        objects[i]->loadTextures(app.findFile(settings.skinspath[settings.skinspathcurrent]));
    }
}
void Game::emptyMsgPump() {

    sf::Event ev;
    shared_ptr<sf::RenderWindow> buf = app.getWindow();

    while ( buf->GetEvent(ev) ) {
        switch(ev.Type) {
        case sf::Event::KeyPressed:
            switch (ev.Key.Code ) {
            case sf::Key::Escape:
            case sf::Key::Q:
                app.setQuit(true);
                break;
            case sf::Key::Up:
                processInput(UP);
                ((Pacman*)objects[1])->setNextDir( UP );
                break;
            case sf::Key::Down:
                processInput(DOWN);
                ((Pacman*)objects[1])->setNextDir( DOWN );
                break;
            case sf::Key::Left:
                processInput(LEFT);
                ((Pacman*)objects[1])->setNextDir( LEFT );
                break;
            case sf::Key::Right:
                processInput(RIGHT);
                ((Pacman*)objects[1])->setNextDir( RIGHT );
                break;
            case sf::Key::Space:
                boost();
                break;
            case sf::Key::P:
                if ( getState() == STATE_GAME )
                    pause();
                break;
            case sf::Key::N:
                if ( getState() != STATE_ENTER_HSCORE )
                    gameInit();
                break;
            case sf::Key::L:
                settings.lvlpathcurrent++;
                if ( settings.lvlpathcurrent >= settings.lvlpathcount)
                    settings.lvlpathcurrent=0;
                gameInit();
                break;
            case sf::Key::S:
                settings.skinspathcurrent++;
                if ( settings.skinspathcurrent >= settings.skinspathcount)
                    settings.skinspathcurrent=0;
                changeSkin();
                break;
            case sf::Key::F:
                toggleFps();
                break;
            case sf::Key::H:
                setState( STATE_VIEW_HSCORE );
                break;
            case sf::Key::Return:
                processInput(ENTER);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}
void Game::toggleSound() {

    if ( app.getSnd()->getOn()) {
        app.getSnd()->toggleSounds();
    } else {
        app.getSnd()->toggleSounds();

        if (gamestarted && !ispaused ) app.getSnd()->play(SIREN, true);
        if (gamestarted && vulnflag && !ispaused ) app.getSnd()->play(VULN, true);
    }
}
void Game::clearHscore() {
    hscore.clear();
}

void Game::renderViewHscore() {
    shared_ptr<sf::RenderWindow>
            buf = app.getWindow();
    std::ostringstream ostr, scstr;
    sf::Vector2f rect;
    int i, sc;
    std::string nm;

    rect.x = settings.fieldwidth*settings.tilesize / 2 - 200;
    rect.y = settings.fieldheight*settings.tilesize / 2 - 200;

    try {
        // DRAW FIELD + SPRITES

        for (i=0;i<NUMOFOBJECTS;i++) if ( objects[i] ) objects[i]->draw();

        // DRAW SCORE + INFO

        for (i=1; i<lives; i++) {
            objects[PAC]->draw( 350+i*50, settings.fieldheight*settings.tilesize);
        }
        if ( specialeaten ) objects[0]->draw( settings.fieldwidth*settings.tilesize - 40 -10, settings.fieldheight*settings.tilesize +10 );

        ostr << "level: " << level << " score: " << score;

        str.SetText(ostr.str());
        str.SetPosition(scorebox);
        buf->Draw(str);

        // DRAW HIGHSCORES

        str.SetText("HIGHSCORES:");
        str.SetPosition(rect);
        buf->Draw(str);

        for (i=0;i<MAXENTRIES;i++) {

            nm= "";
            nm=hscore.getName(i);
            sc=hscore.getScore(i);

            rect.x = settings.fieldwidth * settings.tilesize / 4;
            rect.y = 200 + i*50;

            if ( nm != "" ) {
                str.SetText(nm);
                str.SetPosition(rect);
                buf->Draw(str);
            }

            rect.x = settings.fieldwidth * settings.tilesize / 4 + 200;


            scstr.str("");
            if ( sc ) {
                scstr << sc;
                str.SetText(scstr.str());
                str.SetPosition(rect);
                buf->Draw(str);
            }
        }

    }
    catch ( Error& err ) {
        app.setQuit(true);
        Logger::err( err.getDesc() );
    }
    catch ( ... ) {
        app.setQuit(true);
        Logger::err( "Unexpected error" );
    }
}

void Game::setState(GameStateEnum st) {
    int i;

    if ( st == STATE_GAME ) {
        app.getSnd()->stop();
        app.getSnd()->play(SIREN, true);

        for (i=0;i<NUMOFOBJECTS;i++) if (objects[i]) objects[i]->setAlpha(255);
        for (i=0;i<NUMOFOBJECTS;i++) if ( objects[i] ) objects[i]->setPaused( false);

    }
    else if ( st == STATE_ENTER_HSCORE ) {

        app.getSnd()->stop();

        for (i=0;i<NUMOFOBJECTS;i++) if ( objects[i] ) objects[i]->setPaused( true);
        for (i=0;i<NUMOFOBJECTS;i++) if (objects[i]) objects[i]->setAlpha(150);

    }
    else if ( st == STATE_VIEW_HSCORE ) {


        app.getSnd()->stop();
        app.getSnd()->play(INTERMISSION, false);

        for (i=0;i<NUMOFOBJECTS;i++) if (objects[i]) objects[i]->setAlpha(150);
        for (i=0;i<NUMOFOBJECTS;i++) if ( objects[i] ) objects[i]->setPaused( true);
    }
    else if ( st == STATE_STOPPED && state != STATE_STOPPED) {
        app.getSnd()->stop();
        app.getSnd()->play(INTRO, true);
        for (i=0;i<NUMOFOBJECTS;i++) if ( objects[i] ) objects[i]->setPaused( true);
        for (i=0;i<NUMOFOBJECTS;i++) if (objects[i]) objects[i]->setAlpha(255);
    }

    state = st;

}

void Game::logicEnterHscore() {
    int j;

    if ( inputwaiting ) {

        switch ( key ) {
        case RIGHT :
            if (hscoreselection==2) hscoreselection=0;
            else hscoreselection++;
            for (j=0;j<3;j++) namecol[j]= 150;
            namecol[ hscoreselection ] = 255;
            break;
        case LEFT :
            if (hscoreselection==0) hscoreselection=2;
            else hscoreselection--;
            for (j=0;j<3;j++) namecol[j]= 150;
            namecol[ hscoreselection ] = 255;
            break;
        case DOWN :
            if (name[hscoreselection]=='A') name[hscoreselection]='_';
            else if (name[hscoreselection]=='_') name[hscoreselection]='Z';
            else name[hscoreselection]--;
            break;
        case UP :
            if (name[hscoreselection]=='Z') name[hscoreselection]='_';
            else if (name[hscoreselection]=='_') name[hscoreselection]='A';
            else name[hscoreselection]++;
            break;
        case ENTER :
            hscore.add(name, score);
            hscore.save();

            setState( STATE_VIEW_HSCORE) ;
        default :
             break;
        }
    }
}
void Game::logicGame() {
    int
            i, j,
            delta,
            pacX,
            pacY,
            pacXpix,
            pacYpix,
            ghostXpix[4],
            ghostYpix[4],
            oldscore(score);

    delta = (int)(time -oldtime);

    if ( gamestarted && !ispaused ) {

        //////////////////////////////////
        //	GET POSITIONS
        //////////////////////////////////

        pacXpix = ((Pacman*)objects[PAC])->getXpix();
        pacYpix = ((Pacman*)objects[PAC])->getYpix();
        pacX= (pacXpix + settings.tilesize / 2) / settings.tilesize;
        pacY= (pacYpix + settings.tilesize / 2) / settings.tilesize;
        for (i=0;i<4;i++) {
            ghostXpix[i]= ((Ghost*)objects[i+2])->getXpix();
            ghostYpix[i]= ((Ghost*)objects[i+2])->getYpix();
        }

        //////////////////////////////////
        // MOVE LOGIC
        //////////////////////////////////

        if ( delta < 100 )  {
            //set ghost targets - 2 on pacman, 2 on next intersection

            {
                int tmpx(  pacXpix / settings.tilesize),
                tmpy(  pacYpix / settings.tilesize);

                ((Ghost*)objects[GHOST1])->setTarget( tmpx, tmpy);
                ((Ghost*)objects[GHOST2])->setTarget( tmpx, tmpy);

                ((Pacman*)objects[PAC])->nextIntersection ( tmpx, tmpy);

                ((Ghost*)objects[GHOST3])->setTarget( tmpx, tmpy);
                ((Ghost*)objects[GHOST4])->setTarget( tmpx, tmpy);
            }

            //move objects

            for (i=0;i<NUMOFOBJECTS;i++)
                objects[i]->update( delta );
        }
        ///////////////////////////////////////////
        //	TIME LOGIC
        ///////////////////////////////////////////

        //if ghost timer is up, set ghosts to normal mode

        if ( vulnflag && time > ghosttick ) {
            for (i=0;i<4;i++) {
                if ( ((Ghost*)objects[i+2])->getState() != 3) ((Ghost*)objects[i+2])->setState(0);
            }
            vulnflag= false;
            deadghostcount= 0;

            app.getSnd()->stop(VULN);
        }

        // if less than 2 secs left in vuln mode, set warning mode

        else if ( vulnflag && time > ghosttick -2000) for (i=0;i<4;i++) {
            ((Ghost*)objects[i+2])->setState(2);
            app.getSnd()->modify(VULN, 1.f + (2000 - (ghosttick-time)) / 4400.f );
        }

        // fruit stuff

        if ( specialspawned && !specialeaten && (fruittick - time < 2000) ) {
            ((BckgrObj*)objects[0])->setFruitAlpha( 55 + (fruittick - time) / 10 );
        }
        if ( time > fruittick && specialspawned && !specialeaten) {
            specialspawned = false;
            ((BckgrObj*)objects[0])->setSpecialSpawned(false);
        }

        // pac booster

        if ( isboosted && time > boosttick ) {
            isboosted = false;
            ((Pacman*)objects[PAC])->setSpeedMult( 1 );
        }

        ///////////////////////////////////////////
        // PACMAN LOCATION LOGIC
        ///////////////////////////////////////////

        for (i=0; i< 4; i++) {
            //if (pacX == ghostX[i] && pacY == ghostY[i] ) {
            if (	pacXpix > ghostXpix[i] - 10  &&
                        pacXpix < ghostXpix[i] + 10  &&
                        pacYpix > ghostYpix[i] - 10  &&
                        pacYpix < ghostYpix[i] + 10  ) {

                if ( ((Ghost*)objects[i+2])->getState() == 0 )	{

                    lives--;

                    app.getSnd()->stop();
                    app.getSnd()->play(DEATH, false);

                    sf::Sleep(1.f);

                    if ( lives == 0) {
                        if (hscore.onlist(score) ) setState( STATE_ENTER_HSCORE );
                        else gameInit();
                        return;
                    }
                    else {
                        resetLvl();
                        return;
                    }
                }

                else if  ( ((Ghost*)objects[i+2])->getState() != 3 ) {
                    ((Ghost*)objects[i+2])->setState(3);

                    app.getSnd()->play(GHOST_EAT, false);
                    deadghostcount++;

                    floatingscore = 200;
                    for (j=1; j< deadghostcount; j++) floatingscore *= 2;
                    floatingscorebox.x = pacXpix;
                    floatingscorebox.y = pacYpix-70;

                    floatingscorecounter = 100;

                    score+= floatingscore;
                }
            }
        }

        if ( objmap[pacY * settings.fieldwidth + pacX ] == 1 ) {
            objmap[pacY * settings.fieldwidth + pacX ] = 0;
            score += SMALL_DOTS_SCORE;

            SoundsEnum s = (soundcounter % 2) == 0 ? MUNCH_A : MUNCH_B;
            app.getSnd()->play(s, false);
            soundcounter++;
        }
        else if ( objmap[pacY * settings.fieldwidth + pacX ] == 2 ) {
            objmap[pacY * settings.fieldwidth + pacX ] = 0;
            score += LARGE_DOTS_SCORE;

            app.getSnd()->play(LARGE_PELLET, false);
            app.getSnd()->play(VULN, true);

            //set ghosts to vulnerable mode

            for (i=2;i<6;i++) ((Ghost*)objects[i])->setState(1);
            deadghostcount=0;	//reset dead counter

            //start ghost timer

            ghosttick = time + settings.vuln_duration;
            vulnflag = true;
        }
        else if ( objmap[pacY * settings.fieldwidth + pacX ] == 3 &&  specialspawned && !specialeaten ) {
            objmap[pacY * settings.fieldwidth + pacX ] = 0;
            score += objscore;

            app.getSnd()->play(FRUIT, false);

            floatingscorebox.x = pacXpix;
            floatingscorebox.y = pacYpix-70;
            floatingscore = objscore;
            floatingscorecounter = 100;

            specialeaten = true;
            ((BckgrObj*)objects[0])->setSpecialEaten(true);
        }


        ///////////////////////////////////
        //	OTHER
        ///////////////////////////////////

        if ( (score >= 10000 && oldscore < 10000) ||
             (score >=100000 && oldscore < 100000) ||
             (score >=1000000 && oldscore < 1000000) ) {
            lives++;

            app.getSnd()->play(EXTRA_MAN, false);
        }

        if ( !specialhasbeenspawned && ((BckgrObj*)objects[0])->getObjCount() == specialspawntime) {
            specialspawned = true;
            ((BckgrObj*)objects[0])->setFruitAlpha(255);
            ((BckgrObj*)objects[0])->setSpecialSpawned(true);
            fruittick = (int)(clock.GetElapsedTime() * 1000.f) + FRUITDURATION;
        }

        if ( ((BckgrObj*)objects[0])->getObjCount() == 0 ) {
            levelcleared = true;
            render();
            nextLvl();
        }
        else ( app.getSnd()->modify(SIREN, 1.f - ((BckgrObj*)objects[0])->getObjCount() / 500.f) );
    }
}
void Game::renderEnterHscore() {
    shared_ptr<sf::RenderWindow>
            buf = app.getWindow();
    std::ostringstream ostr;
    sf::Vector2f rect;
    sf::Color col(255,255,255);
    int i;
    std::string tmp;

    col.r = col.g = col.b = 255;

    rect.x = settings.fieldwidth*settings.tilesize / 2 - 200;
    rect.y = settings.fieldheight*settings.tilesize / 2 - 50;

    try {
        // DRAW FIELD + SPRITES

        for (i=0;i<NUMOFOBJECTS;i++) if ( objects[i] ) objects[i]->draw();

        // DRAW SCORE + INFO

        for (i=1; i<lives; i++) {
            objects[PAC]->draw( 350+i*50, settings.fieldheight*settings.tilesize+5);
        }
        if ( specialeaten ) objects[0]->draw( settings.fieldwidth*settings.tilesize - 40 -10, settings.fieldheight*settings.tilesize +15 );

        ostr << "level: " << level << " score: " << score;

        str.SetText(ostr.str());
        str.SetPosition(scorebox);
        buf->Draw(str);

        // DRAW HIGHSCORE ENTRY

        str.SetText("NEW HIGHSCORE!");
        str.SetPosition(rect);
        buf->Draw(str);

        rect.y += 50;


        str.SetText("Enter name:");
        str.SetPosition(rect);
        buf->Draw(str);

        rect.y += 70;
        rect.x = settings.fieldwidth*settings.tilesize / 2 - 60;

        for (i=0;i<3;i++) {
            tmp=name[i];
            col.r=col.g=col.b=namecol[i];

            str.SetText(tmp);
            str.SetPosition(rect);
            buf->Draw(str);

            rect.x=rect.x+40;
        }
    }
    catch ( Error& err ) {
        app.setQuit(true);
        Logger::err( err.getDesc() );
    }
    catch ( ... ) {
        app.setQuit(true);
        Logger::err( "Unexpected error" );
    }
}
void Game::renderNormal() {
    int i;
    std::ostringstream ostr;
    shared_ptr<sf::RenderWindow> buf = app.getWindow();


    try {
        // DRAW FIELD + SPRITES

        for (i=0;i<NUMOFOBJECTS;i++) if ( objects[i] ) objects[i]->draw();

        // DRAW SCORE + INFO

        for (i=1; i<lives; i++)
            objects[PAC]->draw( 350+i*50, settings.fieldheight*settings.tilesize+5);

        if ( specialeaten ) objects[0]->draw( settings.fieldwidth*settings.tilesize - 40 -10, settings.fieldheight*settings.tilesize +15 );
        if ( boostavailable ) objects[0]->draw( settings.fieldwidth*settings.tilesize - 60 -10, settings.fieldheight*settings.tilesize +15, 4 );

        ostr << "level: " << level << " score: " << score;

        str.SetText(ostr.str());
        str.SetPosition(scorebox);
        buf->Draw(str);

        //DRAW SCORE POPUP
        if (floatingscorecounter != 0) {
            std::ostringstream scoretext;

            scoretext << floatingscore;
            floatingscorecounter--;

            floatingscorebox.y--;

            str.SetText(scoretext.str());
            str.SetPosition(floatingscorebox);
            str.SetColor(sf::Color(255, 255, 255, 55+floatingscorecounter*2));
            buf->Draw(str);
            str.SetColor(sf::Color(255, 255, 255, 255));
        }

        // PAUSE

        if ( ispaused ) {
            sf::Vector2f pauserect;
            pauserect.y = settings.fieldwidth*settings.tilesize / 2 - 100;
            pauserect.x = settings.fieldheight*settings.tilesize / 2 - 10;

            str.SetText("PAUSED");
            str.SetPosition(pauserect);
            buf->Draw(str);
        }

        // LEVEL CLEARED

        else if ( levelcleared ) {
            sf::Vector2f pauserect;
            pauserect.x = settings.fieldwidth*settings.tilesize / 2 - 200;
            pauserect.y = settings.fieldheight*settings.tilesize / 2 - 10;

            str.SetText("LEVEL CLEARED!");
            str.SetPosition(pauserect);
            buf->Draw(str);
        }
    }

    catch ( Error& err ) {
        app.setQuit(true);
        Logger::err( err.getDesc() );
    }
    catch ( ... ) {
        app.setQuit(true);
        Logger::err( "Unexpected error during RenderNormal()" );
    }
}

void Game::boost() {
    if ( boostavailable ) {

        app.getSnd()->play(BOOSTER, false);

        boosttick = (int)(clock.GetElapsedTime() * 1000.f) + BOOSTTIME;
        isboosted = true;
        boostavailable = false;
        ((Pacman*)objects[PAC])->setSpeedMult( 2 );
    }
}
bool Game::pause() {
    int i;

    if ( !ispaused ) {
        ispaused = true;
        app.getSnd()->stop();
        for (i=0;i<NUMOFOBJECTS;i++) objects[i]->setPaused( true);

        pausetick = (int)(clock.GetElapsedTime() * 1000.f);

        return ispaused;
    }
    else {
        ispaused = false;
        app.getSnd()->play(SIREN, true);
        if (vulnflag) app.getSnd()->play(VULN, true);
        for (i=0;i<NUMOFOBJECTS;i++) objects[i]->setPaused( false);

        int delta( (int)(clock.GetElapsedTime() * 1000.f) - pausetick );

        ghosttick += delta;
        fruittick += delta;
        boosttick += delta;

        return ispaused;
    }
}
void Game::nextLvl() {
    std::string tmpstr;


    try {
        level++;

        app.getSnd()->stop();
        app.getSnd()->play(NEWGAME, false);

        objscore*=2;
        settings.vuln_duration -= settings.vuln_duration/10;

        ((Ghost*)objects[ rand()%4 +2])->changeDifficulty( rand()%15, rand()%5 );
        ((Ghost*)objects[ rand()%4 +2])->changeDifficulty( rand()%10, rand()%3 );

        objects[PAC]->reset(settings.pacstartx, settings.pacstarty);
        objects[GHOST1]->reset(settings.ghoststartx, settings.ghoststarty);
        objects[GHOST2]->reset(settings.ghoststartx+2, settings.ghoststarty);
        objects[GHOST3]->reset(settings.ghoststartx-2, settings.ghoststarty);
        objects[GHOST4]->reset(settings.ghoststartx, settings.ghoststarty-2);

        tmpstr = settings.lvlpath[settings.lvlpathcurrent] + OBJFILE;
        if ( ! loadMap(tmpstr, objmap) )
            throw Error("Error loading objmap.txt during Game::nextLvl()");

        vulnflag= false;
        deadghostcount= 0;
        floatingscorecounter = 0;
        floatingscore = 0;
        specialspawned = false;
        ((BckgrObj*)objects[0])->setSpecialSpawned(false);
        specialeaten = false;
        ((BckgrObj*)objects[0])->setSpecialEaten(false);
        specialhasbeenspawned = false;
        time = oldtime = (int)(clock.GetElapsedTime() * 1000.f);
        ghosttick = 0;
        isboosted = false;
        ((Pacman*)objects[PAC])->setSpeedMult( 1);
        boostavailable = true;
        levelcleared = false;

        if (ispaused) pause();


        sf::Sleep(1.f);


        render();
        specialspawntime = rand() % (((BckgrObj*)objects[0])->getObjCount() -20) + 10;

        emptyMsgPump();
        inputwaiting = false;
        gamestarted = false;
    }
    catch ( Error &err) {
        app.setQuit(true);
        Logger::err( err.getDesc() );
    }
    catch ( ... ) {
        app.setQuit(true);
        Logger::err( "Unexpected error" );
    }
}

void Game::gameInit(std::string level, std::string skin) {
    int i;
    std::string tmpstr;

    try {
        isinit=false;
        app.getSnd()->stop();

        //try to set level/skins path
        settings.setPath(MODE_LEVELS,level);
        settings.setPath(MODE_SKINS,skin);

        tmpstr = settings.lvlpath[settings.lvlpathcurrent] + CFGFILE;

        if ( !settings.loadSettings(tmpstr) )
            throw Error("Error loading level settings");

        //resetting variables
        score=0;
        vulnflag= false;
        deadghostcount= 0;
        ghosttick = 0;
        floatingscorecounter = 0;
        floatingscore = 0;
        lives = 3;
        level = 1;
        objscore = 200;
        specialspawned = false;
        specialeaten = false;
        specialhasbeenspawned = false;
        inputwaiting = false;
        gamestarted = false;
        isboosted = false;
        time = oldtime = (int)(clock.GetElapsedTime() * 1000.f);
        boostavailable = true;
        levelcleared = false;
        setState( STATE_GAME);
        name = "AAA";
        namecol[0] = 255;
        namecol[1] = 150;
        namecol[2] = 150;
        hscoreselection = 0;


        if ( ispaused )
            pause();

        scorebox.x = 20;
        scorebox.y = settings.fieldheight * settings.tilesize;



        //DYNAMIC OBJECTS INIT
        if (map!=NULL) {
            delete[] map;
            map=NULL;
        }
        if (objmap!=NULL) {
            delete[] objmap;
            objmap=NULL;
        }
        for (i=0; i< NUMOFOBJECTS; i++) {
            if (objects[i]!=NULL) {
                delete objects[i];
                objects[i] = NULL;
            }
        }

        Logger::msg("Unloading complete");

        //if level has different field size than currently selected, setup new window with proper size
        if (settings.fieldwidth*settings.tilesize != app.getWindow()->GetWidth()
            || settings.fieldheight*settings.tilesize+EXTRA_Y_SPACE != app.getWindow()->GetHeight()) {
            app.initWindow();
            Logger::msg("window resized...");
        }

        // INIT MAPS
        map = new int[settings.fieldheight*settings.fieldwidth];
        objmap = new int[settings.fieldheight*settings.fieldwidth];

        tmpstr = settings.lvlpath[settings.lvlpathcurrent];

        if ( !loadMap(tmpstr + MAPFILE, map) )
            throw Error("Failed to load map.txt");
        if ( !loadMap(tmpstr + OBJFILE, objmap) )
            throw Error("Failed to load objmap.txt");

        Logger::msg("Maps loaded");

        //creating font

        loadFont();

        Logger::msg("Font created");

        //loading level graphics

        objects[0] = new BckgrObj( app.getWindow(), 10 );
        objects[0]->loadTextures(app.findFile(settings.skinspath[settings.skinspathcurrent]));

        Logger::msg("Level background loaded");

        Logger::msg("Sounds loaded");

        app.getSnd()->play(NEWGAME, false);

        setState( STATE_STOPPED);

        hscore.setfilename(settings.lvlpath[settings.lvlpathcurrent] + "hscore");
        hscore.load();


        //create pacman + ghosts

        objects[1] = new Pacman( app.getWindow(),
                            20,
                            settings.pacstartx,
                            settings.pacstarty,
                            settings.pacspeed,
                            settings.tilesize,
                            settings.fieldheight,
                            settings.fieldwidth,
                            map);
        objects[1]->loadTextures(app.findFile(settings.skinspath[settings.skinspathcurrent]));

        objects[2] = new Ghost( app.getWindow(),
                            20,
                            settings.ghoststartx,
                            settings.ghoststarty,
                            settings.ghostspeed + rand()%20 + 10,
                            settings.tilesize,
                            settings.fieldheight,
                            settings.fieldwidth,
                            map,
                            "1");
        objects[2]->loadTextures(app.findFile(settings.skinspath[settings.skinspathcurrent]));

        objects[3] = new Ghost( app.getWindow(),
                            20,
                            settings.ghoststartx+2,
                            settings.ghoststarty,
                            settings.ghostspeed + rand()%20-10,
                            settings.tilesize,
                            settings.fieldheight,
                            settings.fieldwidth,
                            map,
                            "2");
        objects[3]->loadTextures(app.findFile(settings.skinspath[settings.skinspathcurrent]));

        objects[4] = new Ghost( app.getWindow(),
                            20,
                            settings.ghoststartx-2,
                            settings.ghoststarty,
                            settings.ghostspeed + rand()%20-10,
                            settings.tilesize,
                            settings.fieldheight,
                            settings.fieldwidth,
                            map,
                            "3");
        objects[4]->loadTextures(app.findFile(settings.skinspath[settings.skinspathcurrent]));

        objects[5] = new Ghost( app.getWindow(),
                            20,
                            settings.ghoststartx,
                            settings.ghoststarty-2,
                            settings.ghostspeed + rand()%20 - 10,
                            settings.tilesize,
                            settings.fieldheight,
                            settings.fieldwidth,
                            map,
                            "4");
        objects[5]->loadTextures(app.findFile(settings.skinspath[settings.skinspathcurrent]));

        for (i=0;i<4;i++) ((Ghost*)objects[i+2])->changeDifficulty(0, settings.ghostiq);	//SET DIFFICULTY SPECIFIED IN CONFIG FILE

        Logger::msg("Objects loaded");

        //calculate special fruit spawn time

        render();
        specialspawntime = rand() % (((BckgrObj*)objects[0])->getObjCount() -20) + 10;

        emptyMsgPump();
        inputwaiting = false;
        gamestarted = false;

        isinit = true;
    }
    catch ( Error &err) {
        app.setQuit(true);
        Logger::err( err.getDesc() );
    }
    catch ( ... ) {
        app.setQuit(true);
        Logger::err( "Unexpected error" );
    }
}


void Game::resetLvl() {	// vars and positions when pacman dies during level

    app.getSnd()->stop();

    sf::Sleep(1.f);

    app.getSnd()->play(NEWGAME, false);


    // setting vars
    vulnflag= false;
    deadghostcount= 0;
    floatingscorecounter= 0;
    floatingscore= 0;
    ghosttick = 0;
    isboosted = false;
    ((Pacman*)objects[PAC])->setSpeedMult( 1);

    if (ispaused) pause();

    objects[PAC]->reset(settings.pacstartx, settings.pacstarty);
    objects[GHOST1]->reset(settings.ghoststartx, settings.ghoststarty);
    objects[GHOST2]->reset(settings.ghoststartx+2, settings.ghoststarty);
    objects[GHOST3]->reset(settings.ghoststartx-2, settings.ghoststarty);
    objects[GHOST4]->reset(settings.ghoststartx, settings.ghoststarty-2);

    render();

    emptyMsgPump();
    gamestarted= false;
    inputwaiting = false;
}
void Game::processInput(KeyEnum k) {

    inputwaiting = true;
    key = k;

    if ( !gamestarted && !ispaused ) {
        gamestarted = true;
        if ( state == STATE_STOPPED || state == STATE_GAME )
            setState( STATE_GAME );
    }
}

void Game::processLogic() {

    time = (int)(clock.GetElapsedTime() * 1000.f);

    if ( state == STATE_GAME ) logicGame();
    else if (state == STATE_ENTER_HSCORE ) logicEnterHscore();

    oldtime = time;
    inputwaiting = false;
}

void Game::getMaps( int **m, int **o) {
    (*m)=map;
    (*o)=objmap;
}
bool Game::loadMap(std::string file, int* memmap) {
    int i, count=0, size = settings.fieldheight * settings.fieldwidth;
    std::string tmp;
    char c('i');
    std::ifstream mp;

    file = app.findFile(file);
    mp.open( file.c_str() );

    if (!mp ) {
        Logger::err(file + " - Loading error");
        app.setQuit(true);
        return false;
    }

    for (i=0;i<size;i++) {
        do {
            if ( mp.eof() ) break;
            c=mp.get();
            if (c == '/' ) {	// enable comments in map file by prefixing line with  a slash
                getline(mp, tmp);
                continue;
            }
        }
        while (! (c >= '0' && c <= '9') ) ;
        memmap[i]=c-48;
        count++;
        if (!mp) break;
    }

    if ( mp.is_open() ) mp.close();

    if (count != size) {
        Logger::err(file + " - Loading error");
        app.setQuit(true);
        return false;
    }

    Logger::msg(file + " loaded");
    return true;
}


std::string Game::getFPS() {
    unsigned int newtick = (int)(clock.GetElapsedTime() * 1000.f);
    std::ostringstream ostr;
    float diff;

    diff = (float)(newtick-ticks) / (float)counter;	// ms per frame

    diff = 1000.0f / diff;			// frames per s

    ostr << diff << "fps";

    ticks = newtick;

    return ostr.str();
}

void Game::render() {
    shared_ptr<sf::RenderWindow>
            buf = app.getWindow();

    if (counter == 100) {
        fps = getFPS();
        counter = 0;
    }

    try {
        if ( !renderisbusy ) {
            renderisbusy = true;

            ////////////////////////////////
            // STATE SWITCH
            ////////////////////////////////

            if ( (state == STATE_GAME) || (state == STATE_STOPPED) ) renderNormal();
            else if (state == STATE_ENTER_HSCORE ) renderEnterHscore();
            else if (state == STATE_VIEW_HSCORE ) renderViewHscore();


            if ( showfps ) {
                str.SetText(fps);
                str.SetPosition(10, 10);
                buf->Draw(str);
            }

            buf->Display();

            renderisbusy = false;
            counter++;
        }
    }
    catch ( Error& err ) {
        app.setQuit(true);
        Logger::err( err.getDesc() );
    }
    catch ( ... ) {
        app.setQuit(true);
        Logger::err( "Unexpected error during Render()" );
    }
}

bool Game::loadFont() {

    try {
        if (!font.LoadFromFile(app.findFile("arial.ttf"),24)) {
            throw Error("Failed to create font object ");
        }
        str.SetFont(font);
    }
    catch ( Error& err ) {
        app.setQuit(true);
        Logger::err( err.getDesc() );
        return false;
    }
    catch ( ... ) {
        app.setQuit(true);
        Logger::err( "Unexpected error while loading font" );
        return false;
    }

    Logger::msg("Font loaded");
    return true;
}

void Game::prepareShutdown() {
    for (int i = 0; i < NUMOFOBJECTS; i++) {
        if (objects[i]) {
            delete objects[i];
        }
    }
}
Game::Game()
:   isinit(false),
    state(STATE_STOPPED),
    counter(0),
    map(NULL),
    objmap(NULL),
    deadghostcount(0),
    lives(2),
    objscore(200),
    floatingscore(0),
    floatingscorecounter(0),
    specialspawntime(0),
    level(1),
    hscoreselection(0),
    soundcounter(0),
    inputwaiting(false),
    gamestarted(false),
    vulnflag(false),
    specialspawned(false),
    specialeaten(false),
    specialhasbeenspawned(false),
    ispaused(false),
    isboosted(false),
    showfps(false),
    renderisbusy(false)

{
    int i;

    time = oldtime = ticks = (int)(clock.GetElapsedTime() * 1000.f);

    fps = "loading";

    for (i=0;i<NUMOFOBJECTS;i++) objects[i]=NULL;

    for (i=0;i<10;i++)
        num[i]='0'+i;

    name = "AAA";
    namecol[0] = 255;
    namecol[1] = 150;
    namecol[2] = 150;
}

Game::~Game()
{
    if ( map ) delete[] map;
    if ( objmap ) delete[] objmap;
}

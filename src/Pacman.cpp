/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "Pacman.h"

#define PACSIZE 40

extern App app;

void Pacman::setSpeedMult( int s) {
    spdmult = s;
}
void Pacman::draw(int ix, int iy, int obj, int type) {
    pacEl[3]->blit(buf, sf::Vector2i(ix, iy));
}
void Pacman::reset(int ix, int iy) {
    elapsedTime=0.f;
    x=ix;
    y=iy;
    xpix=ix*tilesize;
    ypix=iy*tilesize;
    xfloat= (float) xpix;
    yfloat= (float) ypix;
    dx=0;
    dy=0;
    nextdx=0;
    nextdy=0;
    paused=true;
}
void Pacman::nextIntersection(int &ix, int &iy) {
    int
            xtmp=x,
            ytmp=y,
            i=0;

    while (i< ( height+width) / 2 ) {
        if (xtmp == 0 && dx == -1) xtmp = width-2;
        else if (xtmp == width-2 && dx == 1) xtmp = 0;
        else if (ytmp == 0 && dy == -1) ytmp = height-2;
        else if (ytmp == height-2 && dy == 1) ytmp = 0;

        if ( ! collision(xtmp+dx, ytmp+dy) ) {
            xtmp+= dx;
            ytmp += dy;
        }
        if ( ! collision(xtmp + dy, ytmp + dx) || ! collision(xtmp - dy, ytmp - dx) ) break;
        i++;
    }

    ix=xtmp;
    iy=ytmp;
}

void Pacman::update(int time) {
    bool dirclear=false;
    int oldx, oldy;

    //screen wrappers
    if ( x == width-2 && dx == 1 ) {
        x = 0;
        xpix = 0;
        xfloat = 0;
    }
    else if ( xpix == 0 && dx == -1 ) {
        x = width-2;
        xpix = x*tilesize;
        xfloat = (float)xpix;
    }
    else if ( y == height-2 && dy == 1 ) {
        y = 0;
        ypix = 0;
        yfloat = 0;
    }
    else if ( ypix == 0 && dy == -1 ) {
        y = height-2;
        ypix = y*tilesize;
        yfloat = (float)ypix;
    }

    //remember old coords for adjustments at end
    oldx=xpix;
    oldy=ypix;


    //if turnaround, apply dirchange immediately
    if ( dx == -nextdx && dy == -nextdy && dx!=dy ) {
        dx=nextdx;
        dy=nextdy;
    }
    //if node is reached
    if ( xpix%tilesize == 0 && ypix%tilesize == 0 ) {
        if ( dx != nextdx || dy != nextdy ) {
            //if nextdir is clear, apply dirchange
            if ( (dirclear = (! collision(x+nextdx, y+nextdy))) ) {
                dx=nextdx;
                dy=nextdy;
            }
        }
    }

    //if nextdir not clear check current dir
    if ( ! dirclear ) {
        if ( dx >= 0 && dy >= 0)
            dirclear = ! collision(x+dx,y+dy);
        else if ( dx <= 0 && dy <= 0)
            dirclear = ! collision( (xpix+dx) / tilesize, (ypix+dy) / tilesize);
    }

    //MOVEMENT PART STARTS HERE

    if ( dirclear ) {
                //  dir    *       speed in percent
        xfloat += ( (float)(time * dx * spdmod * spdmult) / MOVEMOD );
        yfloat += ( (float)(time * dy * spdmod * spdmult) / MOVEMOD );
    }

    //COORD ADJUSTMENTS

    if ( xfloat > (float)(x+1)*tilesize ) xfloat = (float)(x+1)*tilesize;
    if ( yfloat > (float)(y+1)*tilesize ) yfloat = (float)(y+1)*tilesize;

    if ( xfloat < (float)(x)*tilesize && oldx > (float)(x)*tilesize )
        xfloat = (float)(x)*tilesize;
    if ( yfloat < (float)(y)*tilesize && oldy > (float)(y)*tilesize)
        yfloat = (float)(y)*tilesize;

    //COORD UPDATES

    xpix=(int)xfloat;
    ypix=(int)yfloat;

    x= xpix/tilesize;
    y=ypix/tilesize;

    elapsedTime += time;

}

int Pacman::getAnimationStep() const {
    /* get elapsed time down to a manageable level */
    int elapsed = elapsedTime / 25.f;
    /* modulo 15 because we have 7 animation steps */
    int step = elapsed % 15;
    /* subtract 7 and return abs because we are going back and forth between 0 and 7 */
    return 7 - abs(step - 7);
}

void Pacman::draw() {

    int animStep = getAnimationStep();

    shared_ptr<Sprite> s;
    if ((dx == 1 && dy == 0) || (dx == 0 && dy == 0)) {	//right or initial
        s = pacEl[animStep];
    } else if (dx == -1 && dy == 0) {	//left
        s = pacElRot[animStep][1];
    } else if (dx == 0 && dy == -1) {	//up
        s = pacElRot[animStep][2];
    } else if (dx == 0 && dy == 1) {	//down
        s = pacElRot[animStep][0];
    }

    s->setAlpha(alpha);
    s->blit(buf, sf::Vector2i(xpix, ypix));
}

bool Pacman::loadTextures(std::string path) {

    int i,j;
    std::string num[10];

    for (i=0;i<10;i++)
        num[i]='0'+i;

    try {
        for (i=0;i<NUMPACANIM;i++) {

            pacEl[i].reset(new Sprite());
            pacEl[i]->load(path + "pac" + num[i] + ".png");

            //cache rotated sprites
            for (j=0;j<3;j++) {
                pacElRot[i][j].reset(new Sprite());
                pacElRot[i][j]->load(path + "pac" + num[i] + ".png");
                if (j==1) {
                    pacElRot[i][j]->setFlipX();
                } else {
                    pacElRot[i][j]->setRotation(360-(j+1)*90);
                }
            }
        }


        Logger::msg("Pacman textures loaded");
    }
    catch (Error &err) {
        app.setQuit(true);
        Logger::err(err.getDesc());
        return false;
    }
    catch (...) {
        app.setQuit(true);
        Logger::err("Unexpected error");
        return false;
    }
    return true;
}
int Pacman::getXpix() {
    return xpix;
}

int Pacman::getYpix() {
    return ypix;
}
bool Pacman::collision(int xtmp, int ytmp) {
    //error check
    if (xtmp < 0 || xtmp >= width || ytmp < 0 || ytmp >= height ) return 1;
    //collision detection
    if ( map[ ( ytmp ) * width + (xtmp) ] == 0 &&
         map[ ( ytmp ) * width + (xtmp + 1) ] == 0 &&
         map[ ( ytmp + 1 ) * width + (xtmp) ] == 0 &&
         map[ ( ytmp + 1 ) * width + (xtmp + 1) ] == 0 )
        return 0;
    return 1;
}
void Pacman::setNextDir(int next) {
    if (next == 0) {
        nextdx=0;
        nextdy=-1;
    } else if (next == 1) {
        nextdx=1;
        nextdy=0;
    } else if (next == 2) {
        nextdx=0;
        nextdy=1;
    } else if (next == 3) {
        nextdx=-1;
        nextdy=0;
    }
}
Pacman::Pacman(shared_ptr<sf::RenderWindow> buf, int os, int ix, int iy, int ispdmod,
			   int itilesize, int iheight, int iwidth, int *imap)
:   Object( buf, os),
    x(ix),
    y(iy),
    dx(0),
    dy(0),
    nextdx(0),
    nextdy(0),
    spdmod(ispdmod),
    spdmult(1),
    tilesize(itilesize),
    height(iheight),
    width(iwidth),
    map(imap),
    elapsedTime(0.f)
{

    xpix=x*tilesize;
    ypix=y*tilesize;

    xfloat=(float)xpix;
    yfloat=(float)ypix;
}

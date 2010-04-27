/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#pragma once
#include "Main.h"

class Object
{
public:
    Object(SDL_Surface *buffer, int os )
    :	buf(buffer),
            offset(os),
            paused(true),
            alpha(255)
    {
    }

    virtual ~Object() {}

    void setPaused(bool b) { paused = b; }
    void setAlpha(int a) { alpha = a; }

    virtual void reset( int ix, int iy) = 0;
    virtual void Update(int time) = 0;

    virtual void Draw() = 0;
    virtual void Draw(int ix, int iy, int obj=3, int type=1)=0;
    virtual bool LoadTextures(std::string path)=0;

    virtual SDL_Surface* Rotate( SDL_Surface *src, int angle, double zoomx=1, double zoomy=1 ) {
        SDL_PixelFormat *fmt;

        if (!(angle < 360 && angle >= 0 && angle % 90 == 0))
            return src;

        int turns = (360 - angle) / 90;

        SDL_Surface *zoomedSurface = zoomSurface(src, zoomx, zoomy, SMOOTHING_OFF);

        /* for some reason, if we don't exit here without rotation we get an invisible surface.. */
        if (turns % 4 == 0)
            return zoomedSurface;

        SDL_Surface *rotatedSurface = rotateSurface90Degrees(zoomedSurface, turns);
        SDL_FreeSurface(zoomedSurface);

        return rotatedSurface;
    }
protected:
    SDL_Surface *buf;
    const int offset;
    bool paused;
    int alpha;
};

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

    virtual SDL_Surface* Rotate( SDL_Surface *src, double angle, double zoomx=1, double zoomy=1 ) {
        SDL_PixelFormat *fmt;

        SDL_Surface *tmpSurface = rotozoomSurfaceXY(src,angle,zoomx,zoomy,SMOOTHING_OFF);

        fmt=tmpSurface->format;
        SDL_SetColorKey(tmpSurface,SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(fmt,255,0,255));

        //rotozoom breaks color keys so we need to recreate the surface with the correct format
        SDL_Surface *tmpFixedSurface = SDL_DisplayFormatAlpha(tmpSurface);
        SDL_FreeSurface(tmpSurface);

        return tmpFixedSurface;
    }
protected:
    SDL_Surface *buf;
    const int offset;
    bool paused;
    int alpha;
};

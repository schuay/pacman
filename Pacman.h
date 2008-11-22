#pragma once
#include "Main.h"
#include "Object.h"

#define NUMPACANIM 8

class Pacman :
	public Object
{
public:
	Pacman(SDL_Surface *buf, int os, int ix, int iy, int ispdmod, int itilesize, int iheight, int iwidth, int *imap);
	~Pacman();

	void Draw();
	void Draw(int ix, int iy, int obj=3, int type=1);
	bool LoadTextures(std::string path);

	void setNextDir(int next);
	bool collision(int xtmp, int ytmp);

	void setSpeedMult( int s);

//  Pacman::Rotate now replaced by Object::Rotate
//	void Rotate(int degree, int x, int y);

	void Update( int time );
	void nextIntersection(int &ix, int &iy);
	void reset(int ix, int iy);

	int getXpix();
	int getYpix();
private:
	int		x,	//current position in tiles (all positions refer to upper left corner)
			y,

			dx,	//current direction in x and y
			dy,

			nextdx,	//queued direction
			nextdy,

			xpix,	//current position in pixels
			ypix,

			spdmod,	// speed modifier	- default 100?
			spdmult,

			tilesize,
			height,
			width,

			*map;
	unsigned int animcounter;
	float	xfloat,	//current position as floating point based on pixel pos - allows for infinite speed
			yfloat;	// variations because xfloat can be reduced to xpix
	SDL_Surface	*pacEl[NUMPACANIM],
                *pacElRot[NUMPACANIM][3];
};

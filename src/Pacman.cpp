#include "Pacman.h"

#define PACSIZE 40

extern Log logtxt;
extern App app;

void Pacman::setSpeedMult( int s) {
	spdmult = s;
}
void Pacman::Draw(int ix, int iy, int obj, int type) {
    SDL_Rect pos;

    pos.x=ix;
    pos.y=iy;
    pos.h=PACSIZE;
    pos.w=PACSIZE;

    SDL_SetAlpha(pacEl[3],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
    SDL_BlitSurface(pacEl[3],NULL,buf,&pos);
}
void Pacman::reset(int ix, int iy) {
	animcounter=0;
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
	int		xtmp=x,
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

void Pacman::Update(int time) {
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
		//if nextdir!=dir
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
		if ( dx >= 0 && dy >= 0) {

			dirclear = ! collision(x+dx,y+dy);
		}
		else if ( dx <= 0 && dy <= 0) {
			dirclear = ! collision( (xpix+dx) / tilesize, (ypix+dy) / tilesize);
		}
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

}
void Pacman::Draw() {

	int i;
	SDL_Rect pos;

    //calculate displayed animation frame from animcounter.. abs is not the right function
//    i=7-abs((animcounter-15)/2);
    //there's probably a better way to handle this:
	if ( animcounter >= 0 && animcounter < 2 ) i=0;
	else if ( animcounter >= 2 && animcounter < 4 ) i=1;
	else if ( animcounter >= 4 && animcounter < 6 ) i=2;
	else if ( animcounter >= 6 && animcounter < 8 ) i=3;
	else if ( animcounter >= 8 && animcounter < 10 ) i=4;
	else if ( animcounter >= 10 && animcounter < 12 ) i=5;
	else if ( animcounter >= 12 && animcounter < 14 ) i=6;
	else if ( animcounter >= 14 && animcounter < 16 ) i=7;
	else if ( animcounter >= 16 && animcounter < 18 ) i=7;
	else if ( animcounter >= 18 && animcounter < 20 ) i=6;
	else if ( animcounter >= 20 && animcounter < 22 ) i=5;
	else if ( animcounter >= 22 && animcounter < 24 ) i=4;
	else if ( animcounter >= 24 && animcounter < 26 ) i=3;
	else if ( animcounter >= 26 && animcounter < 28 ) i=2;
	else if ( animcounter >= 28 && animcounter < 30 ) i=1;
	else if ( animcounter >= 30 && animcounter < 32 ) i=0;
	else i=0; //avoid compiler warning

    pos.y=ypix;
    pos.x=xpix;
    pos.w=PACSIZE;
    pos.h=PACSIZE;

	if (dx == 1 && dy == 0) {	//right
//		sprite->Draw(pacEl[ i], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        SDL_SetAlpha(pacEl[i],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
        SDL_BlitSurface(pacEl[i],NULL,buf,&pos);
	}
	else if (dx == -1 && dy == 0) {	//left
//		Rotate(180, xpix, ypix);
//		sprite->Draw(pacEl[ i], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0) , D3DCOLOR_RGBA(255, 255, 255, alpha));
//		sprite->SetTransform( &id );
        SDL_SetAlpha(pacElRot[i][1],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
        SDL_BlitSurface(pacElRot[i][1],NULL,buf,&pos);
	}
	else if (dx == 0 && dy == -1) {	//up
//		Rotate(270, xpix, ypix);
//		sprite->Draw(pacEl[ i], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0) , D3DCOLOR_RGBA(255, 255, 255, alpha));
//		sprite->SetTransform( &id );
        SDL_SetAlpha(pacElRot[i][2],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
        SDL_BlitSurface(pacElRot[i][2],NULL,buf,&pos);
	}
	else if (dx == 0 && dy == 1) {	//down
//		Rotate(90, xpix, ypix);
//		sprite->Draw(pacEl[ i], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0) , D3DCOLOR_RGBA(255, 255, 255, alpha));
//		sprite->SetTransform( &id );
        SDL_SetAlpha(pacElRot[i][0],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
        SDL_BlitSurface(pacElRot[i][0],NULL,buf,&pos);
	}
	else if (dx == 0 && dy == 0) {	//init position
//		sprite->Draw(pacEl[ 0], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        SDL_SetAlpha(pacEl[i],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
        SDL_BlitSurface(pacEl[i],NULL,buf,&pos);
	}

	if ( !paused) {
		if (animcounter == 31) animcounter = 0;
		else animcounter++;
	}
}


bool Pacman::LoadTextures(std::string path) {

	int i,j;
	std::string num[10];
	SDL_PixelFormat *fmt;

    for (i=0;i<10;i++)
        num[i]='0'+i;
//	num[0]="0";
//	num[1]="1";
//	num[2]="2";
//	num[3]="3";
//	num[4]="4";
//	num[5]="5";
//	num[6]="6";
//	num[7]="7";
//	num[8]="8";
//	num[9]="9";

	try {
		for (i=0;i<NUMPACANIM;i++) {
		    pacEl[i]=SDL_LoadBMP((path + "pac" + num[i] + ".bmp").c_str());

		    if ( !pacEl[i] )
				throw Error("Failed to load pacman texture: " + num[i]);

            fmt=pacEl[i]->format;
            SDL_SetColorKey(pacEl[i],SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(fmt,255,0,255));

            //cache rotated sprites
            for (j=0;j<3;j++) {
                if (j==1)
                    pacElRot[i][j]=Rotate(pacEl[i],0,-1,1);
                else
                    pacElRot[i][j]=Rotate(pacEl[i],360-(j+1)*90);
            }
		}


		logtxt.print("Pacman textures loaded");
	}
	catch ( Error &err) {
		std::cerr << err.getDesc();
		app.setQuit(true);
		logtxt.print( err.getDesc() );
		return false;
	}
	catch ( ... ) {
		std::cerr << "Unexpected exception in Pacman::LoadTextures()";
		app.setQuit(true);
		logtxt.print( "Unexpected error" );
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
	if (next >= 0 && next <=3 ) {
		if (next == 0) {
			nextdx=0;
			nextdy=-1;
		}
		if (next == 1) {
			nextdx=1;
			nextdy=0;
		}
		if (next == 2) {
			nextdx=0;
			nextdy=1;
		}
		if (next == 3) {
			nextdx=-1;
			nextdy=0;
		}
	}
}
Pacman::Pacman(SDL_Surface *buf, int os, int ix, int iy, int ispdmod,
			   int itilesize, int iheight, int iwidth, int *imap)
:	Object( buf, os),
	x(ix),
	y(iy),
	spdmod(ispdmod),
	tilesize(itilesize),
	height(iheight),
	width(iwidth),
	map(imap),
	animcounter(0),
	dx(0),
	dy(0),
	nextdx(0),
	nextdy(0),
	spdmult(1)
{
	int i,j;

	for (i=0;i<NUMPACANIM;i++) {
	    pacEl[i]=NULL;
	    for (j=0;j<3;j++)
            pacElRot[i][j]=NULL;
	}

	xpix=x*tilesize;
	ypix=y*tilesize;

	xfloat=(float)xpix;
	yfloat=(float)ypix;
}

Pacman::~Pacman()
{
	int i,j;

	for (i=0;i<NUMPACANIM;i++) {
	    if (pacEl[i]) SDL_FreeSurface(pacEl[i]);
	    for (j=0;j<3;j++)
            if (pacElRot[i][j]) SDL_FreeSurface(pacElRot[i][j]);
	}
}

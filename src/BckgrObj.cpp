#include "BckgrObj.h"

extern Log logtxt;
extern App app;
extern Game game;
extern Settings settings;



void BckgrObj::Draw(int ix, int iy, int obj, int type, int alp) {
    SDL_Rect pos;

    pos.x=ix;
    pos.y=iy;
    pos.h=pos.w=20;

	if (type == 1) {
        SDL_SetAlpha(objEl[obj],SDL_SRCALPHA|SDL_RLEACCEL,alp);
        SDL_BlitSurface(objEl[obj],NULL,buf,&pos);
	}
	else {
        SDL_SetAlpha(mapEl[obj],SDL_SRCALPHA|SDL_RLEACCEL,alp);
        SDL_BlitSurface(mapEl[obj],NULL,buf,&pos);
	}
}

void BckgrObj::Draw(int ix, int iy, int obj, int type) {
    Draw(ix,iy,obj,type,255);
}

void BckgrObj::setFruitAlpha(int a) {
	fruitalpha = a;
}

void BckgrObj::Draw() {
	int *map(NULL),
		*objmap(NULL),
		height,
		width,
		i,
		j;
    SDL_Rect pos;

	height = settings.fieldheight;
	width = settings.fieldwidth;
	game.getMaps(&map, &objmap);

	objcounter = 0;

	SDL_BlitSurface(mapEl[0], NULL, buf, NULL);

	//DRAW FIELD
	for (j=0;j<height;j++) {
		for (i=0; i<width; i++) {

		    pos.x=i*settings.tilesize;
            pos.y=j*settings.tilesize;
            pos.h=20;
            pos.w=20;

			if (map[j*width+i]==1	&&	// horizontal line
				( map[j*width+i+1] != 0 || i == width-1 ) &&
				( map[j*width+i-1] != 0 || i == 0 ) ) {
                SDL_SetAlpha(mapEl[1],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[1], NULL, buf, &pos );
				}

			else if (map[j*width+i]==1)	{	// vertical line
                SDL_SetAlpha(mapElRot[1][0],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapElRot[1][0], NULL, buf, &pos);
			}

			else if (map[j*width+i]==2 &&		//ghost door
					map[j*width+i + 1] != 0 &&
					map[j*width+i - 1] != 0) {
//				sprite->Draw(mapEl[2], NULL, NULL, & D3DXVECTOR3((float)i*settings.tilesize, (float)j*settings.tilesize, 1) , D3DCOLOR_RGBA(255, 255, 255, alpha));
                SDL_SetAlpha(mapEl[2],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[2], NULL, buf, &pos);
                }
			else if (map[j*width+i]==2)	{	// vertical ghost door
                SDL_SetAlpha(mapElRot[2][0],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapElRot[2][0], NULL, buf, &pos);
			}

			else if (map[j*width+i]==3) {		//upper left corner
                SDL_SetAlpha(mapEl[3],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[3], NULL, buf, &pos);
			}
			else if (map[j*width+i]==4) {		// upper right corner
                SDL_SetAlpha(mapEl[4],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[4], NULL, buf, &pos);
			}
			else if (map[j*width+i]==5) {		// lower  right corner
                SDL_SetAlpha(mapEl[5],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[5], NULL, buf, &pos);
			}
			else if (map[j*width+i]==6) {		// lower left corner
                SDL_SetAlpha(mapEl[6],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[6], NULL, buf, &pos);
			}

			else if (map[j*width+i]==7 && 		// left T
					  ( map[j*width+i-1]==0 || i == 0 ) ) {
                SDL_SetAlpha(mapEl[7],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[7], NULL, buf, &pos);
            }
			else if (map[j*width+i]==7)	{	// upside down T
                SDL_SetAlpha(mapElRot[7][0],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapElRot[7][0], NULL, buf, &pos);
			}
			else if (map[j*width+i]==8 &&		// right T
					( map[j*width+i+1]==0 || i == width-1 ) ) {
                SDL_SetAlpha(mapEl[8],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[8], NULL, buf, &pos);
            }
			else if (map[j*width+i]==8)	{	// upright T
                SDL_SetAlpha(mapElRot[8][0],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapElRot[8][0], NULL, buf, &pos);
			}

			else if (map[j*width+i]==9 &&
						map[j*width+i-1] != 0 &&
						map[j*width+i-1] != 2 &&
						i > 0 )	 {//right stub
//				sprite->Draw(mapEl[9], NULL, NULL, & D3DXVECTOR3((float)i*settings.tilesize, (float)j*settings.tilesize, 1) , D3DCOLOR_RGBA(255, 255, 255, alpha));
                SDL_SetAlpha(mapEl[9],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapEl[9], NULL, buf, &pos);
            }
			else if (map[j*width+i]==9 &&
						map[j*width+i+1] != 0 &&
						map[j*width+i+1] != 2 &&
						i < width-1) {	// left stub
                SDL_SetAlpha(mapElRot[9][1],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapElRot[9][1], NULL, buf, &pos);
			}
			else if (map[j*width+i]==9 &&
						map[(j+1)*width+i] != 0 &&
						map[(j+1)*width+i] != 2 &&
						j < height -1) {	// upper stub
                SDL_SetAlpha(mapElRot[9][0],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapElRot[9][0], NULL, buf, &pos);
			}
			else if (map[j*width+i]==9)	{// lower stub
                SDL_SetAlpha(mapElRot[9][2],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(mapElRot[9][2], NULL, buf, &pos);
			}
		}
	}

	//DRAW OBJECTS

	for (j=0;j<height;j++) {
		for (i=0; i<width; i++) {

		    pos.x=i*settings.tilesize+10; // +10 are needed for correct placement
            pos.y=j*settings.tilesize+10;
            pos.h=20;
            pos.w=20;

			if (objmap[j*width+i]==1) {
				SDL_SetAlpha(objEl[1],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(objEl[1], NULL, buf, &pos);
				objcounter++;
			}
			if (objmap[j*width+i]==2) {	// BIG DOTS!
				SDL_SetAlpha(objEl[2],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
                SDL_BlitSurface(objEl[2], NULL, buf, &pos);
				objcounter++;
			}
			if (objmap[j*width+i]==3 && specialspawned && !specialeaten) {	// fruit
                SDL_SetAlpha(objEl[3],SDL_SRCALPHA,fruitalpha);
				SDL_BlitSurface(objEl[3], NULL, buf, &pos);
				objcounter++;
			}
		}
	}


}

bool BckgrObj::LoadTextures(std::string path) {

	int i;
	std::string num[NUMOFMAPTEX];
	SDL_PixelFormat *fmt;

    for (i=0;i<NUMOFMAPTEX;i++)
        num[i]='0'+i;

	try {
		for (i=0;i<NUMOFMAPTEX;i++) {
		    mapEl[i] = SDL_LoadBMP((path + "m" + num[i] + ".bmp").c_str());
		    if ( mapEl[i] == NULL )
				throw Error(num[i] + "Failed to load map texture");

            //get pixel format from surface
            fmt=mapEl[i]->format;
            //set the transparent color key to RGB 255 0 255
            SDL_SetColorKey(mapEl[i],SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(fmt,255,0,255));

            for (int j=0;j<3;j++) {
                mapElRot[i][j]=Rotate(mapEl[i],(j+1)*90);
            }
		}
		for (i=1;i<5;i++) {
			objEl[i] = SDL_LoadBMP((path + "o" + num[i] + ".bmp").c_str());
            if ( objEl[i] == NULL )
				throw Error(num[i] + "Failed to load object texture");

            //get pixel format from surface
            fmt=objEl[i]->format;
            //set the transparent color key to RGB 255 0 255
            SDL_SetColorKey(objEl[i],SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(fmt,255,0,255));
		}

		logtxt.print("Field textures loaded");
	}
	catch ( Error &err) {
		std::cerr << err.getDesc().c_str();
		app.setQuit(true);
		logtxt.print( err.getDesc() );
		return false;
	}
	catch ( ... ) {
		std::cerr << "Unexpected exception in BckgrObj::LoadTextures";
		app.setQuit(true);
		logtxt.print( "Unexpected error during Game()" );
		return false;
	}
	return true;
}

BckgrObj::BckgrObj( SDL_Surface *buffer, int os)
:	Object( buffer, os),
	objcounter(0),
	fruitalpha(255),
	specialspawned(false),
	specialeaten(false)
{
	int i;

	for (i=0;i<10;i++) {
		mapEl[i]=NULL;
		objEl[i]=NULL;
		for (int j=0;j<3;j++)
                mapElRot[i][j]=NULL;
	}
}

BckgrObj::~BckgrObj()
{
	int i;

	for (i=0;i<NUMOFMAPTEX;i++) {
		if ( mapEl[i] ) SDL_FreeSurface(mapEl[i]);
		if ( objEl[i] ) SDL_FreeSurface(objEl[i]);
		for (int j=0;j<3;j++)
                if ( mapElRot[i][j] ) SDL_FreeSurface(mapElRot[i][j]);
	}
}

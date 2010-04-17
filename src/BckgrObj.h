#pragma once
#include "Main.h"
#include "Object.h"

#define NUMOFMAPTEX 10

class BckgrObj :
	public Object
{
public:
    BckgrObj(SDL_Surface *buffer, int os);
    ~BckgrObj();

    void Draw();
    void Draw(int ix, int iy, int obj=3, int type=1);
    void Draw(int ix, int iy, int obj, int type, int alp);

    virtual void reset( int ix, int iy) { ix = iy; };   /* avoid compiler warnings */
    virtual void Update(int time) { time = 0; };        /* avoid compiler warnings */

    bool LoadTextures(std::string path);

    void setSpecialSpawned(bool b) {specialspawned = b;	}
    void setSpecialEaten(bool b) {specialeaten = b;	}

    int getObjCount() {	return objcounter;	}

    void setFruitAlpha(int a);

private:
    SDL_Surface
            *mapEl[NUMOFMAPTEX],
            *objEl[NUMOFMAPTEX],
            *mapElRot[NUMOFMAPTEX][3];

    int
            objcounter,
            fruitalpha;

    bool
            specialspawned,
            specialeaten;
};

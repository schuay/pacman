/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "BckgrObj.h"

extern App app;
extern Game game;
extern Settings settings;



void BckgrObj::Draw(int ix, int iy, int obj, int type, int alp) {

    shared_ptr<Sprite> s = (type == 1) ? objEl[obj] : mapEl[obj];
    s->SetAlpha(alp);
    s->Blit(buf, sf::Vector2i(ix, iy));
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

    height = settings.fieldheight;
    width = settings.fieldwidth;
    game.getMaps(&map, &objmap);

    objcounter = 0;

    mapEl[0]->Blit(buf, sf::Vector2i(0, 0));

    //DRAW FIELD
    for (j=0;j<height;j++) {
        for (i=0; i<width; i++) {

            shared_ptr<Sprite> s;
            switch (map[j * width + i]) {
            case 1:
                if (( map[j*width+i+1] != 0 || i == width-1 ) &&
                    ( map[j*width+i-1] != 0 || i == 0 )) {
                    s = mapEl[1];
                } else {
                    s = mapElRot[1][0];
                }
                break;
            case 2:
                if (map[j*width+i + 1] != 0 && map[j*width+i - 1] != 0) {
                    s = mapEl[2];
                } else {
                    s = mapElRot[2][0];
                }
                break;
            case 3:
                s = mapEl[3];
                break;
            case 4:
                s = mapEl[4];
                break;
            case 5:
                s = mapEl[5];
                break;
            case 6:
                s = mapEl[6];
                break;
            case 7:
                if ( map[j*width+i-1]==0 || i == 0 ) {
                    s = mapEl[7];
                } else {
                    s = mapElRot[7][0];
                }
                break;
            case 8:
                if ( map[j*width+i+1]==0 || i == width-1 ) {
                    s = mapEl[8];
                } else {
                    s = mapElRot[8][0];
                }
                break;
            case 9:
                if (map[j*width+i-1] != 0 &&
                        map[j*width+i-1] != 2 &&
                        i > 0) {
                    s = mapEl[9];
                } else if (map[j*width+i+1] != 0 &&
                           map[j*width+i+1] != 2 &&
                           i < width-1) {
                    s = mapElRot[9][1];
                } else if (map[(j+1)*width+i] != 0 &&
                           map[(j+1)*width+i] != 2 &&
                           j < height -1) {
                    s = mapElRot[9][0];
                } else {
                    s = mapElRot[9][2];
                }
                break;
            default:
                continue;
            }

            s->SetAlpha(alpha);
            s->Blit(buf, sf::Vector2i(i*settings.tilesize, j*settings.tilesize));

        }
    }

    //DRAW OBJECTS

    for (j=0;j<height;j++) {
        for (i=0; i<width; i++) {

            shared_ptr<Sprite> s;
            switch (objmap[j*width+i]) {
            case 1:
            case 2:
                s = objEl[objmap[j*width+i]];
                break;
            case 3:
                if (!specialspawned || specialeaten) {
                    continue;
                }
                s = objEl[objmap[j*width+i]];
                break;
            default:
                continue;
            }

            s->SetAlpha(alpha);
            s->Blit(buf, sf::Vector2i(i*settings.tilesize+10, // +10 are needed for correct placement
                j*settings.tilesize+10));

            objcounter++;
        }
    }


}

bool BckgrObj::LoadTextures(std::string path) {

    int i;
    std::string num[NUMOFMAPTEX];

    for (i=0;i<NUMOFMAPTEX;i++)
        num[i]='0'+i;

    try {
        for (i=0;i<NUMOFMAPTEX;i++) {

            mapEl[i].reset(new Sprite());
            mapEl[i]->Load(path + "m" + num[i] + ".png");

            for (int j=0;j<3;j++) {
                mapElRot[i][j].reset(new Sprite());
                mapElRot[i][j]->Load(path + "m" + num[i] + ".png");
                mapElRot[i][j]->SetRotation((j+1)*90);
            }
        }
        for (i=1;i<5;i++) {

            objEl[i].reset(new Sprite());
            objEl[i]->Load(path + "o" + num[i] + ".png");
        }

        Logger::Msg("Field textures loaded");
    }
    catch ( Error &err) {
        std::cerr << err.getDesc().c_str();
        app.setQuit(true);
        Logger::Err( err.getDesc() );
        return false;
    }
    catch ( ... ) {
        std::cerr << "Unexpected exception in BckgrObj::LoadTextures";
        app.setQuit(true);
        Logger::Err( "Unexpected error during Game()" );
        return false;
    }
    return true;
}

BckgrObj::BckgrObj( shared_ptr<sf::RenderWindow> buffer, int os)
    :	Object( buffer, os),
    objcounter(0),
    fruitalpha(255),
    specialspawned(false),
    specialeaten(false)
{
}

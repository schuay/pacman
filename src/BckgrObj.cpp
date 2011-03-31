/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "BckgrObj.h"

extern Log logtxt;
extern App app;
extern Game game;
extern Settings settings;



void BckgrObj::Draw(int ix, int iy, int obj, int type, int alp) {

    shared_ptr<sf::Sprite> s = (type == 1) ? objEl[obj] : mapEl[obj];
    s->SetPosition(ix, iy);

    s->SetColor(sf::Color(255, 255, 255, alp));

    buf->Draw(*(s.get()));
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

    buf->Draw(*(mapEl[0].get()));

    //DRAW FIELD
    for (j=0;j<height;j++) {
        for (i=0; i<width; i++) {

            shared_ptr<sf::Sprite> s;
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

            s->SetPosition(i*settings.tilesize, j*settings.tilesize);
            s->SetColor(sf::Color(255, 255, 255, alpha));
            buf->Draw(*(s.get()));

        }
    }

    //DRAW OBJECTS

    for (j=0;j<height;j++) {
        for (i=0; i<width; i++) {

            shared_ptr<sf::Sprite> s;
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

            s.get()->SetPosition(i*settings.tilesize+10, // +10 are needed for correct placement
                j*settings.tilesize+10);
            s->SetColor(sf::Color(255, 255, 255, alpha));
            buf->Draw(*(s.get()));

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
            sf::Image *img = new sf::Image();
            imgs[i].reset(img);
            if (!img->LoadFromFile(path + "m" + num[i] + ".png")) {
                throw Error(num[i] + "Failed to load map texture");
            }
            img->CreateMaskFromColor(sf::Color(255, 0, 255));

            mapEl[i].reset(new sf::Sprite(*img));

            for (int j=0;j<3;j++) {
                mapElRot[i][j].reset(new sf::Sprite(*img));
//                mapElRot[i][j]->SetCenter(10.f, 10.f);
//                mapElRot[i][j]->SetRotation((j+1)*90);
            }
        }
        for (i=1;i<5;i++) {
            sf::Image *img = new sf::Image();
            objimgs[i].reset(img);

            if (!img->LoadFromFile(path + "o" + num[i] + ".png")) {
                throw Error(num[i] + "Failed to load object texture");
            }
            img->CreateMaskFromColor(sf::Color(255, 0, 255));

            objEl[i].reset(new sf::Sprite(*img));
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

BckgrObj::BckgrObj( shared_ptr<sf::RenderWindow> buffer, int os)
    :	Object( buffer, os),
    objcounter(0),
    fruitalpha(255),
    specialspawned(false),
    specialeaten(false)
{
}

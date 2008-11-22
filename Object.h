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

	virtual void reset( int ix, int iy) {}
	virtual void Update(int time) {}

	virtual void Draw()=0;
	virtual void Draw(int ix, int iy, int obj=3, int type=1)=0;
	virtual bool LoadTextures(std::string path)=0;

	virtual SDL_Surface* Rotate( SDL_Surface *src, double angle, double zoomx=1, double zoomy=1 ) {
        return rotozoomSurfaceXY(src,angle,zoomx,zoomy,SMOOTHING_OFF);
	}
protected:
	//ID3DXSprite *sprite;
	//LPDIRECT3DDEVICE9	dev;
	//D3DXMATRIX rot, trans, trans2, scale, id;
	SDL_Surface *buf;
	const int offset;
	bool paused;
	int alpha;
};

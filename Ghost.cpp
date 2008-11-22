#include "Ghost.h"

#define GHOSTSIZE 40

extern Log logtxt;
extern App app;
extern Settings settings;


void Ghost::Draw(int ix, int iy, int obj, int type) {
    SDL_Rect pos;

    pos.x=ix;
    pos.y=iy;
    pos.w=pos.h=GHOSTSIZE;

//	sprite->Draw(ghostEl[ 0], NULL, NULL, & D3DXVECTOR3((float)ix, (float)iy, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
    SDL_SetAlpha(ghostEl[0],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
    SDL_BlitSurface(ghostEl[0],NULL,buf,&pos);
}
int Ghost::getXpix() {
	return xpix;
}

int Ghost::getYpix() {
	return ypix;
}
void Ghost::setTarget(int ix, int iy) {
	xtarget=ix;
	ytarget=iy;
}
bool Ghost::collision(int xtmp, int ytmp) {
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
void Ghost::changeDifficulty(int spd, int iq) {
	defspeed += spd;
	baddie_iq += iq;
}
void Ghost::reset(int ix, int iy) {
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

	spdmod=defspeed;
	state=0;
	gateopen=true;

}
int Ghost::getState() {
	return state;
}
bool Ghost::find() {
	int		i,
			currentx = xpix / tilesize,
			currenty = ypix / tilesize,
			tmppos;

	//if target is not clear

	if ( collision( xtarget, ytarget) ) return 0;

	// reset arrays

	for (i=0; i<width*height; i++) {
		closed[i]=0;
		parentDir[i]=4;
		closedF[i]=-1;
		Gstore[i]=-1;
	}

	// reset heap

	heap.reset();

	//init loop

	parentDir[ currenty*width + currentx ]= -1;
	Gstore[ y*width + x ] = 0;
	heap.add( currentx , currenty , calcF(currentx,currenty) );

	// loop start

	while ( true ) {

		//set focus coords

		currentx=heap.getX();
		currenty=heap.getY();

		// remember F, set status as closed

		closedF[currenty*width+currentx]=heap.getF();
		closed[currenty*width+currentx]=1;

		// if current tile == target, terminate loop

		if (currentx == xtarget && currenty == ytarget) return 1;

		//remove from open list

		heap.remove();

		// first up: RIGHT NEIGHBOR SQUARE
		// special case : current == width-2

		i=1;
		if ( currentx == width-2 ) {

			//if neighbor tile is clear and not on closed list

			if ( ! collision_ignore_gate(0, currenty) && !closed[currenty*width+0] ) {

				// if not already on openlist

				if ( !(tmppos=heap.find(0, currenty)) ) {

					Gstore[(currenty)*width+0]=Gstore[currenty*width+currentx] + 1;

					heap.add(0, currenty, calcF(0, currenty));	//add to openlist

					parentDir[currenty*width+0]=3;				//set parent square
				}

				// if already on openlist -> this path lower G?

				else {
					if ( Gstore[currenty*width+0] > Gstore[currenty*width+currentx] + 1 ) {

						Gstore[currenty*width+0] = Gstore[currenty*width+currentx] + 1;	//update G
						parentDir[currenty*width+0]=3;							//update parent direction
						heap.changeF( heap.find(0, currenty), calcF(0,currenty));	//update F stored in openList
					}
				}
			}
		}

		// usual case: current != width -2

		else {
			if (! collision_ignore_gate(currentx+i, currenty) && !closed[currenty*width+currentx +i] ) {
				if ( !(tmppos=heap.find(currentx+i, currenty)) ) {
					Gstore[(currenty)*width+currentx + i]=Gstore[currenty*width+currentx] + 1;
					heap.add(currentx+i, currenty, calcF(currentx+i, currenty));
					parentDir[currenty*width+currentx+i]=3;
				}
				//if already on openlist
				else {			//if this path has a lower G
					if ( Gstore[currenty*width+currentx+i] > Gstore[currenty*width+currentx] + 1 ) {
						Gstore[currenty*width+currentx+i] = Gstore[currenty*width+currentx] + 1;
						parentDir[currenty*width+currentx+i]=3;
						heap.changeF( heap.find(currentx+i, currenty), calcF(currentx+i,currenty));
					}
				}
			}
		}

		// LEFT NEIGHBOR SQUARE
		//special case : currentx == 0

		i = -1;

		if (currentx == 0 ) {
			if (! collision_ignore_gate(width-2, currenty) && !closed[currenty*width+width-2] ) {
				if ( !(tmppos=heap.find(width-2, currenty)) ) {
					Gstore[(currenty)*width+width-2]=Gstore[currenty*width+currentx] + 1;
					heap.add(width-2, currenty, calcF(width-2, currenty));
					parentDir[currenty*width+width-2]=1;
				}
				else {
					if ( Gstore[currenty*width+width-2] > Gstore[currenty*width+currentx] + 1 ) {
						Gstore[currenty*width+width-2] = Gstore[currenty*width+currentx] + 1;
						parentDir[currenty*width+width-2]=1;
						heap.changeF( heap.find(width-2, currenty), calcF(width-2,currenty));
					}
				}
			}
		}

		// normal case
		else {
			if (! collision_ignore_gate(currentx+i, currenty) && !closed[currenty*width+currentx+i] ) {
				if ( !(tmppos=heap.find(currentx+i, currenty)) ) {
					Gstore[(currenty)*width+currentx + i]=Gstore[currenty*width+currentx] + 1;
					heap.add(currentx+i, currenty, calcF(currentx+i, currenty));
					parentDir[currenty*width+currentx+i]=1;
				}
				else {
					if ( Gstore[currenty*width+currentx+i] > Gstore[currenty*width+currentx] + 1 ) {
						Gstore[currenty*width+currentx+i] = Gstore[currenty*width+currentx] + 1;
						parentDir[currenty*width+currentx+i]=1;
						heap.changeF( heap.find(currentx+i, currenty), calcF(currentx+i,currenty));
					}
				}
			}
		}

		// LOWER NEIGHBOR SQUARE

		i=1;

		// special case

		if ( currenty == height -2 ) {
			if ( ! collision_ignore_gate(currentx, 0) && !closed[0*width+currentx] ) {

				// if not already on openlist

				if ( !(tmppos=heap.find(currentx, 0)) ) {

					Gstore[(0)*width+currentx]=Gstore[currenty*width+currentx] + 1;

					heap.add(currentx, 0, calcF(currentx, 0));	//add to openlist

					parentDir[0*width+currentx]=0;				//set parent square
				}

				// if already on openlist -> this path lower G?

				else {
					if ( Gstore[0*width+currentx] > Gstore[currenty*width+currentx] + 1 ) {

						Gstore[0*width+currentx] = Gstore[currenty*width+currentx] + 1;	//update G
						parentDir[0*width+currentx]=0;							//update parent direction
						heap.changeF( heap.find(currentx, 0), calcF(currentx, 0));	//update F stored in openList
					}
				}
			}
		}

		//normal case

		else {

			if (! collision_ignore_gate(currentx, currenty+i) && !closed[(currenty+i)*width+currentx] ) {
				if ( !(tmppos=heap.find(currentx, currenty+i)) ) {
					Gstore[(currenty+i)*width+currentx]=Gstore[currenty*width+currentx] + 1;
					heap.add(currentx, currenty+i, calcF(currentx, currenty+i));
					parentDir[(currenty+i)*width+currentx]=0;
				}
				else {
					if ( Gstore[(currenty+i)*width+currentx] > Gstore[currenty*width+currentx] + 1 ) {
						Gstore[(currenty+i)*width+currentx] = Gstore[currenty*width+currentx] + 1;
						parentDir[(currenty+i)*width+currentx]=0;
						heap.changeF( heap.find(currentx, currenty+i), calcF(currentx,currenty+i));
					}
				}
			}
		}

		// UPPER NEIGHBOR SQUARE

		i=-1;

		//special case
		if (currenty == 0 ) {
			if (! collision_ignore_gate(currentx, height-2) && !closed[(height-2)*width+currentx] ) {
				if ( !(tmppos=heap.find(currentx, height-2)) ) {
					Gstore[(height-2)*width+currentx]=Gstore[currenty*width+currentx] + 1;
					heap.add(currentx, height-2, calcF(currentx, height-2));
					parentDir[(height-2)*width+currentx]=2;
				}
				else {
					if ( Gstore[(height-2)*width+currentx] > Gstore[currenty*width+currentx] + 1 ) {
						Gstore[(height-2)*width+currentx] = Gstore[currenty*width+currentx] + 1;
						parentDir[(height-2)*width+currentx]=2;
						heap.changeF( heap.find(currentx, height-2), calcF(currentx,height-2));
					}
				}
			}
		}

		//normal case
		else {
			if (! collision_ignore_gate(currentx, currenty+i) && !closed[(currenty+i)*width+currentx] ) {
				if ( !(tmppos=heap.find(currentx, currenty+i)) ) {
					Gstore[(currenty+i)*width+currentx]=Gstore[currenty*width+currentx] + 1;
					heap.add(currentx, currenty+i, calcF(currentx, currenty+i));
					parentDir[(currenty+i)*width+currentx]=2;
				}
				else {
					if ( Gstore[(currenty+i)*width+currentx] > Gstore[currenty*width+currentx] + 1 ) {
						Gstore[(currenty+i)*width+currentx] = Gstore[currenty*width+currentx] + 1;
						parentDir[(currenty+i)*width+currentx]=2;
						heap.changeF( heap.find(currentx, currenty+i), calcF(currentx,currenty+i));
					}
				}
			}
		}

		// if open list is empty, terminate

		if ( heap.isEmpty() ) return 0;
	}
}
int Ghost::calcF(int ix, int iy) const {
	int a,b;

	//distance current tile -> start tile

	a= Gstore[(iy)*width+ix] ;

	// x-distance current tile -> target tile

	b= ix > xtarget ? (ix-xtarget) : (xtarget-ix);  //current to target =H

	//special cases: target left edge, baddie right edge

	if ( ( width-1)-ix+xtarget < b ) 	b=(width-2)-ix+xtarget;	//width -1 to get correct F

	//				target right edge, baddie left edge

	if (ix + (width+1) -xtarget < b )	b=ix + (width-2) -xtarget;

	a+=b;

	// y-distance current tile -> target tile

	b= iy > ytarget ? (iy-ytarget) : (ytarget-iy);

	//special case: target upper edge, baddie lower edge

	if ( ( height-1)-iy+ytarget < b ) 	b=(height-2)-iy+ytarget;

	// vice versa

	if (iy + (height+1) -ytarget < b )	b=iy + (height-2) -ytarget;

	a+=b;

	return a;
}
bool Ghost::tracePath() {
	int xtmp, ytmp;

	//reset dirToTar

	dirToTar = -1;

	//set coords

	xtmp= heap.getX();
	ytmp= heap.getY();

	if (ytmp == ytarget && xtmp == xtarget ) {	//error check

		while ( parentDir[ ytmp*width + xtmp ] != -1 ) {

			dirToTar = parentDir[ytmp*width+xtmp];		//not sure about dtotarget -1, maybe without -1

			if (dirToTar == 0) {
				if (ytmp==0) ytmp=height-2;
				else ytmp--;
			}
			else if (dirToTar == 1) {
				if ( xtmp== width-2) xtmp=0;
				else xtmp++;
			}
			else if (dirToTar == 2) {
				if (ytmp == height -2 ) ytmp=0;
				else ytmp++;
			}
			else if (dirToTar == 3) {
				if (xtmp == 0) xtmp= width-2;
				else xtmp--;
			}
		}

		if ( dirToTar == 0 ) dirToTar = 2;
		else if ( dirToTar == 1 ) dirToTar = 3;
		else if ( dirToTar == 2 ) dirToTar = 0;
		else if ( dirToTar == 3 ) dirToTar = 1;

		if (dirToTar < 0 || dirToTar > 3 ) return 1;	//error check

		return 0;
	}

	else return 1;	//error
}
void Ghost::pathCalcVuln() {
	bool flag = 0;
	int newdir = -1;


	//find path
	if ( find() ) flag=tracePath();

	// RANDOM PATH, one not in shortest direction to pacman is preferred (dirToTar)

	//if within the starting square
	if (gateopen) {
		if ( dirToTar == 0) {
			nextdx = 0;
			nextdy = -1;
		}
		else if ( dirToTar == 1 ) {
			nextdx = 1;
			nextdy = 0;
		}
		else if ( dirToTar == 2 ) {
			nextdx = 0;
			nextdy = 1;
		}
		else if ( dirToTar == 3 ) {
			nextdx = -1;
			nextdy = 0;
		}
	}

	//if dead end
	else if ( !dirClear[1+dx][1+dy] && !dirClear[1+dy][1+dx] && !dirClear[1-dy][1-dx] && dx != dy ) {
		nextdx = -dx;
		nextdy = -dy;
	}
	//generate random dir != - current dir
	else {
		nextdx = -dx;
		nextdy = -dy;
		while ( nextdx == -dx && nextdy == -dy ) {

			newdir = rand()%4;

			//the following 2 lines make the baddies prefer directions other than shortest way to pacman

			if (newdir == dirToTar ) newdir = rand()%4;
			if (newdir == dirToTar ) newdir = rand()%4;

			if ( newdir == 0 ) {
				nextdx = 0;
				nextdy = -1;
			}
			else if ( newdir == 1 ) {
				nextdx = 1;
				nextdy = 0;
			}
			else if ( newdir == 2 ) {
				nextdx = 0;
				nextdy = 1;
			}
			else if ( newdir == 3 ) {
				nextdx = -1;
				nextdy = 0;
			}
			if ( !dirClear[1+nextdx][1+nextdy] ) {
				nextdx = -dx;
				nextdy = -dy;
			}
		}
	}

	dx=nextdx;
	dy=nextdy;

}

void Ghost::pathCalcDead() {
	bool flag = 0;
	int newdir = -1,
		cur_opp_dir=-1;	// opposite of current direction

	//translate dx + dy into dir -> 0 = up, 1 = right, 2 = down, 3 = left

	if (dx == 1) cur_opp_dir = 3;
	else if (dx == -1) cur_opp_dir = 1;
	else if (dy == 1) cur_opp_dir = 0;
	else if (dy == -1) cur_opp_dir = 2;



	xtarget= baddie_start_point_x ;
	ytarget= baddie_start_point_y ;

	//find path
	if ( find() ) flag=tracePath();

	//if find and trace successful
	// TRACE PATH

	if (! flag ) {		// pathfinding + trace successful
		if ( dirToTar == 0) {
			nextdx = 0;
			nextdy = -1;
		}
		else if ( dirToTar == 1 ) {
			nextdx = 1;
			nextdy = 0;
		}
		else if ( dirToTar == 2 ) {
			nextdx = 0;
			nextdy = 1;
		}
		else if ( dirToTar == 3 ) {
			nextdx = -1;
			nextdy = 0;
		}
	}

	// ELSE RANDOM PATH	-- only happens of trace not successful
	else {

		//if dead end
		if ( !dirClear[1+dx][1+dy] && !dirClear[1+dy][1+dx] && !dirClear[1-dy][1-dx] && dx != dy ) {
			nextdx = -dx;
			nextdy = -dy;
		}
		//generate random dir != - current dir
		else {
			nextdx = -dx;
			nextdy = -dy;
			while ( nextdx == -dx && nextdy == -dy ) {

				newdir = rand()%4;

				if ( newdir == 0 ) {
					nextdx = 0;
					nextdy = -1;
				}
				else if ( newdir == 1 ) {
					nextdx = 1;
					nextdy = 0;
				}
				else if ( newdir == 2 ) {
					nextdx = 0;
					nextdy = 1;
				}
				else if ( newdir == 3 ) {
					nextdx = -1;
					nextdy = 0;
				}
				if ( !dirClear[1+nextdx][1+nextdy] ) {
					nextdx = -dx;
					nextdy = -dy;
				}
			}
		}
	}
	dx=nextdx;
	dy=nextdy;
}

void Ghost::pathCalcNormal() {
	bool flag = 0;
	int newdir = -1,
		cur_opp_dir=-1;	// opposite of current direction

	//translate dx + dy into dir -> 0 = up, 1 = right, 2 = down, 3 = left

	if (dx == 1) cur_opp_dir = 3;
	else if (dx == -1) cur_opp_dir = 1;
	else if (dy == 1) cur_opp_dir = 0;
	else if (dy == -1) cur_opp_dir = 2;


	if ( gateopen && !collision(settings.gatex, settings.gatey) )
		setTarget(settings.gatex, settings.gatey);

	//find path
	if ( find() ) flag=tracePath();

	//if find and trace successful, random roll successful AND calculated direction != opposite of current dir
	// chance based on Gstore[target square] = distance
	// TRACE PATH

	if (! flag &&		// pathfinding + trace successful
			rand()%(( width+height) / 2 ) + baddie_iq > Gstore[ ytarget * width + xtarget ]  && // random roll successful
			dirToTar != cur_opp_dir) {	//and pathfinding direction is not the opposite of current direction
		if ( dirToTar == 0) {
			nextdx = 0;
			nextdy = -1;
		}
		else if ( dirToTar == 1 ) {
			nextdx = 1;
			nextdy = 0;
		}
		else if ( dirToTar == 2 ) {
			nextdx = 0;
			nextdy = 1;
		}
		else if ( dirToTar == 3 ) {
			nextdx = -1;
			nextdy = 0;
		}
	}

	// ELSE RANDOM PATH
	else {

		//if dead end
		if ( !dirClear[1+dx][1+dy] && !dirClear[1+dy][1+dx] && !dirClear[1-dy][1-dx] && dx != dy ) {
			nextdx = -dx;
			nextdy = -dy;
		}
		//generate random dir != - current dir
		else {
			nextdx = -dx;
			nextdy = -dy;
			while ( nextdx == -dx && nextdy == -dy ) {

				newdir = rand()%4;

				if ( newdir == 0 ) {
					nextdx = 0;
					nextdy = -1;
				}
				else if ( newdir == 1 ) {
					nextdx = 1;
					nextdy = 0;
				}
				else if ( newdir == 2 ) {
					nextdx = 0;
					nextdy = 1;
				}
				else if ( newdir == 3 ) {
					nextdx = -1;
					nextdy = 0;
				}
				if ( !dirClear[1+nextdx][1+nextdy] ) {
					nextdx = -dx;
					nextdy = -dy;
				}
			}
		}
	}
	dx=nextdx;
	dy=nextdy;

}

bool Ghost::collision_ignore_gate(int xtmp, int ytmp) {
	//error check
	if (xtmp < 0 || xtmp >= width || ytmp < 0 || ytmp >= height ) return 1;
	//collision detection
	if ( ( map[ ( ytmp ) * width + (xtmp) ] == 0 || map[ ( ytmp ) * width + (xtmp) ] == 2 ) &&
		 ( map[ ( ytmp ) * width + (xtmp + 1) ] == 0 || map[ ( ytmp ) * width + (xtmp + 1) ] == 2 ) &&
		 ( map[ ( ytmp +1 ) * width + (xtmp) ] == 0  || map[ ( ytmp +1 ) * width + (xtmp) ] == 2 )&&
		 ( map[ ( ytmp + 1 ) * width + (xtmp + 1) ] == 0 || map[ ( ytmp + 1 ) * width + (xtmp + 1) ] == 2 ) )
		return 0;
	return 1;
}
void Ghost::setState(int st) {
	int curdir;

	//vulnerable mode
	if (st == 1 && state != 3) {
		state = st;

		spdmod= 2*defspeed/3;

		if (dx == 1) curdir=1;
		else if (dx == -1) curdir=3;
		else if (dy == 1) curdir=2;
		else if (dy == -1) curdir=0;

		if (curdir == dirToTar ) {
			dx=-dx;
			dy=-dy;
		}
	}
	//warning mode
	else if (st == 2 && state == 1) {
		state = st;
		spdmod= 2*defspeed/3;
		animcounter=0;
	}
	//dead mode
	else if (st == 3 && state != 0) {
		gateopen = 1;
		state = st;
		spdmod= 4*defspeed/3;
	}
	//normal mode
	else if (st == 0 && state == 3) {
		gateopen = 1;
		state = st;
		spdmod =defspeed;
	}
	else if (st == 0 && state != 0) {
		state = st;
		spdmod =defspeed;
	}
}
void Ghost::Update( int time) {
	bool	dirclear=0,
			cont=0;
	int		oldx,
			oldy;

	//if target reached, set normal state

	if ( state == 3 &&
		x == baddie_start_point_x &&
		y == baddie_start_point_y) setState(0);


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
	if ( y == height-2 && dy == 1 ) {
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

	//if at tile beginning

	if ( xpix % tilesize == 0 && ypix % tilesize == 0) {

		//init dirClear array
		//to the right
		if ( gateopen ) {
			//right
			if (collision_ignore_gate( xpix / tilesize + 1, ypix / tilesize ) ) dirClear[1+1][1+0]=0;
			else dirClear[1+1][1+0]=1;

			//left

			if (collision_ignore_gate( xpix / tilesize - 1, ypix / tilesize ) ) dirClear[1-1][1+0]=0;
			else dirClear[1-1][1+0]=1;

			//up

			if (collision_ignore_gate( xpix / tilesize , ypix / tilesize - 1) ) dirClear[1+0][1-1]=0;
			else dirClear[1+0][1-1]=1;

			//down

			if (collision_ignore_gate( xpix / tilesize , ypix / tilesize +1) ) dirClear[1+0][1+1]=0;
			else dirClear[1+0][1+1]=1;
		}
		else {
			//right
			if (collision( xpix / tilesize + 1, ypix / tilesize ) ) dirClear[1+1][1+0]=0;
			else dirClear[1+1][1+0]=1;

			//left

			if (collision( xpix / tilesize - 1, ypix / tilesize ) ) dirClear[1-1][1+0]=0;
			else dirClear[1-1][1+0]=1;

			//up

			if (collision( xpix / tilesize , ypix / tilesize - 1) ) dirClear[1+0][1-1]=0;
			else dirClear[1+0][1-1]=1;

			//down

			if (collision( xpix / tilesize , ypix / tilesize +1) ) dirClear[1+0][1+1]=0;
			else dirClear[1+0][1+1]=1;
		}

		// switch gateopen status if current tile is a gate tile.

		if ( map[ y*width + x] == 2 ) gateopen = !gateopen;

		// if a direction perpendicular to current direction is clear
		// OR current direction is blocked ( = dead end)
		// OR if dx == dy (starting state) set cont flag to 1.
		// cont flag determines whether new dir will be calculated or not

		if ( dirClear[1+dy][1+dx] || dirClear[1-dy][1-dx] || !dirClear[1+dx][1+dy] || dx == dy ) cont = 1;

		// if cont == 1, calc new direction
		// newdir cannot be the opposite of curdir UNLESS it is the only way.

		if (cont == 1 && state == 0) pathCalcNormal();
		else if (cont == 1 && ( state == 1 || state == 2) ) pathCalcVuln();
		else if (cont == 1 && state == 3 ) pathCalcDead();
	}

	//MOVEMENT PART STARTS HERE


		//  dir    *       speed in percent
	xfloat += ( (float)(time * dx * spdmod) / MOVEMOD );
	yfloat += ( (float)(time * dy * spdmod) / MOVEMOD );

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
void Ghost::Draw() {

    SDL_Rect pos;

    pos.x=xpix;
    pos.y=ypix;
    pos.h=GHOSTSIZE;
    pos.w=GHOSTSIZE;

	//normal state

	if (state == 0) {
//	    sprite->Draw(ghostEl[ 0], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        SDL_SetAlpha(ghostEl[0],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
        SDL_BlitSurface(ghostEl[0],NULL,buf,&pos);
	}

	//vulnerable state

	else if (state == 1) {
//	    sprite->Draw(ghostEl[ 2], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        SDL_SetAlpha(ghostEl[2],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
        SDL_BlitSurface(ghostEl[2],NULL,buf,&pos);
	}

	//warning state

	else if (state == 2) {
		if ( !paused ) animcounter++;
		if (animcounter%30 < 15) {
//		    sprite->Draw(ghostEl[ 3], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
            SDL_SetAlpha(ghostEl[3],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
            SDL_BlitSurface(ghostEl[3],NULL,buf,&pos);
		}
		else {
//		    sprite->Draw(ghostEl[ 2], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
            SDL_SetAlpha(ghostEl[2],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
            SDL_BlitSurface(ghostEl[2],NULL,buf,&pos);
		}
	}
	//if dead, only eyes are drawn

	else if (state == 3) {
//	    sprite->Draw(ghostEl[ 4], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        SDL_SetAlpha(ghostEl[4],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
        SDL_BlitSurface(ghostEl[4],NULL,buf,&pos);
	}

	if (dx == 1) {
//	    sprite->Draw(ghostEl[ 1], NULL, NULL, & D3DXVECTOR3((float)xpix+2.0f, (float)ypix, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        pos.x=pos.x+2;
	}
	else if (dx == -1) {
//	    sprite->Draw(ghostEl[ 1], NULL, NULL, & D3DXVECTOR3((float)xpix-2.0f, (float)ypix, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        pos.x=pos.x-2;
	}
	else if (dy == -1) {
//	    sprite->Draw(ghostEl[ 1], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix-2.0f, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        pos.y=pos.y-2;
	}
	else if (dy == 1) {
//	    sprite->Draw(ghostEl[ 1], NULL, NULL, & D3DXVECTOR3((float)xpix, (float)ypix+2.0f, 0.5) , D3DCOLOR_RGBA(255, 255, 255, alpha));
        pos.y=pos.y+2;
	}

    //draw eyes
    SDL_SetAlpha(ghostEl[1],SDL_SRCALPHA|SDL_RLEACCEL,alpha);
    SDL_BlitSurface(ghostEl[1],NULL,buf,&pos);

}

bool Ghost::LoadTextures(std::string path) {

    std::string files[5];
    SDL_PixelFormat *fmt;

    files[0]=path + "baddie" + filename + ".bmp";
    files[1]=path + "baddie_eyes.bmp";
    files[2]=path + "baddie" + filename + "vuln.bmp";
    files[3]=path + "baddie" + filename + "warn.bmp";
    files[4]=path + "baddie_dead.bmp";

	try {

		for (int i = 0; i<5; i++) {
		    ghostEl[i]=SDL_LoadBMP(files[i].c_str());
		    if ( !ghostEl[i] )
                throw Error("Failed to load ghost texture: " + files[i]);

            fmt=ghostEl[i]->format;
            SDL_SetColorKey(ghostEl[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(fmt,255,0,255));
		}
		logtxt.print(filename + " ghost textures loaded");
	}
	catch ( Error &err) {
		std::cerr << err.getDesc();
		app.setQuit(true);
		logtxt.print( err.getDesc() );
		return false;
	}
	catch ( ... ) {
		std::cerr << "Unexpected exception in Ghost::LoadTextures()";
		app.setQuit(true);
		logtxt.print( "Unexpected error" );
		return false;
	}
	return true;


}

Ghost::Ghost(SDL_Surface *buf, int os, int ix, int iy, int ispdmod, int itilesize,
			 int iheight, int iwidth, int *imap, std::string fn)
: Object( buf, os),
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
	heap(iwidth, iheight),
	dirToTar(-1),
	state(0),
	gateopen(1),
	baddie_iq(0)
{
	int i,j;

	filename = fn;

	for (i=0;i<5;i++) ghostEl[i]=NULL;

	xpix=x*tilesize;
	ypix=y*tilesize;

	xfloat=(float)xpix;
	yfloat=(float)ypix;

	defspeed=spdmod;

	baddie_start_point_x=ix;
	baddie_start_point_y=iy;

	closed = new bool[height*width];
	parentDir = new int[height*width];
	Gstore = new int[height*width];
	closedF = new int[height*width];

	for (i=0;i<3;i++) for (j=0; j<3; j++) dirClear[j][i]=0;
}

Ghost::~Ghost()
{
	int i;

	for (i=0;i<5;i++) if ( ghostEl[i] ) SDL_FreeSurface(ghostEl[i]);

	if  ( closed != NULL ) delete[] closed;
	if ( parentDir != NULL ) delete[] parentDir;
	if ( Gstore != NULL ) delete[] Gstore;
	if ( closedF != NULL ) delete[] closedF;
}

#pragma once
#include <vector>
#include "Main.h"

class Settings {
public:
	Settings();
	~Settings();

	bool LoadSettings(std::string filename);
	//searches for str in level/skinspaths; if successful, sets currently selected path.
	//returns 0 on success, 1 on failure
    int setPath(int mode,std::string str);

	//////////////////////////////
	// VARIABLES	- APP
	//////////////////////////////
	int					width,
						height;
//	D3DFORMAT			d3d_bbufformat,
//						d3d_dstencilformat;
//	D3DMULTISAMPLE_TYPE	d3d_multisampling;
//	DWORD				d3d_anisotropy,
//						d3d_vertexprocessing;

	//////////////////////////////
	// VARIABLES	- GAME
	//////////////////////////////

	int					fieldwidth,
						fieldheight,
						tilesize,
						gatex,
						gatey,
						pacstartx,
						pacstarty,
						pacspeed,
						baddiestartx,
						baddiestarty,
						baddiespeed,
						baddieiq,
						vuln_duration,

						lvlpathcount,
						lvlpathcurrent,
						skinspathcount,
						skinspathcurrent;

	std::vector<std::string> lvlpath,
						skinspath;
};

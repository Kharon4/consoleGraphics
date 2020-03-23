#pragma once
#include "math3D/vec3.h"

class screen {
public:
	vec3d TopLeft;
	vec3d right;
	vec3d down;
	short xPixels;
	short yPixels;

	screen(short x = 100, short y = 40, vec3d TOPLEFT = vec3d(-1,0,1) , vec3d RIGHT = vec3d(2,0,0) , vec3d DOWN = vec3d(0,0,-2 ));

};

class cam {
public:
	vec3d vertex;
	screen sc;

	cam(screen SCREEN = screen(), vec3d VERTEX = vec3d(0, -1, 0));
};

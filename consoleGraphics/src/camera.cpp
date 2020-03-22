#include "camera.h"

screen::screen(short x, short y, vec3d TOPLEFT, vec3d RIGHT, vec3d DOWN) {
	xPixels = x;
	yPixels = y;
	TopLeft = TOPLEFT;
	right = RIGHT;
	down = DOWN;
}

cam::cam(screen SCREEN, vec3d VERTEX) {
	sc = SCREEN;
	vertex = VERTEX;
}
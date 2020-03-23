#pragma once
#include "math3D/vec3.h"
#include "camera.h"
#include "consoleColor.h"
#include <vector>

class renderDataMask {
public:
	bool mesh = false;
	bool camPos = false;
	bool realPos = false;
	bool relativePos = false;
	bool vertex = false;
	bool s = false;
	bool pts = false;
};

struct renderData {
	char mesh;
	vec3d camPos;
	vec3d realPos;
	vec3d relativePos;
	vec3d vertex;
	screen s;
	vec3d pts[3];
};

class shader {
	static long unsigned int no;

protected:
	renderDataMask renderDataM;

public:
	static CHAR_INFO none;

	shader();
	~shader();

	virtual CHAR_INFO shade(renderData rd);

	shader* getSelfPointer();
	renderDataMask getRenderMask();
	static unsigned long getNoOfShaders();
};


//solid char shader
class solidCharShader : public shader {
public:
	CHAR_INFO data;
	solidCharShader(CHAR_INFO Data = none);
	CHAR_INFO shade(renderData rd);
};


//rudamentory point light shader 
struct light {
	vec3f color;
	vec3d pt;
	float intensity;
};

class rdLightShader : public shader {
public:
	vec3f surfaceColor;
	std::vector<light>* lights;
	rdLightShader(vec3f SurfaceColor, std::vector<light>* Lights);

	CHAR_INFO shade(renderData rd);
};
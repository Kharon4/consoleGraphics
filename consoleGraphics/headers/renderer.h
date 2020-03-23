#pragma once

#include "mesh.h"
#include "math3D/vec3.h"
#include "math3D/linearMath.h"

class graphicalWorld {
public:
	std::vector<mesh::point> points;
	std::vector<mesh::line> lines;
	std::vector<mesh::triangle> triangles;

	std::vector<std::vector <CHAR_INFO>> render(screen sc, vec3d vertex, CHAR_INFO background);

};

static graphicalWorld _globalWorld;

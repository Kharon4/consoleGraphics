#pragma once

#include "shader.h"
#include "math3d/vec3.h"

namespace mesh {
	class point {
	public:

		vec3d pt;
		shader* shaderPointer;
		point(vec3d PT, shader* Shader) {
			pt = PT;

			shaderPointer = Shader;
		}

	};

	class line {
	public:
		vec3d pts[2];
		shader* shaderPointer;
		line(vec3d pt0, vec3d pt1, shader* Shader) {
			pts[0] = pt0;
			pts[1] = pt1;
			shaderPointer = Shader;
		}
	};

	class triangle {
	public:
		vec3d pts[3];
		shader* shaderPointer;
		triangle(vec3d pt0, vec3d pt1, vec3d pt2, shader* Shader) {
			pts[0] = pt0;
			pts[1] = pt1;
			pts[2] = pt2;
			shaderPointer = Shader;
		}
	};
}
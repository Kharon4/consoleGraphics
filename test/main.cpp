#include<iostream>
#include "consoleController.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"


#include <fstream>

int main() {
	std::cout << "hello world\n";
	consoleController::get(150, 10);
	consoleController::get()->setTitle(L"my engine");
	cam camera;
	camera.sc.xPixels = 256;
	camera.sc.yPixels = 64;
	camera.vertex.z += 0.75;
	camera.sc.TopLeft.z += 0.75;
	//system("pause");
	consoleController::get()->setSize(camera.sc.xPixels, camera.sc.yPixels);
	std::vector<std::vector<CHAR_INFO>>data;
	

	solidCharShader sh(color::getColor(1, 0.75, 0));
	
	mesh::line l(vec3d(-1, 1.5, 0), vec3d(-0.25, 1.5, 0), &sh);
	_globalWorld.lines.push_back(l);
	data = _globalWorld.render(camera.sc, camera.vertex,color::getColor(0,0.25,0.5));

	consoleController::get()->draw(&data);
	system("pause");
	return 0;
}
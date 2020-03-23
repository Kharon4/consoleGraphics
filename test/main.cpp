#include<iostream>
#include "consoleController.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"


int main() {
	std::cout << "hello world\n";
	consoleController::get(150, 10);
	consoleController::get()->setTitle(L"my engine");
	cam camera;
	system("pause");
	consoleController::get()->setSize(camera.sc.xPixels, camera.sc.yPixels);
	system("pause");
	std::vector<std::vector<CHAR_INFO>>data;
	

	solidCharShader sh(color::getColor(1,0,0));
	mesh::line l(vec3d(-0.5, 1, -0.25), vec3d(0.5, 1, 0.25), &sh);
	_globalWorld.lines.push_back(l);

	data = _globalWorld.render(camera.sc, camera.vertex, color::getColor(0.75, 0.75, 0.75));
	consoleController::get()->draw(&data);
	system("pause");
	return 0;
}
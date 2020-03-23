#include<iostream>
#include "consoleController.h"
#include "consoleColor.h"
#include "input.h"

int main() {
	std::cout << "hello world\n";
	consoleController::get(150, 10);
	consoleController::get()->setTitle(L"my engine");
	system("pause");
	consoleController::get()->setSize(100, 15);
	system("pause");
	std::vector<std::vector<CHAR_INFO>>data;
	std::vector<CHAR_INFO> Data;
	CHAR_INFO val;
	for (int i = 0; i < 100; ++i){
		val = color::getColor(i / 100.0, (100 - i) / 100.0, 0);
		Data.push_back(val);
	}
	data.push_back(Data);

	consoleController::get()->draw(&data);
	while (1) {
		input::get()->update();
		std::cout << input::get()->mouseX << " , " << input::get()->mouseY << std::endl;
	}
	return 0;
}
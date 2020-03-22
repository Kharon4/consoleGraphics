#include<iostream>
#include "consoleController.h"

int main() {
	std::cout << "hello world\n";
	consoleController::get(150,10);
	consoleController::get()->setTitle(L"my engine");
	
	std::vector<std::vector<CHAR_INFO>>data;
	std::vector<CHAR_INFO> Data;
	CHAR_INFO val;
	val.Char.AsciiChar = '#';
	val.Attributes = 200;
	Data.push_back(val);
	data.push_back(Data);

	consoleController::get()->draw(&data);

	system("pause");
	return 0;
}
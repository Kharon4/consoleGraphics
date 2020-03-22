#include<iostream>
#include "consoleController.h"

int main() {
	std::cout << "hello world\n";
	consoleController::get(150,10);
	consoleController::get()->setTitle(L"my engine");
	
	

	system("pause");
	return 0;
}
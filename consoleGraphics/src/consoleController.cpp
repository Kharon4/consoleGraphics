#include "consoleController.h"
#include<chrono>
#include <fstream>


uint64_t millis() {
	uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::
		now().time_since_epoch()).count();
	return ms;
}

void consoleController::drawLogo() {
	std::vector<std::vector<CHAR_INFO>>dataT;
	std::ifstream file("startupLogo.bin", std::ios::in | std::ios::binary);
	short size;
	file.read((char*)&size, sizeof(short));
	std::vector<CHAR_INFO> temp;
	for (int i = 0; i < size; ++i)temp.push_back(CHAR_INFO());
	file.read((char*)&size, sizeof(short));
	for (int i = 0; i < size; ++i)dataT.push_back(temp);

	for(int i = 0 ; i < dataT.size() ; ++i)
		for (int j = 0; j < dataT[0].size(); ++j)
			file.read((char*)&(dataT[i][j]), sizeof(CHAR_INFO));

	setSize(dataT[0].size(), dataT.size());
	draw(&dataT);
}

consoleController* consoleController::obj = nullptr;

consoleController::consoleController(short X, short Y, LPCWSTR Title) {
	x = X;
	y = Y;
	title = Title;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	data = nullptr;
	SetConsoleTitle(L"ConsoleGraphics by AbhishekKhurana");
	unsigned long long start = millis();
	drawLogo();
	while (millis() - start < 1500);

	setSize(X, Y);
	SetConsoleTitle(title);
	data = new CHAR_INFO[x * ((long)y)];
}

consoleController::~consoleController() {
	delete[] data;
}

consoleController* consoleController::get(short X, short Y, LPCWSTR Title) { if (obj == nullptr)obj = new consoleController(X,Y,Title); return obj; }

void consoleController::setTitle(LPCWSTR Title) {
	title = Title;
	SetConsoleTitle(title);
}

LPCWSTR consoleController::getTitle() { return title; }

void consoleController::setSize(short X, short Y) {
	x = X;
	y = Y;
	COORD bufferSize = { x,y };
	SMALL_RECT windowSize = { 0,0,x - 1,y - 1 };
	SMALL_RECT windowSizeTemp = { 0,0,1,1 };
	SetConsoleWindowInfo(consoleHandle, TRUE, &windowSizeTemp);
	SetConsoleScreenBufferSize(consoleHandle, bufferSize);
	SetConsoleWindowInfo(consoleHandle, TRUE, &windowSize);
	SetConsoleScreenBufferSize(consoleHandle, bufferSize);
	
	if(data!=nullptr)delete[] data;
	data = nullptr;
	data = new CHAR_INFO[x * ((long)y)];
}

short consoleController::getWidth() { return x; }
short consoleController::getHeight() { return y; }

void consoleController::draw(std::vector< std::vector<CHAR_INFO> >* d) {
	
	for (int i = 0; (i < y) && (i < (*d).size()); ++i) {
		for (int j = 0; j < x && (j < (*d)[i].size()); ++j) {
			data[i * x + j] = (*d)[i][j];
		}
	}
	draw();
}

void consoleController::draw(CHAR_INFO* Data) {
	if (Data == nullptr)Data = data;
	COORD bsize = { x , y };
	COORD null = { 0 , 0 };
	SMALL_RECT rect;
	rect.Top = 0;
	rect.Left = 0;
	rect.Right = x;
	rect.Bottom = y;
	WriteConsoleOutputA(consoleHandle, Data, bsize, null, &rect);
}
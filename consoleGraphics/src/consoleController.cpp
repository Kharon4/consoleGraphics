#include "consoleController.h"



consoleController* consoleController::obj = nullptr;

consoleController::consoleController(short X, short Y, LPCWSTR Title) {
	x = X;
	y = Y;
	title = Title;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SMALL_RECT windowSize = { 0,0,x - 1,y - 1 };
	SetConsoleWindowInfo(consoleHandle, 1, &windowSize);
	COORD bufferSize = { x,y };
	SetConsoleScreenBufferSize(consoleHandle, bufferSize);
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
	SMALL_RECT windowSize = { 0,0,x - 1,y - 1 };
	SetConsoleWindowInfo(consoleHandle, 1, &windowSize);
	COORD bufferSize = { x,y };
	SetConsoleScreenBufferSize(consoleHandle, bufferSize);
	delete[] data;
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
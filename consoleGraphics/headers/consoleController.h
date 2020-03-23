#pragma once

#include<Windows.h>
#include<vector>

class consoleController {
private:
	static consoleController* obj;
	short x, y;
	LPCWSTR title;

	HANDLE consoleHandle;
	CHAR_INFO *data;
	
	void drawLogo();

	consoleController(short X, short Y, LPCWSTR Title);
	~consoleController();
public:
	consoleController(const consoleController&) = delete;
	consoleController& operator=(const consoleController&) = delete;

	static consoleController* get(short X = 80 , short Y = 20 , LPCWSTR Title = L"consoleGraphics");

	void setTitle(LPCWSTR Title);
	LPCWSTR getTitle();

	void setSize(short X, short Y);
	short getWidth();
	short getHeight();

	void draw(std::vector< std::vector<CHAR_INFO> >* d);
	void draw(CHAR_INFO* Data = nullptr);

};

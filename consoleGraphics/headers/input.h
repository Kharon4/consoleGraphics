#pragma once

#include<windows.h>


class input {
private:
	
	static input * obj;
	unsigned long long time;//for timing
	HWND consoleHandel;//for input

	input();
	~input();
public:

	input(const input&) = delete;
	input& operator=(const input&) = delete;

	//keyboard related variables
	bool isDown[256];
	bool pressed[256];
	bool released[256];
	bool go[256];//start a frame after pressed and ends with isDown
	bool end[256];//starts a frame after released and ends with start of isDown 

	//mouse related variables
	long int mouseX, mouseY;
	long int mouseRelX, mouseRelY;
	long int changeX, changeY;
	long int lockX, lockY;
	bool lock = false;

	//timing related variables
	double deltaTime = 0;
	
	static input* get();

	void update(bool checkForTime = true);//call for updating input state

	uint64_t millis();

	void hideCursor();
	void showCursor();
};

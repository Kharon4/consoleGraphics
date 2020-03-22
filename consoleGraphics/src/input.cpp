#include<chrono>

#include "input.h"


using namespace std;


input* input::obj = nullptr;

input::input() {
	consoleHandel = GetConsoleWindow();
	for (int i = 0; i < 256; ++i) {
		isDown[i] = false;
		pressed[i] = false;
		released[i] = false;
		go[i] = false;
		end[i] = false;
	}
	time = millis();
}

input::~input() {}

input* input::get() {
	if (obj == nullptr)obj = new input;
	return obj;
}

uint64_t input::millis(){
	uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::
		now().time_since_epoch()).count();
	return ms;
}

void input::update(bool checkForTime) {
	//update keyboard stuff
	for (int i = 0; i < 256; ++i) {

		bool temp = GetAsyncKeyState(i);
		//go
		if (temp && isDown[i])go[i] = true;
		else go[i] = false;
		//end
		if (!temp && !isDown[i])end[i] = true;
		else end[i] = false;

		isDown[i] = temp;

		if (!pressed[i] && isDown[i] && !go[i]) {
			pressed[i] = true;
		}
		else {
			pressed[i] = false;
		}
		if (!released[i] && !isDown[i] && !end[i]) {
			released[i] = true;
		}
		else {
			released[i] = false;
		}
	}
	//update mouse stuff
	POINT p;
	GetCursorPos(&p);
	changeX = p.x - mouseX;
	changeY = p.y - mouseY;
	mouseX = p.x;
	mouseY = p.y;
	ScreenToClient(consoleHandel, &p);
	mouseRelX = p.x;
	mouseRelY = p.y;
	if (lock) {
		SetCursorPos(lockX, lockY);
		GetCursorPos(&p);
		mouseX = p.x;
		mouseY = p.y;
		ScreenToClient(consoleHandel, &p);
		mouseRelX = p.x;
		mouseRelY = p.y;
	}
	
	if(checkForTime){//get deltatime
		unsigned long long temp = millis();
		time = temp - time;
		deltaTime = time / 1000.0;
		time = temp;
	}
}

void input::hideCursor() {
	(ShowCursor(NULL));
}
void input::showCursor() {
	ShowCursor(true);
}
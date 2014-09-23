#pragma once
#define MAX_CONTROLLERS 2
#include "include\SDL\SDL.h"
/**
* Input Handler Class. Input handler manages input devices and maps them as appropriate.
*/
class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	SDL_Joystick* joystick[MAX_CONTROLLERS];

	int joyMode; // 0 = Gamepad. 1 = Dual Joy.

	bool OpenAll();
	void CloseAll();
	void Spin(SDL_Event* e);
	void getAxes(SDL_Event* e);
	void getHats(SDL_Event* e);
	void HandleBtn(SDL_Event* e);

private:
	int cCam[2];
	static const char digits[10];
	
	
};


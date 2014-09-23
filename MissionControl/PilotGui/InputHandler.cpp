#include "InputHandler.h"
#include <algorithm> 
#include <iostream>
#include "globals.h"

#define JOYSTICK_SCALE_FACTOR 0.03051850947
#define JOYSTICK_DEAD_ZONE 40


const char InputHandler::digits[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };



InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

/**
* Open all joystick devices and set modes.
*/
bool InputHandler::OpenAll()
{
	
	//CloseAll();
	if (SDL_NumJoysticks() < 1)
		return false;
	else if (SDL_NumJoysticks() == 1) //Load Gamepad Mode
	{
		this->joyMode = 0;
		//Set EXPO and Sensitivy to constants:
		joyData->axis[3] = -600; //Sensitivity
		joyData->axis[7] = 100; //EXPO gain
		cCam[0] = '0';
		cCam[1] = '1';
	}
	else if (SDL_NumJoysticks() >= 2) //Load Joystick Mode
		this->joyMode = 1;

	int k = min(SDL_NumJoysticks(), 2);
	for (int i = 0; i < k;  i++)
	{
		joystick[i] = SDL_JoystickOpen(i);
		if (joystick[i] == NULL)
			return false;
	}
	return true;
}

void InputHandler::CloseAll()
{
	printf("close joysticks");
	for each(SDL_Joystick* joy in joystick)
	{
		if (joy != NULL)
		{
			SDL_JoystickClose(joy);
			joy = NULL;
		}
	}
}

void InputHandler::Spin(SDL_Event* e)
{
	getAxes(e);
		//Motion on controller 0
}

void InputHandler::getAxes(SDL_Event* e)
{
	if (abs(JOYSTICK_SCALE_FACTOR*e->jaxis.value) < JOYSTICK_DEAD_ZONE)
	{
		joyData->axis[e->jaxis.axis + e->jaxis.which * 4] = 0; //Map joystick axis to axes array
		return;
	}
	
	switch (joyMode)
	{
		case 0:  // Gamepad
			switch (e->jaxis.axis)
			{
			case 0:
				joyData->axis[0] = (JOYSTICK_SCALE_FACTOR*e->jaxis.value);
				break;
			case 1:
				joyData->axis[1] = (JOYSTICK_SCALE_FACTOR*e->jaxis.value);
				break;
			case 2:
				joyData->axis[4] =(JOYSTICK_SCALE_FACTOR*e->jaxis.value);
				break;
			case 4:
				joyData->axis[5] =(JOYSTICK_SCALE_FACTOR*e->jaxis.value);
				break;
			}
			break;
		case 1:  // Dual device
				joyData->axis[e->jaxis.axis + e->jaxis.which * 4] = (JOYSTICK_SCALE_FACTOR*e->jaxis.value); //Map joystick axis to axes array
			break;
	}
}

void InputHandler::getHats(SDL_Event* e)
{
	switch (joyMode)
	{
	case 0:  // Gamepad
	case 1:  // Dual device
		switch (e->jhat.value)
		{
		case SDL_HAT_LEFTUP:
			joyData->axis[8 + e->jhat.which * 2] = -1000;
			joyData->axis[9 + e->jhat.which * 2] = 1000;
			break;
		case SDL_HAT_UP:
			joyData->axis[8 + e->jhat.which * 2] = 0;
			joyData->axis[9 + e->jhat.which * 2] = 1000;
			break;
		case SDL_HAT_RIGHTUP:
			joyData->axis[8 + e->jhat.which * 2] = 1000;
			joyData->axis[9 + e->jhat.which * 2] = 1000;
			break;
		case SDL_HAT_LEFT:
			joyData->axis[8 + e->jhat.which * 2] = -1000;
			joyData->axis[9 + e->jhat.which * 2] = 0;
			break;
		case SDL_HAT_CENTERED:
			joyData->axis[8 + e->jhat.which * 2] = 0;
			joyData->axis[9 + e->jhat.which * 2] = 0;
			break;
		case SDL_HAT_RIGHT:
			joyData->axis[8 + e->jhat.which * 2] = 1000;
			joyData->axis[9 + e->jhat.which * 2] = 0;
			break;
		case SDL_HAT_LEFTDOWN:
			joyData->axis[8 + e->jhat.which * 2] = -1000;
			joyData->axis[9 + e->jhat.which * 2] = -1000;
			break;
		case SDL_HAT_DOWN:
			joyData->axis[8 + e->jhat.which * 2] = 0;
			joyData->axis[9 + e->jhat.which * 2] = -1000;
			break;
		case SDL_HAT_RIGHTDOWN:
			joyData->axis[8 + e->jhat.which * 2] = 1000;
			joyData->axis[9 + e->jhat.which * 2] = -1000;
			break;
		}
		break;
	case 2:  // Single devices multi-axes
		break;
	}
}

void InputHandler::HandleBtn(SDL_Event* e)
{
	switch (joyMode)
	{
	case 0:  // Gamepad
		switch (e->jbutton.button)
		{
			
		case 0: // Cam 2
			OutHdlr->mode = 0;
			break;
		case 1: // Cam 2
			OutHdlr->mode = 1;
			break;
		case 2: // Cam 2
			OutHdlr->mode = 2;
			break;
		case 3: // Cam 2
			OutHdlr->mode = 3;
			break;
		case 8:
			cCam[0]++;
			cCam[0] %= 4;
			cam[0] = digits[cCam[0]];
			break;
		case 9:
			cCam[1]++;
			cCam[1] %= 4;
			cam[1] = digits[cCam[1]];
			break; 
		default:
			printf("Unhandled Button Press: %d on Joystick %d\n", e->jbutton.button, e->jbutton.which);
			break;
		}
		break;
	case 1:  // Dual device
		switch (e->jbutton.button)
		{
		case 0:
			if (e->jbutton.which == 0)
				ctrlCli->uSend("T000A5", 6);
			else
				ctrlCli->uSend("T000A4", 6);
			break;
		case 1:
			if (e->jbutton.which == 1)
				thumbLock = !thumbLock;
			break;
		case 2: // 2
			cam[e->jbutton.which] = '0';
			break;
		case 3: // 3
			cam[e->jbutton.which] = '3';
			break;
		case 4: // 1
			cam[e->jbutton.which] = '1';
			break;
		case 5: // 4
			cam[e->jbutton.which] = '2';
			break;
		case 6: 
			if(e->jbutton.which == 0)
				OutHdlr->mode = 0;
			else
				ctrlCli->uSend("T000A0", 6); // Stow
			break;
		case 7:
			if (e->jbutton.which == 0)
				OutHdlr->mode = 1;
			else
				ctrlCli->uSend("T000A1", 6); //Rdy Pos
			break;
		case 8:
			if (e->jbutton.which == 0) 
				OutHdlr->mode = 2;
			else
				ctrlCli->uSend("T000A2", 6); // Deposit
			break;
		case 9:
			if (e->jbutton.which == 0)
				OutHdlr->mode = 3;
			break;
		default:
			printf("Unhandled Button Press: %d on Joystick %d\n", e->jbutton.button, e->jbutton.which);
			break;
		}
		break;
	}

}
	

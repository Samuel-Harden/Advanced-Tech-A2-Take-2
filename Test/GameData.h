#pragma once

#include <dinput.h>
//#include "camera.h"

struct GameData
{
	float m_dt;  //time step since last frame
	//GameStateEnum m_GS; //global GameState

	// player input
	unsigned char* m_keyboardState; //current state of the Keyboard
	unsigned char* m_prevKeyboardState; //previous frame's state of the keyboard
	DIMOUSESTATE* m_mouseState; //current state of the mouse
	//Camera* m_currentCam;
};

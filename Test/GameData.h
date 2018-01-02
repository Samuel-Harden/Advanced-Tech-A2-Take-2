#pragma once

#include <dinput.h>
#include "Camera.h"

struct GameData
{
	float m_dt;  //time step since last frame

	// player input
	unsigned char* m_keyboard_state;	  //current state of the Keyboard
	unsigned char* m_prev_keyboard_state; //previous frame's state of the keyboard
	DIMOUSESTATE* m_mouse_state;		  //current state of the mouse
	Camera* m_current_cam;
	bool play;
};

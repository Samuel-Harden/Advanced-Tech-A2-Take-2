#pragma once

#include <dinput.h>

class TPSCamera;
struct GameData;

class InputHandler
{
public:
	InputHandler(HWND& _hWnd, HINSTANCE& _hInstance);
	~InputHandler();

	bool readKeyboard(); //Get current Keyboard state
	bool readMouse();    //Get current Keyboard state

	unsigned char& getKeyboardState();
	unsigned char& getPreviousKeyboardState();
	DIMOUSESTATE&  getMouseState();

	bool tick(GameData* _GD, TPSCamera* _camera);

protected:

	IDirectInput8*			m_direct_input;
	IDirectInputDevice8*	m_keyboard;
	IDirectInputDevice8*	m_mouse;
	unsigned char			m_keyboard_state[256];
	unsigned char			m_prev_keyboard_state[256];
	DIMOUSESTATE			m_mouse_state;
	DIMOUSESTATE			m_prev_mouse_state;

private:

};
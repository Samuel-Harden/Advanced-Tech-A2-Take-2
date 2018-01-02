#include "InputHandler.h"

#include "TPSCamera.h"
#include "GameData.h"


InputHandler::InputHandler(HWND& _hWnd, HINSTANCE& _hInstance)
{
	m_keyboard     = nullptr;
	m_direct_input = nullptr;

	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_direct_input, NULL);
	hr = m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_keyboard->SetCooperativeLevel(_hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_direct_input->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	hr = m_mouse->SetCooperativeLevel(_hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}


InputHandler::~InputHandler()
{
	//tidy away Direct Input Stuff
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
	}
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
	}
	if (m_direct_input)
	{
		m_direct_input->Release();
	}
}


unsigned char& InputHandler::getKeyboardState()
{
	return m_keyboard_state[256];
}


unsigned char& InputHandler::getPreviousKeyboardState()
{
	return m_prev_keyboard_state[256];
}


DIMOUSESTATE& InputHandler::getMouseState()
{
	return m_mouse_state;
}


bool InputHandler::tick(GameData* _GD, TPSCamera* _camera)
{
	if ((m_keyboard_state[DIK_ESCAPE] & 0x80) &&
		!(m_prev_keyboard_state[DIK_ESCAPE] & 0x80))
	{
		// return false to exit the application
		return false;
	}

	if ((m_keyboard_state[DIK_P] & 0x80) &&
		!(m_prev_keyboard_state[DIK_P] & 0x80))
	{
		if (_GD->play == true)
		{
			_GD->play = false;
			//return true;
		}

		else if (_GD->play == false)
		{
			_GD->play = true;
			//return true;
		}
	}

	if ((m_mouse_state.rgbButtons[1] & 0x80))
	{
		_camera->allowRotation(_GD);
	}

	if ((-m_mouse_state.lZ & 0X80))
	{
		_camera->increaseZoom();
	}

	if ((m_mouse_state.lZ & 0X80))
	{
		_camera->decreaseZoom();
	}

	return true;
}


bool InputHandler::readKeyboard()
{
	// Copy over old keyboard state
	memcpy(m_prev_keyboard_state, m_keyboard_state, sizeof(unsigned char) * 256);

	// clear out previous state
	ZeroMemory(&m_keyboard_state, sizeof(m_keyboard_state));

	// Read the keyboard device
	HRESULT hr = m_keyboard->GetDeviceState(sizeof(m_keyboard_state),
		(LPVOID)&m_keyboard_state);

	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired
		// then try to get control back
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}

		else
		{
			return false;
		}
	}

	return true;
}


bool InputHandler::readMouse()
{
	// Set previous mouse state
	m_prev_mouse_state = m_mouse_state;

	// clear out previous state
	ZeroMemory(&m_mouse_state, sizeof(m_mouse_state));

	// Read the mouse device
	HRESULT hr = m_mouse->GetDeviceState(sizeof(m_mouse_state),
		(LPVOID)&m_mouse_state);

	if (FAILED(hr))
	{
		// If the mouse lost focus or was not acquired
		// then try to get control back
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}

		else
		{
			return false;
		}
	}

	return true;
}
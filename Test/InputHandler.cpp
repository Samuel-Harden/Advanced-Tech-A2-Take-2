#include "InputHandler.h"

#include "TPSCamera.h"

#include "GameData.h"



InputHandler::InputHandler(HWND& _hWnd, HINSTANCE& _hInstance)
{
	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(_hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pMouse->SetCooperativeLevel(_hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}



InputHandler::~InputHandler()
{
	//tidy away Direct Input Stuff
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
	}
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
	}
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
	}
}



unsigned char& InputHandler::getKeyboardState()
{
	return m_keyboardState[256];
}



unsigned char& InputHandler::getPreviousKeyboardState()
{
	return m_prevKeyboardState[256];
}



DIMOUSESTATE& InputHandler::getMouseState()
{
	return m_mouseState;
}



bool InputHandler::Tick(GameData* _GD, TPSCamera* _camera)
{
	if ((m_keyboardState[DIK_ESCAPE] & 0x80) &&
		!(m_prevKeyboardState[DIK_ESCAPE] & 0x80))
	{
		// return false to exit the application
		return false;
	}

	if ((m_mouseState.rgbButtons[1] & 0x80))
	{
		_camera->AllowRotation(_GD);
	}

	if ((-m_mouseState.lZ & 0X80))
	{
		_camera->IncreaseZoom();
	}

	if ((m_mouseState.lZ & 0X80))
	{
		_camera->DecreaseZoom();
	}

	return true;
}



bool InputHandler::readKeyboard()
{
	// Copy over old keyboard state
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(unsigned char) * 256);

	// clear out previous state
	ZeroMemory(&m_keyboardState, sizeof(m_keyboardState));

	// Read the keyboard device
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState),
		(LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired
		// then try to get control back
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
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
	m_prevMouseState = m_mouseState;

	// clear out previous state
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));

	// Read the mouse device
	HRESULT hr = m_pMouse->GetDeviceState(sizeof(m_mouseState),
		(LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		// If the mouse lost focus or was not acquired
		// then try to get control back
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}

		else
		{
			return false;
		}
	}
	return true;
}
#include "Game.h"

// system headers
#include <windows.h>
#include <time.h>
#include <iostream>

// my headers
#include "GameData.h"
#include "DrawData.h"

#include "InputHandler.h"

#include "SwarmBot.h"
#include "SwarmManager.h"
#include "TPSCamera.h"
#include "Light.h"


Game::Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance)
{

	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);


	// Seed the random number generator
	srand((UINT)time(NULL));

	m_hWnd = _hWnd;

	m_GD = new GameData;


	// Init render system for VBGOs
	VBGO::Init(_pd3dDevice);

	// Find how big my window is to correctly calculate my aspect ratio
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	float AR = (float)width / (float)height;

	screenWidth = width;
	screenHeight = height;

	input_handler = std::make_unique <InputHandler>(_hWnd, _hInstance);

	// Set GameData struct and populate its pointers
	m_GD->m_keyboardState = &input_handler->getKeyboardState();
	m_GD->m_prevKeyboardState = &input_handler->getPreviousKeyboardState();
	m_GD->m_mouseState = &input_handler->getMouseState();

	//create a base light
	m_light = new Light(XMFLOAT3(220.0f, 220.0f, 220.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.4f, 0.1f, 0.1f, 1.0f));

	bot_cam = new SwarmBot();
	bot_cam->init(_pd3dDevice);
	bot_cam->setPos({0.0f, 0.0f, 0.0f});

	m_camera = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, bot_cam,
		DirectX::Vector3Up, XMFLOAT3{0.0f, 5.0f, -60.0f});
	
	//create DrawData struct and populate its pointers
	m_DD = new DrawData;
	m_DD->m_pd3dImmediateContext = nullptr;
	m_DD->m_cam = m_camera;
	m_DD->m_light = m_light;

	swarm_manager = std::make_unique<SwarmManager>(_pd3dDevice, 200);

	//lock the cursor to the centre of the window
	RECT window;

	GetWindowRect(m_hWnd, &window);

	SetCursorPos((window.left + window.right) >> 1,
		(window.bottom + window.top) >> 1);

	//ShowCursor(true);
};



Game::~Game()
{
	//delete Game Data & Draw Data
	delete m_GD;
	delete m_DD;

	//tidy up VBGO render system
	VBGO::CleanUp();
};



bool Game::Tick()
{
	//Poll Keyboard
	input_handler->readKeyboard();

	// Poll Mouse
	input_handler->readMouse();

	if (input_handler->Tick(m_GD, m_camera) == false)
	{
		// if False exit the application
		return false;
	}

	m_camera->tick(m_GD);
	m_light->tick(m_GD);

	swarm_manager->Tick(m_GD);

	//calculate frame time-step dt for passing down to game objects
	DWORD currentTime = GetTickCount();
	m_GD->m_dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;

	return true;
}



void Game::Draw(ID3D11DeviceContext* _pd3dImmediateContext)
{
	//set immediate context of the graphics device
	m_DD->m_pd3dImmediateContext = _pd3dImmediateContext;

	m_DD->m_cam = m_camera;

	//update the constant buffer for the rendering of VBGOs
	VBGO::UpdateConstantBuffer(m_DD);

	//bot_cam->draw(m_DD);

	swarm_manager->Draw(m_DD);


	m_light->draw(m_DD);
	m_camera->draw(m_DD);
};
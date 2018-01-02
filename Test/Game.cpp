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


Game::Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance)
{
	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);

	// Seed the random number generator
	srand((UINT)time(NULL));

	m_hWnd = _hWnd;

	m_game_data = new GameData;

	// Init render system for VBGOs
	VBGO::init(_pd3dDevice);

	// Find how big my window is to correctly calculate my aspect ratio
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	float AR = (float)width / (float)height;

	screen_width = width;
	screen_height = height;

	input_handler = std::make_unique <InputHandler>(_hWnd, _hInstance);

	// Set GameData struct and populate its pointers
	m_game_data->m_keyboard_state = &input_handler->getKeyboardState();
	m_game_data->m_prev_keyboard_state = &input_handler->getPreviousKeyboardState();
	m_game_data->m_mouse_state = &input_handler->getMouseState();
	m_game_data->play = true;

	bot_cam = new SwarmBot();
	bot_cam->init(_pd3dDevice);

	m_camera = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, bot_cam,
		DirectX::Vector3Up, XMFLOAT3{0.0f, 0.0f, -500.0f});
	
	//create DrawData struct and populate its pointers
	m_draw_data = new DrawData;
	m_draw_data->m_pd3dImmediateContext = nullptr;
	m_draw_data->m_cam = m_camera;

	swarm_manager = std::make_unique<SwarmManager>(_pd3dDevice, 40000);

	bot_cam->setPos({ swarm_manager->getZoneCenter(), swarm_manager->getZoneCenter(), 0.0f });

	//lock the cursor to the centre of the window
	RECT window;

	GetWindowRect(m_hWnd, &window);

	SetCursorPos((window.left + window.right) >> 1,
		(window.bottom + window.top) >> 1);
};



Game::~Game()
{
	//delete Game Data & Draw Data
	delete m_game_data;
	delete m_draw_data;

	delete bot;
	delete bot_cam;

	//tidy up VBGO render system
	VBGO::cleanUp();
};



bool Game::tick()
{
	//Poll Keyboard
	input_handler->readKeyboard();

	// Poll Mouse
	input_handler->readMouse();

	if (input_handler->tick(m_game_data, m_camera) == false)
	{
		// if False exit the application
		return false;
	}

	m_camera->tick(m_game_data);

	if (m_game_data->play)
	{
		swarm_manager->tick(m_game_data);
	}

	//calculate frame time-step dt for passing down to game objects
	DWORD current_time = GetTickCount();
	m_game_data->m_dt = min((float)(current_time - m_play_time) / 1000.0f, 0.1f);
	m_play_time = current_time;

	return true;
}



void Game::draw(ID3D11DeviceContext* _pd3dImmediateContext)
{
	//set immediate context of the graphics device
	m_draw_data->m_pd3dImmediateContext = _pd3dImmediateContext;

	m_draw_data->m_cam = m_camera;

	//update the constant buffer for the rendering of VBGOs
	VBGO::updateConstantBuffer(m_draw_data);

	swarm_manager->draw(m_draw_data);

	m_camera->draw(m_draw_data);
};
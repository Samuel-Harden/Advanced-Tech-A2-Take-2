#include "Game.h"
//DXTK headers

//system headers
#include <windows.h>
#include <time.h>
#include <iostream>

//our headers
#include "GameData.h"
#include "DrawData.h"

#include "SwarmBot.h"
#include "TPSCamera.h"
#include "Light.h"


Game::Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance)
{
	// Create DirectXTK spritebatch stuff
	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);


	// Seed the random number generator
	srand((UINT)time(NULL));

	// Direct Input Stuff
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

	//create a base light
	m_light = new Light(XMFLOAT3(220.0f, 220.0f, 220.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.4f, 0.1f, 0.1f, 1.0f));

	bot_cam = new SwarmBot();
	bot_cam->init(_pd3dDevice);
	bot_cam->setPos({0.0f, 0.0f, 0.0f});

	bot = new SwarmBot();
	bot->init(_pd3dDevice);
	bot->setPos({ 5.0f, 5.0f, 0.0f });

	m_camera = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, bot_cam,
		DirectX::Vector3Up, XMFLOAT3{0.0f, 5.0f, -60.0f});
	
	//create DrawData struct and populate its pointers
	m_DD = new DrawData;
	m_DD->m_pd3dImmediateContext = nullptr;
	m_DD->m_cam = m_camera;
	m_DD->m_light = m_light;

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
	m_camera->tick(m_GD);
	m_light->tick(m_GD);

	bot_cam->tick();
	bot->tick();

	// Testing Movement...
	XMFLOAT3 pos = bot_cam->getPos();

	if (pos.x > 12.0f)
	{
		pos.x = -12.0f;
	}
	pos.x += 0.001f;
	bot_cam->setPos(pos);
	// End Testing...

	//Poll Keyboard
	//m_inputHandler->readKeyboard();

	// Poll Mouse
	//m_inputHandler->readMouse();

	/*
	//calculate frame time-step dt for passing down to game objects
	DWORD currentTime = GetTickCount();
	m_GD->m_dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;
	*/
	return true;
}



void Game::Draw(ID3D11DeviceContext* _pd3dImmediateContext)
{
	//set immediate context of the graphics device
	m_DD->m_pd3dImmediateContext = _pd3dImmediateContext;

	//update the constant buffer for the rendering of VBGOs
	VBGO::UpdateConstantBuffer(m_DD);

	bot->draw(m_DD);
	bot_cam->draw(m_DD);


	m_light->draw(m_DD);
	m_camera->draw(m_DD);
};
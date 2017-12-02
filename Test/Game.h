#pragma once

#include <d3d11.h>
#include <windows.h>
#include <list>
#include "Effects.h"


using std::list;
//using std::unique_ptr;
//using namespace DirectX;

class SwarmBot;
class TPSCamera;
class Light;

struct GameData;
struct DrawData;

class Game
{
public:

	Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance);
	virtual ~Game();

	bool Tick(); //tick the game state

	void Draw(ID3D11DeviceContext* _pd3dImmediateContext); //render the current game state

protected:
	DWORD m_playTime; //amount of time since the game started

	HWND m_hWnd;

	GameData* m_GD;			//Data to be shared to all Game Objects as they are ticked
	DrawData* m_DD;			//Data to be shared to all Game Objects as they are drawn
	
	//Tick functions for each state
	//void PlayTick();

	SwarmBot* bot_cam;
	SwarmBot* bot;
	TPSCamera* m_camera;
	Light* m_light;

private:

	float screenWidth;
	float screenHeight;
};
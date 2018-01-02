#pragma once

#include <d3d11.h>
#include <windows.h>
#include <list>
#include <memory>
#include "FrameTimer.h"

class SwarmBot;
class SwarmManager;
class TPSCamera;
class InputHandler;
class FrameTimer;

struct GameData;
struct DrawData;


class Game
{
public:

	Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance);
	virtual ~Game();

	bool tick(); //tick the game state

	void draw(ID3D11DeviceContext* _pd3dImmediateContext); //render the current game state

protected:
	DWORD m_play_time; //amount of time since the game started

	HWND m_hWnd;

	GameData* m_game_data;			//Data to be shared to all Game Objects as they are ticked
	DrawData* m_draw_data;			//Data to be shared to all Game Objects as they are drawn
	
	std::unique_ptr<InputHandler> input_handler;

	std::unique_ptr<SwarmManager> swarm_manager;

	SwarmBot* bot_cam;
	SwarmBot* bot;
	TPSCamera* m_camera;

private:

	FrameTimer ft;

	float screen_width;
	float screen_height;
};
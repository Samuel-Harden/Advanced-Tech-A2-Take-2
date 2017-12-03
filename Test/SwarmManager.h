#pragma once

#include <d3d11_1.h>
#include<DirectXMath.h>

#include <vector>
#include <memory>

struct GameData;
struct DrawData;

class SwarmBot;
class SwarmBotData;
class Behaviour;

class SwarmManager
{
public:
	SwarmManager(ID3D11Device* _pd3dDevice, int _max_bots);
	~SwarmManager();

	void Tick(GameData* _game_data);
	void Draw(DrawData* _draw_data);

protected:

private:

	void GenerateBots(ID3D11Device* _pd3dDevice, const int& _max_bots);
	void GenerateBehaviours();
	void GenerateWaypoints();
	void GenerateBotData();

	void UpdateBots();

	std::vector<SwarmBot*> swarm_bots;

	SwarmBotData* swarm_data;

	std::vector<Behaviour*> swarm_behaviours;

	std::vector<DirectX::XMFLOAT3> waypoints;

	SwarmBot* bot;

	int no_waypoints;

};
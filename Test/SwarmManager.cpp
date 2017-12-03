#include "SwarmManager.h"

#include "SwarmBot.h"
#include "GameData.h"
#include "DrawData.h"
#include "SwarmBotData.h"

#include "Behaviour.h"
#include "Separation.h"
#include "PathFinding.h"


SwarmManager::SwarmManager(ID3D11Device* _pd3dDevice, int _max_bots)
{
	swarm_bots.reserve(_max_bots);
	waypoints.reserve(4);

	GenerateWaypoints();

	GenerateBots(_pd3dDevice, _max_bots);

	GenerateBehaviours();
	GenerateBotData();
}


SwarmManager::~SwarmManager()
{

}


void SwarmManager::Tick(GameData* _game_data)
{
	UpdateBots();
}


void SwarmManager::Draw(DrawData* _draw_data)
{
	for (int i = 0; i < swarm_bots.size(); i++)
	{
		swarm_bots[i]->draw(_draw_data);
	}
}


void SwarmManager::GenerateBots(ID3D11Device* _pd3dDevice, const int& _max_bots)
{
	for (int i = 0; i < _max_bots; i++)
	{
		bot = new SwarmBot();
		bot->init(_pd3dDevice);
		swarm_bots.push_back(bot);
	}
}


void SwarmManager::GenerateBehaviours()
{
	Separation* sep = new Separation();
	swarm_behaviours.push_back(sep);

	PathFinding* path = new PathFinding();
	swarm_behaviours.push_back(path);
}


void SwarmManager::GenerateBotData()
{
	swarm_data = new SwarmBotData();

	swarm_data->desiredSep = 2.5f;
	swarm_data->bot_max_force = 0.02f;
	swarm_data->bot_max_speed = 0.5f;
	swarm_data->neighbourDist = 10.0f;
	swarm_data->pathWeight = -0.5f;
	swarm_data->sepWeight = 1.0f;
}


void SwarmManager::GenerateWaypoints()
{
	XMFLOAT3 pos_1 = {-50.0f, -50.0f, 0.0f};
	waypoints.push_back(pos_1);

	XMFLOAT3 pos_2 = { 50.0f, -50.0f, 0.0f };
	waypoints.push_back(pos_2);

	XMFLOAT3 pos_3 = { -50.0f, 50.0f, 0.0f };
	waypoints.push_back(pos_3);

	XMFLOAT3 pos_4 = { 50.0f, 50.0f, 0.0f };
	waypoints.push_back(pos_4);
}


void SwarmManager::UpdateBots()
{
	for (int i = 0; i < swarm_bots.size(); i++)
	{
		swarm_bots[i]->run(swarm_bots, swarm_data, swarm_behaviours, waypoints);
	}

	for (int i = 0; i < swarm_bots.size(); i++)
	{
		swarm_bots[i]->tick(swarm_data);
	}
}
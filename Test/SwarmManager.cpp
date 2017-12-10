#include "SwarmManager.h"

#include <iostream>

#include "SwarmBot.h"
#include "GameData.h"
#include "DrawData.h"
#include "SwarmBotData.h"
#include "Zone.h"

#include "Behaviour.h"
#include "Separation.h"
#include "PathFinding.h"


SwarmManager::SwarmManager(ID3D11Device* _pd3dDevice, int _max_bots) :
	no_zones(36),
	grid_width(6),
	grid_height(6),
	current_zone(0)
{
	zones.reserve(no_zones), // 7 x 7 grid
	waypoints.reserve(4);

	GenerateWaypoints(_max_bots);

	GenerateZones(_pd3dDevice, _max_bots);

	GenerateBehaviours();
	GenerateBotData();
}


SwarmManager::~SwarmManager()
{

}


void SwarmManager::Tick(GameData* _game_data)
{
	UpdateZones(current_zone);

	current_zone++;

	if (current_zone >= no_zones)
	{
		current_zone = 0;
	}
}


void SwarmManager::Draw(DrawData* _draw_data)
{
	for (int i = 0; i < zones.size(); i++)
	{
		zones[i]->Draw(_draw_data);
	}
}


void SwarmManager::GenerateZones(ID3D11Device* _pd3dDevice, const int& _max_bots)
{
	XMFLOAT2 pos = Vector2Zero;

	int no_bots = _max_bots / no_zones;

	int zone_ID = 0;

	float zone_size_x = max_area / grid_width;
	float zone_size_y = max_area / grid_height;

	XMFLOAT2 size = { zone_size_x, zone_size_y };

	// Populate each zone
	for (int i = 0; i < no_zones; i++)
	{
		zone = new Zone(_pd3dDevice, zone_ID, pos, size, no_bots);

		zones.push_back(zone);
		zone_ID++;
		pos.x += zone_size_x;

		if (pos.x >= max_area)
		{
			pos.x = 0.0f;
			pos.y += zone_size_y;
		}
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

	swarm_data->desiredSep    =  2.5f;
	swarm_data->bot_max_force = 0.02f;
	swarm_data->bot_max_speed =  1.5f;
	swarm_data->neighbourDist = 10.0f;
	swarm_data->pathWeight    = -0.5f;
	swarm_data->sepWeight     =  10.0f;
}


void SwarmManager::GenerateWaypoints(int _max_bots)
{
	max_area = _max_bots / 40;

	// Create a 20% offset from the edge of the grid
	float min_pos = max_area / 5;
	float max_pos = max_area / 5 * 4;

	XMFLOAT3 pos_1 = { min_pos, min_pos, 0.0f}; // Bottom Left
	waypoints.push_back(pos_1);

	XMFLOAT3 pos_2 = { max_pos, max_pos, 0.0f };    // Top Right
	waypoints.push_back(pos_2);

	XMFLOAT3 pos_3 = { min_pos, max_pos, 0.0f };    // Top left
	waypoints.push_back(pos_3);

	XMFLOAT3 pos_4 = { max_pos, min_pos, 0.0f };    // Bottom Right
	waypoints.push_back(pos_4);
}


// needs updating to cycle through the zones
void SwarmManager::UpdateZones(int _zone)
{
	std::vector<int> update_zones;

	SetZonesForUpdate(_zone, update_zones);

	// Update this zone
	zones[current_zone]->Run(swarm_data, swarm_behaviours, waypoints);

	// Uodate surrounding zones
	for (int i = 0; i < update_zones.size(); i++)
	{
		zones[current_zone]->Run(zones[update_zones[i]]->GetSwarm(), swarm_data, swarm_behaviours, waypoints);
	}


	for (int i = 0; i < zones.size(); i++)
	{
		zones[i]->Tick(swarm_data);
	}
}


void SwarmManager::SetZonesForUpdate(int _zone, std::vector<int> _update_zones)
{
	// Get Zones surrounding the current zone

}


// Move bots to new zone if needed...
void SwarmManager::UpdateBotPositions(int _zone)
{
	DirectX::XMFLOAT2 zone_pos = zones[_zone]->GetPos();
	DirectX::XMFLOAT2 zone_size = zones[_zone]->GetSize();

	// loop through all bots
	for (int i = 0; i < zones[_zone]->GetSwarm().size(); i++)
	{
		//check if there is a bot in this pos
		if (zones[_zone]->GetBot(i) != nullptr)
		{
			XMFLOAT3 bot_pos = zones[_zone]->GetBot(i)->GetPos();

			// corners first

			// is bot in the Zone DOWN and LEFT
			if (bot_pos.x < zone_pos.x && bot_pos.y < zone_pos.y)
			{
				// Change Zone
				std::cout << "im Down and Left" << std::endl;

				return;
			}

			// is bot in the Zone DOWN and RIGHT
			if (bot_pos.x >(zone_pos.x + zone_size.x) && bot_pos.y < zone_pos.y)
			{
				// Change Zone
				std::cout << "im Down and Right" << std::endl;

				return;
			}

			// is bot in the Zone UP and LEFT
			if (bot_pos.x < zone_pos.x && bot_pos.y >(zone_pos.y + zone_size.y))
			{
				// Change Zone
				std::cout << "im Up and Left" << std::endl;

				return;
			}

			// is bot in the Zone UP and RIGHT
			if (bot_pos.x > (zone_pos.x + zone_size.x) && bot_pos.y > (zone_pos.y + zone_size.y))
			{
				// Change Zone
				std::cout << "im Up and Right" << std::endl;

				return;
			}

			// is bot in the LEFT
			if (bot_pos.x < zone_pos.x)
			{
				// Change Zone
				std::cout << "im Left" << std::endl;

				return;
			}

			// is bot in the RIGHT
			if (bot_pos.x >(zone_pos.x + zone_size.x))
			{
				// Change Zone
				std::cout << "im Right" << std::endl;

				return;
			}

			// is bot in the Zone DOWN
			if (bot_pos.y < zone_pos.y)
			{
				// Change Zone
				std::cout << "im Down" << std::endl;

				return;
			}

			// is bot in the Zone DOWN
			if (bot_pos.y >(zone_pos.y + zone_size.y))
			{
				// Change Zone
				std::cout << "im Up" << std::endl;

				return;
			}

			// if none are true then the bots in the correct zone
		}
	}
}


float SwarmManager::GetZoneCenter()
{
	return max_area / 2;
}
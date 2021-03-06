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


SwarmManager::SwarmManager(ID3D11Device* _pd3dDevice) :
	max_bots(10000),
	no_zones(12),
	grid_width(3),
	grid_height(4),
	current_zone(0)
{
	m_zones.reserve(no_zones),
	waypoints.reserve(4);

	generateWaypoints();

	generateZones(_pd3dDevice);

	generateBehaviours();
	generateBotData();
}


SwarmManager::~SwarmManager()
{
	delete swarm_data;
	swarm_data = nullptr;

	// Delete Zones
	for (std::vector<Zone*>::iterator it = m_zones.begin();
		it != m_zones.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}

	m_zones.clear();
}


void SwarmManager::tick(GameData* _game_data)
{
	updateZones(current_zone, _game_data);

	current_zone++;

	if (current_zone == no_zones)
	{
		current_zone = 0;
	}
}


void SwarmManager::draw(DrawData* _draw_data)
{
	for (int i = 0; i < m_zones.size(); i++)
	{
		m_zones[i]->draw(_draw_data);
	}
}


void SwarmManager::generateZones(ID3D11Device* _pd3dDevice)
{
	XMFLOAT2 pos = Vector2Zero;

	int no_bots = max_bots / no_zones;

	int zone_ID = 0;
	int row = 0;
	int col = 0;

	zone_width = max_area / grid_width;
	zone_height = max_area / grid_height;

	XMFLOAT2 size = { zone_width, zone_height };

	// Populate each zone
	for (int i = 0; i < no_zones; i++)
	{
		zone = new Zone(_pd3dDevice, zone_ID, pos, size, row, col, no_bots);

		m_zones.push_back(zone);
		zone_ID++;
		pos.x += zone_width;
		col++;

		if (pos.x >= max_area)
		{
			pos.x = 0.0f;
			pos.y += zone_height;
			col = 0;
			row++;
		}
	}
}


void SwarmManager::generateBehaviours()
{
	Separation* sep = new Separation();
	swarm_behaviours.push_back(sep);

	PathFinding* path = new PathFinding();
	swarm_behaviours.push_back(path);
}


void SwarmManager::generateBotData()
{
	swarm_data = new SwarmBotData();

	swarm_data->desired_separation    =  5.0f;
	swarm_data->bot_max_force         = 0.02f;
	swarm_data->bot_max_speed         =  0.6f;
	swarm_data->neighbour_distance    = 20.0f;
	swarm_data->path_weight           = -1.0f;
	swarm_data->sep_weight            =  10.0f;
}


void SwarmManager::generateWaypoints()
{
	max_area = max_bots / 10;

	// Create a 20% offset from the edge of the grid
	float min_pos = max_area / 5;
	float max_pos = max_area / 5 * 4;

	XMFLOAT3 pos_1 = { min_pos, min_pos, 0.0f };    // Bottom Left
	waypoints.push_back(pos_1);

	XMFLOAT3 pos_2 = { max_pos, max_pos, 0.0f };    // Top Right
	waypoints.push_back(pos_2);

	XMFLOAT3 pos_3 = { min_pos, max_pos, 0.0f };    // Top left
	waypoints.push_back(pos_3);

	XMFLOAT3 pos_4 = { max_pos, min_pos, 0.0f };    // Bottom Right
	waypoints.push_back(pos_4);
}


// needs updating to cycle through the zones
void SwarmManager::updateZones(int _zone, GameData* _game_data)
{
	std::vector<int> update_zones;

	setZonesForUpdate(_zone, update_zones);

	// Update zone
	m_zones[current_zone]->run(swarm_data, swarm_behaviours, waypoints);

	/*// Uodate surrounding zones
	for (int i = 0; i < update_zones.size(); i++)
	{
		zones[current_zone]->Run(zones[update_zones[i]]->GetSwarm(), swarm_data, swarm_behaviours, waypoints);
	}*/

	for (int i = 0; i < m_zones.size(); i++)
	{
		m_zones[i]->tick(swarm_data, _game_data);
	}

	//updateBotPositions(_zone);
	updateBotPositions2(_zone);
}


void SwarmManager::setZonesForUpdate(int _zone, std::vector<int> _update_zones)
{
	// Get Zones surrounding the current zone

}


// OLD FUNCTION using 'updateBotPos2' now (more Accurate)
// Move bots to new zone if needed...
void SwarmManager::updateBotPositions(int _zone)
{
	DirectX::XMFLOAT2 zone_pos = m_zones[_zone]->getPos();
	DirectX::XMFLOAT2 zone_size = m_zones[_zone]->getSize();

	// loop through all bots
	for (int i = 0; i < m_zones[_zone]->getSwarm().size(); i++)
	{
		//check if there is a bot in this pos
		if (m_zones[_zone]->getBot(i) != nullptr)
		{
			XMFLOAT3 bot_pos = m_zones[_zone]->getBot(i)->getPos();

			// corners first

			// is bot in the Zone DOWN and LEFT
			if (bot_pos.x < zone_pos.x && bot_pos.y < zone_pos.y)
			{
				// Change Zone
				std::cout << "im Down and Left" << std::endl;

				int new_zone = _zone - 1 - grid_width; // (move Left, Down a row)

				if(!zoneValidation(new_zone))
					continue;

				moveBot(i, _zone, new_zone);

				return;
			}

			// is bot in the Zone DOWN and RIGHT
			if (bot_pos.x > (zone_pos.x + zone_size.x) && bot_pos.y < zone_pos.y)
			{
				// Change Zone
				std::cout << "im Down and Right" << std::endl;

				int new_zone = _zone + 1 - grid_width; // (move Right, Down a row)

				if (!zoneValidation(new_zone))
					continue;

				moveBot(i, _zone, new_zone);

				return;
			}

			// is bot in the Zone UP and LEFT
			if (bot_pos.x < zone_pos.x && bot_pos.y > (zone_pos.y + zone_size.y))
			{
				// Change Zone
				std::cout << "im Up and Left" << std::endl;

				int new_zone = _zone - 1 + grid_width; // (move Left, Up a Row)

				if (!zoneValidation(new_zone))
					continue;

				moveBot(i, _zone, new_zone);

				return;
			}

			// is bot in the Zone UP and RIGHT
			if (bot_pos.x > (zone_pos.x + zone_size.x) && bot_pos.y > (zone_pos.y + zone_size.y))
			{
				// Change Zone
				std::cout << "im Up and Right" << std::endl;

				int new_zone = _zone + 1 + grid_width; // (move Right, Up a Row)

				if (!zoneValidation(new_zone))
					continue;

				moveBot(i, _zone, new_zone);

				return;
			}

			// is bot in the LEFT
			if (bot_pos.x < zone_pos.x)
			{
				// Change Zone
				std::cout << "im Left" << std::endl;

				int new_zone = _zone - 1 ; // (move Left)

				if (!zoneValidation(new_zone))
					continue;

				moveBot(i, _zone, new_zone);

				return;
			}

			// is bot in the RIGHT
			if (bot_pos.x >(zone_pos.x + zone_size.x))
			{
				// Change Zone
				std::cout << "im Right" << std::endl;

				int new_zone = _zone + 1; // (move Right)

				if (!zoneValidation(new_zone))
					continue;

				moveBot(i, _zone, new_zone);

				return;
			}

			// is bot in the Zone DOWN
			if (bot_pos.y < zone_pos.y)
			{
				// Change Zone
				std::cout << "im Down" << std::endl;

				int new_zone = _zone - grid_width; // (Down a Row)

				if (!zoneValidation(new_zone))
					continue;

				moveBot(i, _zone, new_zone);

				return;
			}

			// is bot in the Zone DOWN
			if (bot_pos.y >(zone_pos.y + zone_size.y))
			{
				// Change Zone
				std::cout << "im Up" << std::endl;

				int new_zone = _zone + grid_width; // (Up a Row)

				if (!zoneValidation(new_zone))
					continue;

				moveBot(i, _zone, new_zone);

				return;
			}
		}
	}
}


// Updated function, way more accurate!
void SwarmManager::updateBotPositions2(int _zone)
{
	// loop through all bots
	for (int i = 0; i < m_zones[_zone]->getSwarm().size(); i++)
	{
		//check if there is a bot in this pos
		if (m_zones[_zone]->getBot(i) != nullptr)
		{
			XMFLOAT2 zone_pos = m_zones[_zone]->getPos();
			XMFLOAT3 bot_pos  = m_zones[_zone]->getBot(i)->getPos();

			// If im already in the correct zone SKIP
			if (bot_pos.x >= zone_pos.x && bot_pos.x < zone_pos.x + zone_width &&
				bot_pos.y >= zone_pos.y && bot_pos.y < zone_pos.y + zone_height)
			{
				goto skip;
			}

			// if bot is in bounds of entire Grid
			// If its not, its still part of x zone, so will still
			// be updated and move back in bounds towards a waypoint
			if (bot_pos.x >= 0 && bot_pos.x <(zone_width * grid_width) &&
				bot_pos.y >= 0 && bot_pos.y <(zone_height * grid_height))
			{
				bool row_found = false;
				bool col_found = false;

				int row = 0;
				int col = 0;

				float row_pos = 0.0f;
				float col_pos = 0.0f;

				// Find the Row and Column of new Zone
				while (!col_found)
				{
					if (bot_pos.x >= col_pos && bot_pos.x < col_pos + zone_width)
					{
						col_found = true;
						break;
					}

					col_pos += zone_width;
					col++;
				}

				while (!row_found)
				{
					if (bot_pos.y >= row_pos && bot_pos.y < row_pos + zone_height)
					{
						row_found = true;
						break;
					}

					row_pos += zone_height;
					row++;
				}

				int new_zone = grid_width * row + col;

				// If bot has moved, update the zones
				if (new_zone != _zone)
				{
					moveBot(i, _zone, new_zone);
				}

				skip:
				{
					// do nothing, already in correct Zone!
				}
			}
		}
	}
}


float SwarmManager::getZoneCenter() const
{
	return max_area / 2;
}


bool SwarmManager::zoneValidation(int _zone)
{
	if (_zone >= 0 && _zone < (grid_width * grid_height))
		return true;

	else 
		return false;
}


void SwarmManager::moveBot(int _bot, int _zone, int _new_zone)
{
	m_zones[_new_zone]->getSwarm().push_back(std::move(m_zones[_zone]->getBot(_bot))); // Add bot to new Zone

	m_zones[_zone]->getSwarm().erase(m_zones[_zone]->getSwarm().begin() + _bot); // Cleanup old Zone
}
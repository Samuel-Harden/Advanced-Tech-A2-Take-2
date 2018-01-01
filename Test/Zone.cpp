#include "Zone.h"

#include "SwarmBot.h"


Zone::Zone(ID3D11Device* _pd3dDevice, int _zone_ID, XMFLOAT2 _pos, XMFLOAT2 _size, int _row, int _col, int _no_bots) :
	zone_ID(_zone_ID),
	zone_position(_pos),
	zone_size(_size),
	zone_row(_row),
	zone_column(_col)
{
	swarm_bots.reserve(_no_bots * 2);

	GenerateBots(_pd3dDevice, _no_bots);
}


Zone::~Zone()
{

}


void Zone::GenerateBots(ID3D11Device* _pd3dDevice, int no_bots)
{
	XMFLOAT2 min_pos = {zone_position.x, zone_position.y};
	XMFLOAT2 max_pos = {zone_position.x + zone_size.x, zone_position.y + zone_size.y};

	int waypoint = 0;

	for (int j = 0; j < no_bots; j++)
	{
		bot = new SwarmBot(min_pos, max_pos, waypoint);
		bot->Init(_pd3dDevice);
		swarm_bots.push_back(bot);

		waypoint++;

		if (waypoint == 4)
		{
			waypoint = 0;
		}
	}
}


void Zone::Tick(SwarmBotData* _SBD, GameData* _game_data)
{
	for (int i = 0; i < swarm_bots.size(); i++)
	{
		swarm_bots[i]->Tick(_SBD, _game_data);
	}
}


void Zone::Run(SwarmBotData* _swarm_data, std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _waypoints)
{
	for (int i = 0; i < swarm_bots.size(); i++)
	{
		swarm_bots[i]->Run(swarm_bots, _swarm_data, _behaviours, _waypoints);
	}
}


void Zone::Run(std::vector<SwarmBot*>& _bots, SwarmBotData* _swarm_data, std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _waypoints)
{
	std::vector<SwarmBot*> swarm = _bots;




	for (int i = 0; i < swarm_bots.size(); i++)
	{
		swarm_bots[i]->Run(swarm_bots, _swarm_data, _behaviours, _waypoints);
	}
}


void Zone::Draw(DrawData* _draw_data)
{
	for (int i = 0; i < swarm_bots.size(); i++)
	{
		swarm_bots[i]->Draw(_draw_data);
	}
}


void Zone::UpdateZone()
{

}


XMFLOAT2 Zone::GetPos()
{
	return zone_position;
}


XMFLOAT2 Zone::GetSize()
{
	return zone_size;
}


std::vector<SwarmBot*>& Zone::GetSwarm()
{
	return swarm_bots;
}


SwarmBot* Zone::GetBot(int _bot)
{
	return swarm_bots[_bot];
}
#include "Zone.h"

#include "SwarmBot.h"


Zone::Zone(ID3D11Device* _pd3dDevice, int _zone_ID, XMFLOAT2 _pos, XMFLOAT2 _size, int _no_bots) :
	zone_ID(_zone_ID),
	zone_position(_pos),
	zone_size(_size)
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

	for (int j = 0; j < no_bots; j++)
	{
		bot = new SwarmBot(min_pos, max_pos);
		bot->Init(_pd3dDevice);
		swarm_bots.push_back(bot);
	}
}


void Zone::Tick(SwarmBotData* _SBD)
{
	for (int i = 0; i < swarm_bots.size(); i++)
	{
		swarm_bots[i]->Tick(_SBD);
	}

	// Update all bots to new Zones if needed
	//UpdateZone();
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


std::vector<SwarmBot*> Zone::GetSwarm()
{
	return swarm_bots;
}


SwarmBot* Zone::GetBot(int _bot)
{
	return swarm_bots[_bot];
}
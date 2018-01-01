#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>

struct DrawData;
struct GameData;

class SwarmBot;
class SwarmBotData;
class Behaviour;

using namespace DirectX;

class Zone
{
public:
	Zone(ID3D11Device* _pd3dDevice, int _zone_ID, XMFLOAT2 _pos, XMFLOAT2 _size, int _row, int _col, int _no_bots);
	~Zone();

	void Tick(SwarmBotData* _SBD, GameData* _game_data);

	void Run(SwarmBotData* _swarm_data,
		std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _waypoints);

	void Run(std::vector<SwarmBot*>& _bots, SwarmBotData* _swarm_data,
		std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _waypoints);
	void Draw(DrawData* _draw_data);

	void UpdateZone();

	XMFLOAT2 GetPos();
	XMFLOAT2 GetSize();

	SwarmBot* GetBot(int _bot);

	std::vector<SwarmBot*>& GetSwarm();

protected:

private:

	void GenerateBots(ID3D11Device* _pd3dDevice, int no_bots);

	SwarmBot* bot;

	std::vector<SwarmBot*> swarm_bots;

	int zone_ID;
	int zone_row;
	int zone_column;
	int no_bots;
	XMFLOAT2 zone_position;
	XMFLOAT2 zone_size;

};
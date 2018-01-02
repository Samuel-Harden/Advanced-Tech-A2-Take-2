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

	void tick(SwarmBotData* _SBD, GameData* _game_data);

	void run(SwarmBotData* _swarm_data,
		std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _waypoints);

	void run(std::vector<SwarmBot*>& _bots, SwarmBotData* _swarm_data,
		std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _waypoints);

	void draw(DrawData* _draw_data);

	XMFLOAT2 getPos() const;
	XMFLOAT2 getSize() const;

	SwarmBot* getBot(int _bot);

	std::vector<SwarmBot*>& getSwarm();

protected:

private:

	void generateBots(ID3D11Device* _pd3dDevice, int no_bots);

	SwarmBot* bot;

	std::vector<SwarmBot*> swarm_bots;

	int zone_ID;
	int zone_row;
	int zone_column;
	int no_bots;
	XMFLOAT2 zone_position;
	XMFLOAT2 zone_size;

};
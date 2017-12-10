#pragma once

#include <d3d11_1.h>
#include<DirectXMath.h>

#include <vector>
#include <memory>

struct GameData;
struct DrawData;

class Zone;
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

	float GetZoneCenter();

protected:

private:

	void GenerateZones(ID3D11Device* _pd3dDevice, const int& _max_bots);
	void GenerateBehaviours();
	void GenerateWaypoints(int _max_bots);
	void GenerateBotData();
	void UpdateZones(int _zone);

	void UpdateBotPositions(int _zone);

	SwarmBotData* swarm_data;

	std::vector<Behaviour*> swarm_behaviours;

	std::vector<DirectX::XMFLOAT3> waypoints;

	Zone* zone;

	// Waypoint positions
	float max_area;

	int no_waypoints;

	// Vector containing vectors(bots in each zone)
	std::vector<Zone*> zones;
	int no_zones;
	int current_zone;

	int grid_width;
	int grid_height;

	// the aim is too keep zomes below 250 (extra 250 as safety net)
	const int max_bots_zone = 500;
};
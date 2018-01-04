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
	SwarmManager(ID3D11Device* _pd3dDevice);
	~SwarmManager();

	void tick(GameData* _game_data);
	void draw(DrawData* _draw_data);

	float getZoneCenter() const;

protected:

private:

	void generateZones(ID3D11Device* _pd3dDevice);
	void generateBehaviours();
	void generateWaypoints();
	void generateBotData();
	void updateZones(int _zone, GameData* _game_data);

	void updateBotPositions(int _zone);
	void updateBotPositions2(int _zone);
	void setZonesForUpdate(int _zone, std::vector<int> _update_zones);

	bool zoneValidation(int _zone);
	void moveBot(int i, int _zone, int _new_zone);

	SwarmBotData* swarm_data;

	std::vector<Behaviour*> swarm_behaviours;

	std::vector<DirectX::XMFLOAT3> waypoints;

	Zone* zone;

	// Waypoint positions
	float max_area;

	int no_waypoints;

	// Vector containing vectors(bots in each zone)
	std::vector<Zone*> m_zones;
	int no_zones;
	int current_zone;

	// No of zones (x & y)
	int grid_width;
	int grid_height;

	// actual size of a zone
	float zone_width;
	float zone_height;

	int max_bots;
};
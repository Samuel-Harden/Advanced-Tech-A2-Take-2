#pragma once

#include <vector>

#include "VBGO.h"
#include "vertex.h"

class Behaviour;
class SwarmBotData;
class PositionCheck;


class SwarmBot : public VBGO
{
public:
	SwarmBot();
	SwarmBot(XMFLOAT2 _min_pos, XMFLOAT2 _max_pos, float _waypoint);
	virtual ~SwarmBot();

	//initialise the Veretx and Index buffers for the Boid
	void init(ID3D11Device* _GD);

	void tick(SwarmBotData* _SBD, GameData* _game_data);

	void run(std::vector<SwarmBot*>& _bots, SwarmBotData* _swarm_data,
		std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _waypoint);

	bool getIsActive() const;
	int getWayPointID() const;
	DirectX::XMFLOAT3 getVelocity() const;

	void setWayPointID(int& _newWayPoint);

protected:

	int m_size;
	myVertex* m_vertices;

private:

	void applyForce(XMFLOAT3& force);

	// Variables
	bool new_pos;

	bool is_active;
	int waypoint_ID;

	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
};
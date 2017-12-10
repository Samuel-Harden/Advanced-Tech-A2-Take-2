#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>

class Behaviour;
class SwarmBotData;
class PositionCheck;



class SwarmBot : public VBGO
{
public:
	SwarmBot();
	SwarmBot(XMFLOAT2 _min_pos, XMFLOAT2 _max_pos);
	virtual ~SwarmBot();

	//initialise the Veretx and Index buffers for the Boid
	void Init(ID3D11Device* _GD);

	void Tick(SwarmBotData* _SBD);
	void Run(std::vector<SwarmBot*>& _bots, SwarmBotData* _swarm_data, std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _waypoint);

	bool GetIsActive();

	DirectX::XMFLOAT3 GetVelocity() const;

	void SetWayPointID(int& _newWayPoint);
	int GetWayPointID() const;

protected:

	int m_size;
	myVertex* m_vertices;

private:

	void SetRandPos(XMFLOAT2 _min, XMFLOAT2 _max);
	float RandomFloat(float _min, float _max);

	void ApplyForce(XMFLOAT3& force);

	// Variables
	bool newPos;

	bool is_active;
	int wayPointID;

	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
};
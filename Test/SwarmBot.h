#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>

class Behaviour;
class PositionCheck;



class SwarmBot : public VBGO
{
public:
	SwarmBot();
	virtual ~SwarmBot();

	//initialise the Veretx and Index buffers for the Boid
	void init(ID3D11Device* _GD);

	void tick();
	void run();

	DirectX::XMVECTOR getVelocity() const;

protected:

	int m_size;
	myVertex* m_vertices;

private:

	// Variables
	bool newPos;

	DirectX::XMVECTOR velocity;
	DirectX::XMVECTOR acceleration;
};
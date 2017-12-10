#pragma once

#include "Behaviour.h"

class PathFinding : public Behaviour
{
public:

	friend class SwarmManager;
	friend class SwarmBot;

	~PathFinding();

protected:
	PathFinding();

	DirectX::XMFLOAT3 CalculateBehaviour3(SwarmBot* _bot, SwarmBotData* _SBD, std::vector<DirectX::XMFLOAT3>& _wpPos) override;

private:

};

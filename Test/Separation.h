#pragma once

#include "Behaviour.h"

class Separation : public Behaviour
{
public:

	friend class SwarmManager;
	friend class SwarmBot;

	~Separation();

protected:
	Separation();

	DirectX::XMFLOAT3 CalculateBehaviour1(SwarmBot* _bot, SwarmBotData* _BD, std::vector<SwarmBot*>& _bots) override;

private:

};
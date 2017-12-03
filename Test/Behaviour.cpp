#include "Behaviour.h"

#include"SwarmBot.h"
#include "SwarmBotData.h"


Behaviour::Behaviour()
{

}


Behaviour::~Behaviour()
{

}


DirectX::XMFLOAT3 Behaviour::calculateBehaviour1(SwarmBot* _bot, SwarmBotData* _SBD, std::vector<SwarmBot*>& _bots)
{
	// Default Value...
	return DirectX::Vector3Zero;
}


DirectX::XMFLOAT3 Behaviour::calculateBehaviour2(SwarmBot* _bot, SwarmBotData* _SBD)
{
	// Default Value...
	return DirectX::Vector3Zero;
}


DirectX::XMFLOAT3 Behaviour::calculateBehaviour3(SwarmBot* _boid, SwarmBotData* _SBD, std::vector<DirectX::XMFLOAT3>& _wpPos)
{
	// Default Value...
	return DirectX::Vector3Zero;
}
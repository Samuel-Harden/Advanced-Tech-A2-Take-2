#pragma once

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

class SwarmBotData;
class GameData;
class SwarmBot;

class Behaviour
{
public:
	friend class SwarmBot;
	friend class SwarmManager;

	virtual ~Behaviour();

protected:

	Behaviour();

	virtual DirectX::XMFLOAT3 calculateBehaviour1(SwarmBot* _bot, SwarmBotData* _SBD, std::vector<SwarmBot*>& _bots);

	virtual DirectX::XMFLOAT3 calculateBehaviour2(SwarmBot* _bot, SwarmBotData* _SBD);

	virtual DirectX::XMFLOAT3 calculateBehaviour3(SwarmBot* _bot, SwarmBotData* _SBD, std::vector<DirectX::XMFLOAT3>& _wpPos);

private:

};
#include "SwarmBotData.h"


SwarmBotData::SwarmBotData()
{

}


SwarmBotData::~SwarmBotData()
{

}


void SwarmBotData::SetSepWeight(float _weight)
{
	sepWeight = _weight;
}


void SwarmBotData::SetFFWeight(float _weight)
{
	ffWeight = _weight;
}


void SwarmBotData::SetAliWeight(float _weight)
{
	aliWeight = _weight;
}


void SwarmBotData::SetCohWeight(float _weight)
{
	cohWeight = _weight;
}


void SwarmBotData::SetRunWeight(float _weight)
{
	runWeight = _weight;
}


void SwarmBotData::SetPathWeight(float _weight)
{
	pathWeight = _weight;
}


void SwarmBotData::SetBotMaxSpeed(float _speed)
{
	bot_max_speed = _speed;
}


void SwarmBotData::SetMaxForce(float _force)
{
	bot_max_force = _force;
}


void SwarmBotData::SetNeighbourDist(float _dist)
{
	neighbourDist = _dist;
}


void SwarmBotData::SetDesiredSep(float _sep)
{
	desiredSep = _sep;
}
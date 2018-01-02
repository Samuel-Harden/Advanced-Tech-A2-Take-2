#include "SwarmBotData.h"


SwarmBotData::SwarmBotData()
{

}


SwarmBotData::~SwarmBotData()
{

}


void SwarmBotData::setSepWeight(float _weight)
{
	sep_weight = _weight;
}


void SwarmBotData::setPathWeight(float _weight)
{
	path_weight = _weight;
}


void SwarmBotData::setBotMaxSpeed(float _speed)
{
	bot_max_speed = _speed;
}


void SwarmBotData::setMaxForce(float _force)
{
	bot_max_force = _force;
}


void SwarmBotData::setNeighbourDist(float _dist)
{
	neighbour_distance = _dist;
}


void SwarmBotData::setDesiredSep(float _sep)
{
	desired_separation = _sep;
}
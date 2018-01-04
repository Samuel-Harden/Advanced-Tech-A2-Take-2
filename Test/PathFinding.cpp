#include "PathFinding.h"

#include "SwarmBot.h"
#include "SwarmBotData.h"
#include "GameData.h"


PathFinding::PathFinding()
{

}


PathFinding::~PathFinding()
{

}


XMFLOAT3 PathFinding::CalculateBehaviour2(SwarmBot* _bot, SwarmBotData* _SBD, std::vector<XMFLOAT3>& _wpPos)
{
	XMFLOAT3 steer = Vector3Zero;
	int count = 0;

	float d = Vector3Distance(_bot->getPos(), _wpPos[_bot->getWayPointID()]);

	// if Boid is further than x away from waypoint
	if (d > 100.0f)
	{
		XMFLOAT3 diff = Vector3Zero;
		// Calculate vector pointing away from neighbour
		diff.x = (_bot->getPos().x - (_wpPos[_bot->getWayPointID()].x));
		diff.y = (_bot->getPos().y - (_wpPos[_bot->getWayPointID()].y));
		diff.z = (_bot->getPos().z - (_wpPos[_bot->getWayPointID()].z));

		XMVECTOR diff_vector = XMLoadFloat3(&diff);

		XMVector3Normalize(diff_vector);

		XMStoreFloat3(&diff, diff_vector);

		diff.x = (diff.x / d); // Weight by distance
		diff.y = (diff.y / d);
		diff.z = (diff.z / d);

		steer.x = (steer.x + diff.x);
		steer.y = (steer.y + diff.y);
		steer.z = (steer.z + diff.z);

	}

	if (d <= 100.0f)
	{
		for (int i = 0; i < _wpPos.size(); i++)
		{
			if (_bot->getWayPointID() == i)
			{
				// Set new waypoint...
				i++;

				if (i == (_wpPos.size()))
				{
					i = 0; // reset waypoint after the last one
				}
				_bot->setWayPointID(i); // assign next Waypoint...
				break;
			}
		}
	}

	// As long as the vector is greater than 0
	if (steer.x != 0.0f && steer.y != 0.0f && steer.z != 0.0f)
	{
		XMVECTOR steer_vector = XMLoadFloat3(&steer);

		steer_vector = XMVector3ClampLength(steer_vector, 0.0f, _SBD->bot_max_speed);

		XMVector3Normalize(steer_vector);

		// Implement Reynolds: steering = desired - velocity

		steer_vector = (steer_vector * _SBD->bot_max_speed);

		XMVECTOR velocity = XMLoadFloat3(&_bot->getVelocity());

		steer_vector = (steer_vector - velocity);

		steer_vector = XMVector3ClampLength(steer_vector, 0.0f, _SBD->bot_max_speed);
	}

	return steer;
}
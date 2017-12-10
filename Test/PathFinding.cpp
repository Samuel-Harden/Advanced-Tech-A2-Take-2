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



XMFLOAT3 PathFinding::CalculateBehaviour3(SwarmBot* _bot, SwarmBotData* _SBD, std::vector<XMFLOAT3>& _wpPos)
{
	XMFLOAT3 steer = Vector3Zero;
	int count = 0;

	float d = Vector3Distance(_bot->GetPos(), _wpPos[_bot->GetWayPointID()]);

	// if Boid is further than x away from waypoint
	if (d > 50)
	{
		XMFLOAT3 diff = Vector3Zero;
		// Calculate vector pointing away from neighbour
		diff.x = (_bot->GetPos().x - (_wpPos[_bot->GetWayPointID()].x));
		diff.y = (_bot->GetPos().y - (_wpPos[_bot->GetWayPointID()].y));
		diff.z = (_bot->GetPos().z - (_wpPos[_bot->GetWayPointID()].z));

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

	if (d <= 50)
	{
		for (int i = 0; i < _wpPos.size(); i++)
		{
			if (_bot->GetWayPointID() == i)
			{
				// Set new waypoint...
				i++;

				if (i == (_wpPos.size()))
				{
					i = 0; // reset waypoint after the last one
				}
				_bot->SetWayPointID(i); // assign next Waypoint...
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

		XMVECTOR velocity = XMLoadFloat3(&_bot->GetVelocity());

		steer_vector = (steer_vector - velocity);

		steer_vector = XMVector3ClampLength(steer_vector, 0.0f, _SBD->bot_max_speed);
	}

	return steer;
}
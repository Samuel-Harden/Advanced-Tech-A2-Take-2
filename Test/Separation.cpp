#include "Separation.h"

#include "SwarmBot.h"
#include "SwarmBotData.h"



Separation::Separation()
{

}



Separation::~Separation()
{

}



// Seperation
// Method checks for nearby boids and steers away
DirectX::XMFLOAT3 Separation::CalculateBehaviour1(SwarmBot* _bot, SwarmBotData* _SBD, std::vector<SwarmBot*>& _bots)
{
	using namespace DirectX;

	XMFLOAT3 steer = Vector3Zero;
	int count = 0;

	// Check through every other boid
	for (int i = 0; i < _bots.size(); i++)
	{
		if (_bots[i]->GetIsActive() == true)
		{
			float d = Vector3Distance(_bot->GetPos(), _bots[i]->GetPos());

			// if Boid is a neighbour
			if (d > 0 && d < _SBD->desiredSep)
			{
				XMFLOAT3 diff = Vector3Zero;
				// Calculate vector pointing away from neighbour
				diff.x = (_bot->GetPos().x - _bots[i]->GetPos().x);
				diff.y = (_bot->GetPos().y - _bots[i]->GetPos().y);
				diff.z = (_bot->GetPos().z - _bots[i]->GetPos().z);

				XMVECTOR diff_vector = XMLoadFloat3(&diff);

				XMVector3Normalize(diff_vector);

				XMStoreFloat3(&diff, diff_vector);

				diff.x = (diff.x / d); // Weight by distance
				diff.y = (diff.y / d);
				diff.z = (diff.z / d);


				steer.x = (steer.x + diff.x);
				steer.y = (steer.y + diff.y);
				steer.z = (steer.z + diff.z);

				count++;
			}
		}
	}

	// Average -- divided by how many
	if (count > 0)
	{
		steer.x = (steer.x / count);
		steer.y = (steer.y / count);
		steer.z = (steer.z / count);
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

		XMStoreFloat3(&steer, steer_vector);
	}
	return steer;
}
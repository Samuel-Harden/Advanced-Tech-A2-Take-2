#include <memory>
#include <string>


class SwarmBotData
{
public:

	// Friend classes have access to protected data
	friend class SwarmBot;
	friend class SwarmManager;

	friend class Separation;
	friend class PathFinding;

	SwarmBotData();
	~SwarmBotData();

protected:

	void setSepWeight(float _weight);
	void setPathWeight(float _weight);

	void setBotMaxSpeed(float _speed);
	void setMaxForce(float _force);
	void setNeighbourDist(float _dist);
	void setDesiredSep(float _sep);

	// Behaviour Weights
	float sep_weight;
	float path_weight;

	// Swarm Bot Variables
	float bot_max_speed;
	float bot_max_force;
	float neighbour_distance;
	float desired_separation;

private:

};
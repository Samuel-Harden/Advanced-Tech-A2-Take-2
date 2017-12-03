
#include <memory>
#include <string>

class SwarmBotData
{
public:

	// Friend classes have access to protected data
	friend class SwarmBot;
	friend class SwarmManager;

	friend class Separation;
	friend class Alignment;
	friend class Cohesion;
	friend class Avoidance;
	friend class PathFinding;

	SwarmBotData();
	~SwarmBotData();

protected:

	void SetSepWeight(float _weight);
	void SetFFWeight(float _weight);
	void SetAliWeight(float _weight);
	void SetCohWeight(float _weight);
	void SetRunWeight(float _weight);
	void SetPathWeight(float _weight);

	void SetBotMaxSpeed(float _speed);
	void SetMaxForce(float _force);
	void SetNeighbourDist(float _dist);
	void SetDesiredSep(float _sep);

	// Behaviour Weights
	float sepWeight;
	float ffWeight;
	float aliWeight;
	float cohWeight;
	float runWeight;
	float pathWeight;

	// Swarm Bot Variables
	float bot_max_speed;
	float bot_max_force;
	float neighbourDist;
	float desiredSep;

private:

};
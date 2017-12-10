#include "SwarmBot.h"

#include <DirectXMath.h>

#include "GameData.h"
#include "SwarmBotData.h"

#include "Behaviour.h"


SwarmBot::SwarmBot()
	: is_active(true),
	newPos(false),
	wayPointID(0),
	acceleration(0.0f, 0.0f, 0.0f)
{

}


SwarmBot::SwarmBot(XMFLOAT2 _min_pos, XMFLOAT2 _max_pos)
	: is_active(true),
	newPos(false),
	wayPointID(0),
	acceleration(0.0f, 0.0f, 0.0f)
{
	SetRandPos(_min_pos, _max_pos);
}



SwarmBot::~SwarmBot()
{

}



void SwarmBot::Init(ID3D11Device* GD)
{
	//calculate number of vertices and primatives
	int numVerts = 6;
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i<numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = {1.0, 1.0};
	}

	int vert = 0;

	XMFLOAT4 color = { .0f, 0.0f, 0.0f, 1.0f };

	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(0.5f, -0.5f, 0.0f);
	m_vertices[vert].Color = color; // BottomFace 
	m_vertices[vert++].Pos = XMFLOAT3(-0.5f, 0.5f, 0.0f);

	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(0.5f, -0.5f, 0.0f);
	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(0.5f, 0.5f, 0.0f);
	m_vertices[vert].Color = color; // FlatFace
	m_vertices[vert++].Pos = XMFLOAT3(-0.5f, 0.5f, 0.0f);

	/*//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i<m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}*/

	BuildIB(GD, indices);
	BuildVB(GD, numVerts, m_vertices);

	delete[] indices;    //this is no longer needed as this is now in the index Buffer
	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	m_vertices = nullptr;
}



void SwarmBot::Tick(SwarmBotData* _SBD)
{
	velocity.x = (velocity.x + acceleration.x);
	velocity.y = (velocity.y + acceleration.y);
	velocity.z = (velocity.z + acceleration.z);

	XMVECTOR velocity_vector = XMLoadFloat3(&velocity);

	velocity_vector = XMVector3Normalize(velocity_vector);

	velocity_vector = XMVector3ClampLength(velocity_vector, 0.0f, _SBD->bot_max_speed);

	XMStoreFloat3(&velocity, velocity_vector);

	m_pos.x = (m_pos.x + velocity.x);
	m_pos.y = (m_pos.y + velocity.y);
	m_pos.z = (m_pos.z + velocity.z);

	XMMATRIX scaleMat = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_rotMat = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);

	XMMATRIX trans_mat = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

	m_worldMat = scaleMat * m_rotMat * trans_mat;
}


void SwarmBot::Run(std::vector<SwarmBot*>& _bots, SwarmBotData* _swarm_data, std::vector<Behaviour*> _behaviours, std::vector<DirectX::XMFLOAT3>& _wpPos)
{
	using namespace DirectX;
	// Behaviours...
	XMFLOAT3 sep = _behaviours[0]->CalculateBehaviour1(this, _swarm_data, _bots);    // Seperation

	XMFLOAT3 pf = _behaviours[1]->CalculateBehaviour3(this, _swarm_data, _wpPos);    // Path Finding

	sep.x *= _swarm_data->sepWeight;
	sep.y *= _swarm_data->sepWeight;
	sep.z *= _swarm_data->sepWeight;

	pf.x *= _swarm_data->pathWeight;
	pf.y *= _swarm_data->pathWeight;
	pf.z *= _swarm_data->pathWeight;

	acceleration = Vector3Zero;

	// add these 'forces' to acceleration...
	ApplyForce(sep);
	ApplyForce(pf);
}


void SwarmBot::ApplyForce(XMFLOAT3& force)
{
	acceleration.x = (acceleration.x + force.x);
	acceleration.y = (acceleration.y + force.y);
	acceleration.z = (acceleration.z + force.z);
}


bool SwarmBot::GetIsActive()
{
	return is_active;
}


void SwarmBot::SetWayPointID(int& _newID)
{
	wayPointID = _newID;
}



int SwarmBot::GetWayPointID() const
{
	return wayPointID;
}


DirectX::XMFLOAT3 SwarmBot::GetVelocity() const
{
	return velocity;
}


void SwarmBot::SetRandPos(XMFLOAT2 _min, XMFLOAT2 _max)
{
	// Set random starting positon, inside the Designated zone
	float pos_x = RandomFloat(_min.x, _max.x);
	float pos_y = RandomFloat(_min.y, _max.y);
	float pos_z = 0.0f;

	m_pos = XMFLOAT3(pos_x, pos_y, pos_z);
}


float SwarmBot::RandomFloat(float _min, float _max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return _min + r * (_max - _min);
}
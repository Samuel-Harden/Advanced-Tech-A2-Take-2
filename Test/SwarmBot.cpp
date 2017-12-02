#include "SwarmBot.h"

#include "GameData.h"


SwarmBot::SwarmBot()
{

}



SwarmBot::~SwarmBot()
{

}



void SwarmBot::init(ID3D11Device* GD)
{
	//calculate number of vertices and primatives
	int numVerts = 12;
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

	XMFLOAT4 color = { 1.0f, 0.0f, 0.0f, 1.0f };

	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = color; // BottomFace 
	m_vertices[vert++].Pos = XMFLOAT3(5.0f, 0.f, 0.0f);
	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(2.5f, 0.0f, 5.0f);

	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(5.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = color; // FlatFace
	m_vertices[vert++].Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(2.5f, 1.25f, 0.0f);

	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(2.5f, 0.0f, 5.0f);
	m_vertices[vert].Color = color; // Face2
	m_vertices[vert++].Pos = XMFLOAT3(2.5f, 1.25f, 0.0f);
	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(2.5f, 1.25f, 0.0f);
	m_vertices[vert].Color = color; // Face3
	m_vertices[vert++].Pos = XMFLOAT3(2.5f, 0.0f, 5.0f);
	m_vertices[vert].Color = color;
	m_vertices[vert++].Pos = XMFLOAT3(5.0f, 0.0f, 0.0f);

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



void SwarmBot::tick()
{
	XMMATRIX scaleMat = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_rotMat = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);

	XMMATRIX trans_mat = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

	m_worldMat = scaleMat * m_rotMat * trans_mat;
}
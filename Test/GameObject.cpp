#include "GameObject.h"
#include "GameData.h"


GameObject::GameObject()
{
	m_pos.x = 0;
	m_pos.y = 0;
	m_pos.z = 0;
	m_pitch = 0;
	m_yaw = 0;
	m_roll = 0;
	m_scale = DirectX::Vector3One;

	// Is this right?!??!?
	m_worldMat = DirectX::XMMatrixIdentity();
}


GameObject::~GameObject()
{

}


void GameObject::Tick(GameData* _game_data)
{

	DirectX::FXMVECTOR scale = { m_scale.x, m_scale.y, m_scale.z };
	//build up the world matrix depending on the new position of the GameObject
	//the assumption is that this class will be inherited by the class that ACTUALLY changes this
	DirectX::XMMATRIX  scaleMat = DirectX::XMMatrixScalingFromVector(scale);
	m_rotMat = DirectX::XMMatrixRotationRollPitchYaw(m_yaw, m_pitch, m_roll); //possible not the best way of doing this!
	DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

	m_worldMat = scaleMat * m_rotMat * transMat;
}


void GameObject::Draw(DrawData* _draw_data)
{

}


// Getters
DirectX::XMFLOAT3 GameObject::GetScale() const
{
	return m_scale;
}


DirectX::XMFLOAT3 GameObject::GetPos() const
{
	return m_pos;
}


float GameObject::GetYaw() const
{
	return m_yaw;
}


float GameObject::GetPitch() const
{
	return m_pitch;
}


float GameObject::GetRoll() const
{
	return m_roll;
}


// Setters
void GameObject::SetPos(DirectX::XMFLOAT3 _pos)
{
	m_pos = _pos;
}


void GameObject::SetPos(float _x, float _y, float _z)
{
	m_pos.x = _x;
	m_pos.y = _y;
	m_pos.z = _z;
}


void GameObject::SetScale(float _scale)
{
	m_scale.x = _scale * 1;
	m_scale.y = _scale * 1;
	m_scale.z = _scale * 1;
}


void GameObject::SetScale(float _x, float _y, float _z)
{
	m_scale.x = _x;
	m_scale.y = _y;
	m_scale.z = _z;
}


void GameObject::SetScale(DirectX::XMFLOAT3 _scale)
{
	m_scale = _scale;
}


void GameObject::SetPitch(float _pitch)
{
	m_pitch = _pitch;
}


void GameObject::SetYaw(float _yaw)
{
	m_yaw = _yaw;
}


void GameObject::SetRoll(float _roll)
{
	m_roll = _roll;
}


void GameObject::SetPitchYawRoll(float _pitch, float _yaw, float _roll)
{
	m_pitch = _pitch;
	m_yaw = _yaw;
	m_roll = _roll;
}
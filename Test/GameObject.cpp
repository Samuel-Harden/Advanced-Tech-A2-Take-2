#include "GameObject.h"
#include "GameData.h"


GameObject::GameObject()
{
	m_pos   = DirectX::Vector3Zero;
	m_scale = DirectX::Vector3One;
	m_pitch = 0;
	m_yaw   = 0;
	m_roll  = 0;

	m_world_matrix = DirectX::XMMatrixIdentity();
}


GameObject::~GameObject()
{

}


void GameObject::tick(GameData* _game_data)
{
	DirectX::FXMVECTOR scale = { m_scale.x, m_scale.y, m_scale.z };

	DirectX::XMMATRIX  scaleMat = DirectX::XMMatrixScalingFromVector(scale);

	m_rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(m_yaw, m_pitch, m_roll);

	DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

	m_world_matrix = scaleMat * m_rotation_matrix * transMat;
}


void GameObject::draw(DrawData* _draw_data)
{

}


// Getters
DirectX::XMFLOAT3 GameObject::getScale() const
{
	return m_scale;
}


DirectX::XMFLOAT3 GameObject::getPos() const
{
	return m_pos;
}


float GameObject::getYaw() const
{
	return m_yaw;
}


float GameObject::getPitch() const
{
	return m_pitch;
}


float GameObject::getRoll() const
{
	return m_roll;
}


// Setters
void GameObject::setPos(DirectX::XMFLOAT3 _pos)
{
	m_pos = _pos;
}


void GameObject::setPos(float _x, float _y, float _z)
{
	m_pos.x = _x;
	m_pos.y = _y;
	m_pos.z = _z;
}


void GameObject::setScale(float _scale)
{
	m_scale.x = _scale * 1;
	m_scale.y = _scale * 1;
	m_scale.z = _scale * 1;
}


void GameObject::setScale(float _x, float _y, float _z)
{
	m_scale.x = _x;
	m_scale.y = _y;
	m_scale.z = _z;
}


void GameObject::setScale(DirectX::XMFLOAT3 _scale)
{
	m_scale = _scale;
}


void GameObject::setPitch(float _pitch)
{
	m_pitch = _pitch;
}


void GameObject::setYaw(float _yaw)
{
	m_yaw = _yaw;
}


void GameObject::setRoll(float _roll)
{
	m_roll = _roll;
}


void GameObject::setPitchYawRoll(float _pitch, float _yaw, float _roll)
{
	m_pitch = _pitch;
	m_yaw = _yaw;
	m_roll = _roll;
}
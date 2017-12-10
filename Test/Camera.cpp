//base camera class
#include "Camera.h"


Camera::Camera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, DirectX::XMFLOAT3 _up, DirectX::XMFLOAT3 _target)
{
	m_fieldOfView = _fieldOfView;
	m_aspectRatio = _aspectRatio;
	m_nearPlaneDistance = _nearPlaneDistance;
	m_farPlaneDistance = _farPlaneDistance;

	m_target = _target;
	m_up = _up;
}


Camera::~Camera()
{

}


void Camera::Tick(GameData* _GD)
{
	DirectX::XMVECTOR pos, target, up;

	pos = {m_pos.x, m_pos.y, m_pos.z};
	target = {m_target.x, m_target.y, m_target.z};
	up = {m_up.x, m_up.y, m_up.z};

	m_projMat = DirectX::XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance);
	
	m_viewMat = DirectX::XMMatrixLookAtLH(pos, target, up);
	GameObject::Tick(_GD);
}


void Camera::Draw(DrawData* _DD)
{
	//standard camera doesn't draw ANYTHING
}


DirectX::XMMATRIX Camera::getProjMatrix()
{
	return m_projMat;
}


DirectX::XMMATRIX Camera::getViewMatrix()
{
	return m_viewMat;
}
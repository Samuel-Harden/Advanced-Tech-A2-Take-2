//base camera class
#include "Camera.h"


Camera::Camera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, DirectX::XMFLOAT3 _up, DirectX::XMFLOAT3 _target)
{
	m_field_of_view        = _fieldOfView;
	m_aspect_ratio         = _aspectRatio;
	m_near_plane_distance  = _nearPlaneDistance;
	m_far_plane_distance   = _farPlaneDistance;

	m_target               = _target;
	m_up                   = _up;
}


Camera::~Camera()
{

}


void Camera::tick(GameData* _GD)
{
	DirectX::XMVECTOR pos, target, up;

	pos     = {m_pos.x, m_pos.y, m_pos.z};
	target  = {m_target.x, m_target.y, m_target.z};
	up      = {m_up.x, m_up.y, m_up.z};

	m_projection_matrix = DirectX::XMMatrixPerspectiveFovLH(m_field_of_view, m_aspect_ratio, m_near_plane_distance, m_far_plane_distance);
	
	m_view_matrix = DirectX::XMMatrixLookAtLH(pos, target, up);

	GameObject::tick(_GD);
}


void Camera::draw(DrawData* _DD)
{
	//standard camera doesn't draw ANYTHING
}


DirectX::XMMATRIX Camera::getProjMatrix()
{
	return m_projection_matrix;
}


DirectX::XMMATRIX Camera::getViewMatrix()
{
	return m_view_matrix;
}
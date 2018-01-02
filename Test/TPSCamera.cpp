#include "TPSCamera.h"

#include "GameData.h"


TPSCamera::TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, DirectX::XMFLOAT3 _up, DirectX::XMFLOAT3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_target_object = _target;
	m_dpos = _dpos;
}


TPSCamera::~TPSCamera()
{

}


void TPSCamera::tick(GameData* _GD)
{
	DirectX::XMMATRIX rotCam = DirectX::XMMatrixRotationRollPitchYaw(getPitch(), getYaw(), 0.0f);

	DirectX::XMVECTOR pos, target_pos, dpos;

	dpos       = {m_dpos.x, m_dpos.y, m_dpos.z};
	pos        = {m_pos.x, m_pos.y, m_pos.z};
	target_pos = {m_target_object->getPos().x, m_target_object->getPos().y, m_target_object->getPos().z};

	// Sets the positon of the camera
	pos = DirectX::XMVectorAdd(target_pos, DirectX::XMVector3Transform(dpos, rotCam));

	DirectX::XMStoreFloat3(&m_pos, pos);

	// Sets what the camera is looking at
	m_target = m_target_object->getPos();

	// Then set up proj and view matrices
	Camera::tick(_GD);
}


void TPSCamera::increaseZoom()
{
	m_dpos.z += 20.0f;

	if (m_dpos.z > -200.0f)
	{
		m_dpos.z = -200.0f;
	}
}


void TPSCamera::decreaseZoom()
{
	m_dpos.z -= 20.0f;

	if (m_dpos.z < -1200.0f)
	{
		m_dpos.z = -1200.0f;
	}
}


void TPSCamera::allowRotation(GameData* _GD)
{
	m_yaw -= 0.01f * _GD->m_mouse_state->lX;
	m_pitch -= 0.01f * _GD->m_mouse_state->lY;

	// Limit camera rotation
	if (m_pitch > 1.0f)
	{
		m_pitch = 1.0f;
	}
	if (m_pitch < -1.0f)
	{
		m_pitch = -1.0f;
	}

	if (m_yaw > 1.0f)
	{
		m_yaw = 1.0f;
	}
	if (m_yaw < -1.0f)
	{
		m_yaw = -1.0f;
	}
}
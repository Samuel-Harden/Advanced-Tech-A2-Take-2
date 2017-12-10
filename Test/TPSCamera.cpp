#include "TPSCamera.h"
#include "GameData.h"

TPSCamera::TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, DirectX::XMFLOAT3 _up, DirectX::XMFLOAT3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetObject = _target;
	m_dpos = _dpos;
}

TPSCamera::~TPSCamera()
{

}

void TPSCamera::Tick(GameData* _GD)
{
	// Set up position  of camera and target position of camera based on new
	// position and orientation of target object

	DirectX::XMMATRIX rotCam = DirectX::XMMatrixRotationRollPitchYaw(GetPitch(), GetYaw(), 0.0f);

	/*// if in the third person view, we take mouse inputs to rotate the camera
	if (camera_state == TPS_CAMERA)
	{
		m_yaw -= 0.01f * _GD->m_mouseState->lX;
		m_pitch -= 0.01f * _GD->m_mouseState->lY;


		// Limit camera rotation
		if (m_pitch > 1.0f)
		{
			m_pitch = 1.0f;
		}
		if (m_pitch < -1.0f)
		{
			m_pitch = -1.0f;
		}
	}*/

	DirectX::XMVECTOR pos, target_pos, dpos;

	dpos = {m_dpos.x, m_dpos.y, m_dpos.z};

	pos = {m_pos.x, m_pos.y, m_pos.z};
	target_pos = { m_targetObject->GetPos().x, m_targetObject->GetPos().y, m_targetObject->GetPos().z };

	// Sets the positon of the camera
	pos = DirectX::XMVectorAdd(target_pos, DirectX::XMVector3Transform(dpos, rotCam));

	DirectX::XMStoreFloat3(&m_pos, pos);

	// Sets what the camera is looking at
	m_target = m_targetObject->GetPos();

	// Then set up proj and view matrices
	Camera::Tick(_GD);
}



void TPSCamera::IncreaseZoom()
{
	m_dpos.z += 20.0f;

	if (m_dpos.z > -200.0f)
	{
		m_dpos.z = -200.0f;
	}
}



void TPSCamera::DecreaseZoom()
{
	m_dpos.z -= 20.0f;

	if (m_dpos.z < -1200.0f)
	{
		m_dpos.z = -1200.0f;
	}
}


void TPSCamera::AllowRotation(GameData* _GD)
{
	m_yaw -= 0.01f * _GD->m_mouseState->lX;
	m_pitch -= 0.01f * _GD->m_mouseState->lY;

	// Limit camera rotation
	if (m_pitch > 1.0f)
	{
		m_pitch = 1.0f;
	}
	if (m_pitch < -1.0f)
	{
		m_pitch = -1.0f;
	}
}
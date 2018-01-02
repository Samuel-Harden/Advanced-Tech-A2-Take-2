#pragma once

#include "camera.h"

class TPSCamera : public Camera
{
public:
	TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, DirectX::XMFLOAT3 _up, DirectX::XMFLOAT3 _dpos);
	virtual ~TPSCamera();

	virtual void tick(GameData* _GD) override;

	void increaseZoom();
	void decreaseZoom();

	void allowRotation(GameData* _GD);

protected:
	GameObject*	m_target_object; //I'm following this object
	DirectX::XMFLOAT3 m_dpos; //I'll lurk this far behind and away from it
};

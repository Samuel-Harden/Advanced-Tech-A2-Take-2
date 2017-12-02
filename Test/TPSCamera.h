#pragma once

#include "camera.h"

//=================================================================
//TPS style camera which will follow a given GameObject around _target
//=================================================================

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
	GameObject*	m_targetObject; //I'm following this object
	DirectX::XMFLOAT3 m_dpos; //I'll lurk this far behind and away from it
};

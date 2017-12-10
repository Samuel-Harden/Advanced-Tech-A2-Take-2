#pragma once

#include "gameobject.h"

struct GameData;

class Camera : public GameObject
{
public:
	Camera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, DirectX::XMFLOAT3 _up = DirectX::Vector3Up, DirectX::XMFLOAT3 _target = DirectX::Vector3Zero);
	~Camera();

	virtual void Tick(GameData* _GD) override;

	virtual void Draw(DrawData* _DD) override;

	//Getters
	DirectX::XMMATRIX getProjMatrix();
	DirectX::XMMATRIX getViewMatrix();

	//Setters

protected:

	//Principle transforms/matrices for this camera
	DirectX::XMMATRIX m_projMat;
	DirectX::XMMATRIX m_viewMat;

	//parameters for setting up a camera
	float m_fieldOfView;
	float m_aspectRatio;
	float m_nearPlaneDistance;
	float m_farPlaneDistance;

	DirectX::XMFLOAT3 m_target;
	DirectX::XMFLOAT3 m_up;
};
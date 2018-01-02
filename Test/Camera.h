#pragma once

#include "gameobject.h"

struct GameData;

class Camera : public GameObject
{
public:
	Camera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, DirectX::XMFLOAT3 _up = DirectX::Vector3Up, DirectX::XMFLOAT3 _target = DirectX::Vector3Zero);
	~Camera();

	virtual void tick(GameData* _GD) override;

	virtual void draw(DrawData* _DD) override;

	//Getters
	DirectX::XMMATRIX getProjMatrix();
	DirectX::XMMATRIX getViewMatrix();

	//Setters

protected:

	//Principle transforms/matrices for this camera
	DirectX::XMMATRIX m_projection_matrix;
	DirectX::XMMATRIX m_view_matrix;

	//parameters for setting up a camera
	float m_field_of_view;
	float m_aspect_ratio;
	float m_near_plane_distance;
	float m_far_plane_distance;

	DirectX::XMFLOAT3 m_target;
	DirectX::XMFLOAT3 m_up;
};
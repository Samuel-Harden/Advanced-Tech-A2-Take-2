#pragma once

#include<DirectXMath.h>

#include "SimpleMathAid.h"

class Camera;
struct GameData;
struct DrawData;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	DirectX::XMFLOAT3 GetScale() const;
	DirectX::XMFLOAT3 GetPos() const;

	virtual void Tick(GameData* _game_data);
	virtual void Draw(DrawData* _draw_data) = 0;

	// Getters
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;

	// Setters
	void SetPos(DirectX::XMFLOAT3 _pos);
	void SetPos(float _x, float _y, float _z);

	void SetScale(float _scale);
	void SetScale(float _x, float _y, float _z);
	void SetScale(DirectX::XMFLOAT3 _scale);

	void SetPitch(float _pitch);
	void SetYaw(float _yaw);
	void SetRoll(float _roll);
	void SetPitchYawRoll(float _pitch, float _yaw, float _roll);

protected:

	DirectX::XMMATRIX m_worldMat;
	DirectX::XMMATRIX m_rotMat;

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	float m_pitch;
	float m_yaw;
	float m_roll;

private:


};
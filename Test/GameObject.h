#pragma once

#include<DirectXMath.h>

#include "GameData.h"
#include "DrawData.h"
#include "SimpleMathAid.h"


class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	DirectX::XMFLOAT3 getScale() const;
	DirectX::XMFLOAT3 getPos() const;

	virtual void tick(GameData* _game_data);
	virtual void draw(DrawData* _draw_data);
	//virtual void draw() = 0;

	// Getters
	float getYaw() const;
	float getPitch() const;
	float getRoll() const;

	// Setters
	void setPos(DirectX::XMFLOAT3 _pos);

	void setScale(float _scale);
	void setScale(float _x, float _y, float _z);
	void setScale(DirectX::XMFLOAT3 _scale);

	void setPitch(float _pitch);
	void setYaw(float _yaw);
	void setRoll(float _roll);
	void setPitchYawRoll(float _pitch, float _yaw, float _roll);

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
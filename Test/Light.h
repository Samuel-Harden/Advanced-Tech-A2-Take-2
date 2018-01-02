#pragma once

#include "gameobject.h"

struct DrawData;

using namespace DirectX;

class Light : public GameObject
{
public:
	Light(XMFLOAT3 _pos, XMFLOAT4 _colour, XMFLOAT4 _ambientColour);
	~Light();

	virtual void draw(DrawData* _DD) override { _DD; }; //draws nothing

	virtual void tick(GameData* _GD) override;

	//getters
	XMFLOAT4 getColour() { return m_colour; }
	XMFLOAT4 getAmbCol() { return m_ambient_colour; }

	//setters
	void setColor(XMFLOAT4 _colour) { m_colour = _colour; }
	void setAmbCol(XMFLOAT4 _colour) { m_ambient_colour = _colour; }

protected:

	XMFLOAT4 m_colour;
	XMFLOAT4 m_ambient_colour;
};
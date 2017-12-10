#pragma once

#include "gameobject.h"

//=================================================================
//a simple light defined by a colour and and ambient colour and 
//used as a point light with no drop off in the standard shader 
//used to draw VBGOs
//=================================================================

struct DrawData;

using namespace DirectX;

class Light : public GameObject
{
public:
	Light(XMFLOAT3 _pos, XMFLOAT4 _colour, XMFLOAT4 _ambientColour);
	~Light();

	virtual void Draw(DrawData* _DD) override { _DD; }; //draws nothing

	virtual void Tick(GameData* _GD) override;

	//getters
	XMFLOAT4 GetColour() { return m_colour; }
	XMFLOAT4 GetAmbCol() { return m_ambientColour; }

	//setters
	void SetColor(XMFLOAT4 _colour) { m_colour = _colour; }
	void SetAmbCol(XMFLOAT4 _colour) { m_ambientColour = _colour; }

protected:

	XMFLOAT4 m_colour;
	XMFLOAT4 m_ambientColour;
};
#include "light.h"

#include "gamedata.h"


Light::Light(XMFLOAT3 _pos, XMFLOAT4 _colour, XMFLOAT4 _ambientColour)
{
	m_pos = _pos;
	m_colour = _colour;
	m_ambient_colour = _ambientColour;
}


Light::~Light()
{

}


void Light::tick(GameData* _GD)
{
	
}
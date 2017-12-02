#pragma once

#include <d3d11.h>

class Camera;
class Light;

struct DrawData
{
	ID3D11DeviceContext* m_pd3dImmediateContext;
	Camera* m_cam;
	Light* m_light;

};

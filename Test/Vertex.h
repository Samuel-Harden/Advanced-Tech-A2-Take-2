#pragma once
//=================================================================
// default Vertex Structure used by the VBGO render system
//=================================================================

#include <d3d11_1.h>
#include <DirectXMath.h>

using namespace DirectX;

struct myVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Norm;
	XMFLOAT4 Color;
	XMFLOAT2 texCoord;
};

const D3D11_INPUT_ELEMENT_DESC myVertexLayout[] =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
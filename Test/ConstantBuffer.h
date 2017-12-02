#pragma once


#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

//as passing to GPU needs to be correctly memory aligned
__declspec(align(16))
struct ConstantBuffer
{
	XMMATRIX	world;
	XMMATRIX	view;
	XMMATRIX	projection;
	XMMATRIX	rot;
	XMFLOAT4	lightCol;
	XMFLOAT4	ambientCol;
	XMFLOAT3    lightPos;
};
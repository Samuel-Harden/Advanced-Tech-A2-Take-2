#pragma once

#include <DirectXMath.h>

// Base Vectors (Like Unity)
namespace DirectX
{
	static const XMFLOAT3 Vector3Zero = { 0.0f,  0.0f,  0.0f };
	static const XMFLOAT3 Vector3One = { 1.0f,  1.0f,  1.0f };
	static const XMFLOAT3 Vector3Up = { 0.0f,  1.0f,  0.0f };
	static const XMFLOAT3 Vector3Down = { 0.0f, -1.0f,  0.0f };
	static const XMFLOAT3 Vector3Left = { -1.0f,  0.0f,  0.0f };
	static const XMFLOAT3 Vector3Right = { 1.0f,  0.0f,  0.0f };
	static const XMFLOAT3 Vector3Forward = { 0.0f,  0.0f,  1.0f };
	static const XMFLOAT3 VectorBackward = { 0.0f,  0.0f, -1.0f };
}
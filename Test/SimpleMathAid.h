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


	// Get distance between two vectors
	static const float Vector3Distance(const XMFLOAT3& v1, const XMFLOAT3& v2)
	{
		XMVECTOR vector_one = XMLoadFloat3(&v1);
		XMVECTOR vector_two = XMLoadFloat3(&v2);
		XMVECTOR vector_sub = XMVectorSubtract(vector_one, vector_two);
		XMVECTOR length = XMVector3Length(vector_sub);

		float distance = 0.0f;
		XMStoreFloat(&distance, length);

		return distance;
	}
}
#pragma once

#include <DirectXMath.h>
#include <stdlib.h>

// Base Vectors (Like Unity)
namespace DirectX
{
	// Vector Two
	static const XMFLOAT2 Vector2Zero   = { 0.0f,  0.0f };

	// Vector Three
	static const XMFLOAT3 Vector3Zero     = { 0.0f,  0.0f,  0.0f };
	static const XMFLOAT3 Vector3One      = { 1.0f,  1.0f,  1.0f };
	static const XMFLOAT3 Vector3Up       = { 0.0f,  1.0f,  0.0f };
	static const XMFLOAT3 Vector3Down     = { 0.0f, -1.0f,  0.0f };
	static const XMFLOAT3 Vector3Left     = {-1.0f,  0.0f,  0.0f };
	static const XMFLOAT3 Vector3Right    = { 1.0f,  0.0f,  0.0f };
	static const XMFLOAT3 Vector3Forward  = { 0.0f,  0.0f,  1.0f };
	static const XMFLOAT3 VectorBackward  = { 0.0f,  0.0f, -1.0f };


	// Helper Functions
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


	static const float randomFloat(float _min, float _max)
	{
		float r = (float)rand() / (float)RAND_MAX;
		return _min + r * (_max - _min);
	}


	XMFLOAT3 static const setRandPos(XMFLOAT2 _min, XMFLOAT2 _max)
	{
		float pos_x = randomFloat(_min.x, _max.x);
		float pos_y = randomFloat(_min.y, _max.y);
		float pos_z = 0.0f; // Only using X and Y

		XMFLOAT3 pos = XMFLOAT3(pos_x, pos_y, pos_z);

		return pos;
	}
}
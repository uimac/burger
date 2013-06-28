/**
 * @file UMVectorTest.cpp
 *
 * @author ori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license.
 *
 */
#include <boost/test/unit_test.hpp>
#include <DirectXMath.h>
#include <random>
#include "UMVector.h"

#define N 10
#define TEST_EPSILON 0.0001f

namespace
{
	std::mt19937 mt;
	std::uniform_real_distribution<> random_range(-10.0, 10.0);
	void um_random(float dst[], int size)
	{
		for (int i = 0; i < size; ++i) 
		{
			dst[i] = static_cast<float>(random_range(mt));
		}
	}
}

BOOST_AUTO_TEST_SUITE(UMVectorTest)
	
using namespace DirectX;
using namespace burger;

//----------------------------------------------------------------------------
// Vector2
//----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Vec2Add)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[4];
		um_random(v, 4);

		XMFLOAT2 dx_dst;
		XMFLOAT2 dx_src_left(v[0], v[1]);
		XMFLOAT2 dx_src_right(v[2], v[3]);

		UMVec2f my_src_left(v[0], v[1]);
		UMVec2f my_src_right(v[2], v[3]);
		UMVec2f my_dst1 = my_src_left + my_src_right;
		UMVec2f my_dst2 = my_src_left;
		my_dst2 += my_src_right;

		XMStoreFloat2(&dx_dst,
			XMVectorAdd(
				XMLoadFloat2(&dx_src_left),
				XMLoadFloat2(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec2Sub)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[4];
		um_random(v, 4);

		XMFLOAT2 dx_dst;
		XMFLOAT2 dx_src_left(v[0], v[1]);
		XMFLOAT2 dx_src_right(v[2], v[3]);

		UMVec2f my_src_left(v[0], v[1]);
		UMVec2f my_src_right(v[2], v[3]);
		UMVec2f my_dst1 = my_src_left - my_src_right;
		UMVec2f my_dst2 = my_src_left;
		my_dst2 -= my_src_right;

		XMStoreFloat2(&dx_dst,
			XMVectorSubtract(
				XMLoadFloat2(&dx_src_left),
				XMLoadFloat2(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec2Scale)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[3];
		um_random(v, 3);

		XMFLOAT2 dx_dst;
		XMFLOAT2 dx_src_left(v[0], v[1]);
		float scale = v[2];

		UMVec2f my_src_left(v[0], v[1]);
		UMVec2f my_src_right(v[2], v[3]);
		UMVec2f my_dst1 = my_src_left * scale;
		UMVec2f my_dst2 = my_src_left;
		my_dst2 *= scale;

		XMStoreFloat2(&dx_dst,
			XMVectorScale(
				XMLoadFloat2(&dx_src_left),
				scale));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec2Div)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[3];
		um_random(v, 3);

		XMFLOAT2 dx_dst;
		XMFLOAT2 dx_src_left(v[0], v[1]);
		float scale = v[2];

		UMVec2f my_src_left(v[0], v[1]);
		UMVec2f my_src_right(v[2], v[3]);
		UMVec2f my_dst1 = my_src_left / scale;
		UMVec2f my_dst2 = my_src_left;
		my_dst2 /= scale;

		XMStoreFloat2(&dx_dst,
			XMVectorScale(
				XMLoadFloat2(&dx_src_left),
				1.0f / scale));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec2Dot)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[4];
		um_random(v, 4);

		float dx_dst;
		XMFLOAT2 dx_src_left(v[0], v[1]);
		XMFLOAT2 dx_src_right(v[2], v[3]);

		UMVec2f my_src_left(v[0], v[1]);
		UMVec2f my_src_right(v[2], v[3]);
		float my_dst1 = my_src_left.dot(my_src_right);

		XMStoreFloat(&dx_dst,
			XMVector2Dot(
				XMLoadFloat2(&dx_src_left),
				XMLoadFloat2(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec2Normalize)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[2];
		um_random(v, 2);

		XMFLOAT2 dx_dst;
		XMFLOAT2 dx_src_left(v[0], v[1]);

		UMVec2f my_src_left(v[0], v[1]);
		UMVec2f my_dst1 = my_src_left.normalized();

		XMStoreFloat2(&dx_dst,
			XMVector2Normalize(
				XMLoadFloat2(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec2LengthSq)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[2];
		um_random(v, 2);

		float dx_dst;
		XMFLOAT2 dx_src_left(v[0], v[1]);

		UMVec2f my_src_left(v[0], v[1]);
		float my_dst1 = my_src_left.length_sq();

		XMStoreFloat(&dx_dst,
			XMVector2LengthSq(
				XMLoadFloat2(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec2Length)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[2];
		um_random(v, 2);

		float dx_dst;
		XMFLOAT2 dx_src_left(v[0], v[1]);

		UMVec2f my_src_left(v[0], v[1]);
		float my_dst1 = my_src_left.length();

		XMStoreFloat(&dx_dst,
			XMVector2Length(
				XMLoadFloat2(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

//----------------------------------------------------------------------------
// Vector3
//----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Vec3Add)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[6];
		um_random(v, 6);

		XMFLOAT3 dx_dst;
		XMFLOAT3 dx_src_left(v[0], v[1], v[2]);
		XMFLOAT3 dx_src_right(v[3], v[4], v[5]);

		UMVec3f my_src_left(v[0], v[1], v[2]);
		UMVec3f my_src_right(v[3], v[4], v[5]);
		UMVec3f my_dst1 = my_src_left + my_src_right;
		UMVec3f my_dst2 = my_src_left;
		my_dst2 += my_src_right;

		XMStoreFloat3(&dx_dst,
			XMVectorAdd(
				XMLoadFloat3(&dx_src_left),
				XMLoadFloat3(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst2.z, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec3Sub)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[6];
		um_random(v, 6);

		XMFLOAT3 dx_dst;
		XMFLOAT3 dx_src_left(v[0], v[1], v[2]);
		XMFLOAT3 dx_src_right(v[3], v[4], v[5]);

		UMVec3f my_src_left(v[0], v[1], v[2]);
		UMVec3f my_src_right(v[3], v[4], v[5]);
		UMVec3f my_dst1 = my_src_left - my_src_right;
		UMVec3f my_dst2 = my_src_left;
		my_dst2 -= my_src_right;

		XMStoreFloat3(&dx_dst,
			XMVectorSubtract(
				XMLoadFloat3(&dx_src_left),
				XMLoadFloat3(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst2.z, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec3Scale)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[4];
		um_random(v, 4);

		XMFLOAT3 dx_dst;
		XMFLOAT3 dx_src_left(v[0], v[1], v[2]);
		float scale = v[3];

		UMVec3f my_src_left(v[0], v[1], v[2]);
		UMVec3f my_dst1 = my_src_left * scale;
		UMVec3f my_dst2 = my_src_left;
		my_dst2 *= scale;

		XMStoreFloat3(&dx_dst,
			XMVectorScale(
				XMLoadFloat3(&dx_src_left),
				scale));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst2.z, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec3Div)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[4];
		um_random(v, 4);

		XMFLOAT3 dx_dst;
		XMFLOAT3 dx_src_left(v[0], v[1], v[2]);
		float scale = v[3];

		UMVec3f my_src_left(v[0], v[1], v[2]);
		UMVec3f my_dst1 = my_src_left / scale;
		UMVec3f my_dst2 = my_src_left;
		my_dst2 /= scale;

		XMStoreFloat3(&dx_dst,
			XMVectorScale(
				XMLoadFloat3(&dx_src_left),
				1.0f / scale));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst2.z, TEST_EPSILON);
	}
}


BOOST_AUTO_TEST_CASE(Vec3Dot)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[6];
		um_random(v, 6);

		float dx_dst;
		XMFLOAT3 dx_src_left(v[0], v[1], v[2]);
		XMFLOAT3 dx_src_right(v[3], v[4], v[5]);

		UMVec3f my_src_left(v[0], v[1], v[2]);
		UMVec3f my_src_right(v[3], v[4], v[5]);
		float my_dst1 = my_src_left.dot(my_src_right);

		XMStoreFloat(&dx_dst,
			XMVector3Dot(
				XMLoadFloat3(&dx_src_left),
				XMLoadFloat3(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec3Normalize)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[3];
		um_random(v, 3);

		XMFLOAT3 dx_dst;
		XMFLOAT3 dx_src_left(v[0], v[1], v[2]);

		UMVec3f my_src_left(v[0], v[1], v[2]);
		UMVec3f my_dst1 = my_src_left.normalized();

		XMStoreFloat3(&dx_dst,
			XMVector3Normalize(
				XMLoadFloat3(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec3LengthSq)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[3];
		um_random(v, 3);

		float dx_dst;
		XMFLOAT3 dx_src_left(v[0], v[1], v[2]);

		UMVec3f my_src_left(v[0], v[1], v[2]);
		float my_dst1 = my_src_left.length_sq();

		XMStoreFloat(&dx_dst,
			XMVector3LengthSq(
				XMLoadFloat3(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec3Length)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[3];
		um_random(v, 3);

		float dx_dst;
		XMFLOAT3 dx_src_left(v[0], v[1], v[2]);

		UMVec3f my_src_left(v[0], v[1], v[2]);
		float my_dst1 = my_src_left.length();

		XMStoreFloat(&dx_dst,
			XMVector3Length(
				XMLoadFloat3(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

//----------------------------------------------------------------------------
// Vector4
//----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Vec4Add)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[8];
		um_random(v, 8);

		XMFLOAT4 dx_dst;
		XMFLOAT4 dx_src_left(v[0], v[1], v[2], v[3]);
		XMFLOAT4 dx_src_right(v[4], v[5], v[6], v[7]);

		UMVec4f my_src_left(v[0], v[1], v[2], v[3]);
		UMVec4f my_src_right(v[4], v[5], v[6], v[7]);
		UMVec4f my_dst1 = my_src_left + my_src_right;
		UMVec4f my_dst2 = my_src_left;
		my_dst2 += my_src_right;

		XMStoreFloat4(&dx_dst,
			XMVectorAdd(
				XMLoadFloat4(&dx_src_left),
				XMLoadFloat4(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst1.w, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst2.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst2.w, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec4Sub)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[8];
		um_random(v, 8);

		XMFLOAT4 dx_dst;
		XMFLOAT4 dx_src_left(v[0], v[1], v[2], v[3]);
		XMFLOAT4 dx_src_right(v[4], v[5], v[6], v[7]);

		UMVec4f my_src_left(v[0], v[1], v[2], v[3]);
		UMVec4f my_src_right(v[4], v[5], v[6], v[7]);
		UMVec4f my_dst1 = my_src_left - my_src_right;
		UMVec4f my_dst2 = my_src_left;
		my_dst2 -= my_src_right;

		XMStoreFloat4(&dx_dst,
			XMVectorSubtract(
				XMLoadFloat4(&dx_src_left),
				XMLoadFloat4(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst1.w, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst2.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst2.w, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec4Scale)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[5];
		um_random(v, 5);

		XMFLOAT4 dx_dst;
		XMFLOAT4 dx_src_left(v[0], v[1], v[2], v[3]);
		float scale = v[4];

		UMVec4f my_src_left(v[0], v[1], v[2], v[3]);
		UMVec4f my_dst1 = my_src_left * scale;
		UMVec4f my_dst2 = my_src_left;
		my_dst2 *= scale;

		XMStoreFloat4(&dx_dst,
			XMVectorScale(
				XMLoadFloat4(&dx_src_left),
				scale));
		
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst1.w, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst2.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst2.w, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec4Div)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[5];
		um_random(v, 5);

		XMFLOAT4 dx_dst;
		XMFLOAT4 dx_src_left(v[0], v[1], v[2], v[3]);
		float scale = v[4];

		UMVec4f my_src_left(v[0], v[1], v[2], v[3]);
		UMVec4f my_dst1 = my_src_left / scale;
		UMVec4f my_dst2 = my_src_left;
		my_dst2 /= scale;

		XMStoreFloat4(&dx_dst,
			XMVectorScale(
				XMLoadFloat4(&dx_src_left),
				1.0f / scale));
		
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst1.w, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst2.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst2.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst2.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst2.w, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec4Dot)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[8];
		um_random(v, 8);

		float dx_dst;
		XMFLOAT4 dx_src_left(v[0], v[1], v[2], v[3]);
		XMFLOAT4 dx_src_right(v[4], v[5], v[6], v[7]);

		UMVec4f my_src_left(v[0], v[1], v[2], v[3]);
		UMVec4f my_src_right(v[4], v[5], v[6], v[7]);
		float my_dst1 = my_src_left.dot(my_src_right);

		XMStoreFloat(&dx_dst,
			XMVector4Dot(
				XMLoadFloat4(&dx_src_left),
				XMLoadFloat4(&dx_src_right)));

		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec4Normalize)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[4];
		um_random(v, 4);

		XMFLOAT4 dx_dst;
		XMFLOAT4 dx_src_left(v[0], v[1], v[2], v[3]);

		UMVec4f my_src_left(v[0], v[1], v[2], v[3]);
		UMVec4f my_dst1 = my_src_left.normalized();

		XMStoreFloat4(&dx_dst,
			XMVector4Normalize(
				XMLoadFloat4(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst.x, my_dst1.x, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.y, my_dst1.y, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.z, my_dst1.z, TEST_EPSILON);
		BOOST_CHECK_CLOSE(dx_dst.w, my_dst1.w, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec4LengthSq)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[4];
		um_random(v, 4);

		float dx_dst;
		XMFLOAT4 dx_src_left(v[0], v[1], v[2], v[3]);

		UMVec4f my_src_left(v[0], v[1], v[2], v[3]);
		float my_dst1 = my_src_left.length_sq();

		XMStoreFloat(&dx_dst,
			XMVector4LengthSq(
				XMLoadFloat4(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_CASE(Vec4Length)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[4];
		um_random(v, 4);

		float dx_dst;
		XMFLOAT4 dx_src_left(v[0], v[1], v[2], v[3]);

		UMVec4f my_src_left(v[0], v[1], v[2], v[3]);
		float my_dst1 = my_src_left.length();

		XMStoreFloat(&dx_dst,
			XMVector4Length(
				XMLoadFloat4(&dx_src_left)));
		
		BOOST_CHECK_CLOSE(dx_dst, my_dst1, TEST_EPSILON);
	}
}

BOOST_AUTO_TEST_SUITE_END()

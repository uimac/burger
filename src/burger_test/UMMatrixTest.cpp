/**
 * @file UMMatrixTest.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license.
 *
 */
#include <boost/test/unit_test.hpp>

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <random>
#include "UMMatrix.h"
#include "UMVector.h"
#include "UMMath.h"

#define N 10
#define TEST_EPSILON 0.0005f

namespace
{
	std::mt19937 mt;
	std::uniform_real_distribution<> random_range(-2.0, 2.0);
	void um_random(float dst[], int size)
	{
		for (int i = 0; i < size; ++i) 
		{
			dst[i] = static_cast<float>(random_range(mt));
		}
	}
} // anonymouse namespace


BOOST_AUTO_TEST_SUITE(UMMatrixTest)
	
//using namespace DirectX;
using namespace burger;

//----------------------------------------------------------------------------
// Matrix44
//----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(MatrixMult)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[16];
		float v_right[16];
		um_random(v_left, 16);
		um_random(v_right, 16);

		XMFLOAT4X4 dx_dst;
		XMFLOAT4X4 dx_src_left(v_left);
		XMFLOAT4X4 dx_src_right(v_right);

		UMMat44f my_src_left(v_left);
		UMMat44f my_src_right(v_right);
		UMMat44f my_dst1 = my_src_left * my_src_right;
		//UMMat44f my_dst2 = my_src_left;
		//my_dst2 *= my_src_right;

		XMStoreFloat4x4(&dx_dst,
				XMMatrixMultiply(
					XMLoadFloat4x4(&dx_src_left),
					XMLoadFloat4x4(&dx_src_right)));
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixTranspose)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[16];
		um_random(v_left, 16);
		
		XMFLOAT4X4 dx_dst;
		XMFLOAT4X4 dx_src_left(v_left);
		
		UMMat44f my_src_left(v_left);
		UMMat44f my_dst1 = my_src_left.transposed();
		
		XMStoreFloat4x4(&dx_dst,
				XMMatrixTranspose(
					XMLoadFloat4x4(&dx_src_left)));
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}

	}
}

BOOST_AUTO_TEST_CASE(MatrixOrthoLH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[4];
		um_random(v_left, 4);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixOrthographicLH(v_left[0], v_left[1], v_left[2], v_left[3]));

		UMMat44f my_dst1;
		um_matrix_ortho_lh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixOrthoRH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[4];
		um_random(v_left, 4);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixOrthographicRH(v_left[0], v_left[1], v_left[2], v_left[3]));

		UMMat44f my_dst1;
		um_matrix_ortho_rh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixPerspectiveLH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[4];
		um_random(v_left, 4);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixPerspectiveLH(v_left[0], v_left[1], v_left[2], v_left[3]));

		UMMat44f my_dst1;
		um_matrix_perspective_lh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixPerspectiveRH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[4];
		um_random(v_left, 4);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixPerspectiveRH(v_left[0], v_left[1], v_left[2], v_left[3]));

		UMMat44f my_dst1;
		um_matrix_perspective_rh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixPerspectiveFovLH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[4];
		um_random(v_left, 4);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixPerspectiveFovLH(v_left[0], v_left[1], v_left[2], v_left[3]));

		UMMat44f my_dst1;
		um_matrix_perspective_fov_lh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixPerspectiveFovRH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[4];
		um_random(v_left, 4);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixPerspectiveFovRH(v_left[0], v_left[1], v_left[2], v_left[3]));

		UMMat44f my_dst1;
		um_matrix_perspective_fov_rh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixOrthoOffCenterLH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[6];
		um_random(v_left, 6);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixOrthographicOffCenterLH(v_left[0], v_left[1], v_left[2], v_left[3], v_left[4], v_left[5]));

		UMMat44f my_dst1;
		um_matrix_ortho_off_center_lh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3], v_left[4], v_left[5]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixOrthoOffCenterRH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[6];
		um_random(v_left, 6);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixOrthographicOffCenterRH(v_left[0], v_left[1], v_left[2], v_left[3], v_left[4], v_left[5]));

		UMMat44f my_dst1;
		um_matrix_ortho_off_center_rh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3], v_left[4], v_left[5]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixPersOffCenterLH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[6];
		um_random(v_left, 6);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixPerspectiveOffCenterLH(v_left[0], v_left[1], v_left[2], v_left[3], v_left[4], v_left[5]));

		UMMat44f my_dst1;
		um_matrix_perspective_off_center_lh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3], v_left[4], v_left[5]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixPersOffCenterRH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v_left[6];
		um_random(v_left, 6);
		
		XMFLOAT4X4 dx_dst;
		XMStoreFloat4x4(&dx_dst,
			XMMatrixPerspectiveOffCenterRH(v_left[0], v_left[1], v_left[2], v_left[3], v_left[4], v_left[5]));

		UMMat44f my_dst1;
		um_matrix_perspective_off_center_rh(my_dst1, v_left[0], v_left[1], v_left[2], v_left[3], v_left[4], v_left[5]);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixLookAtLH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[9];
		um_random(v, 9);
		
		XMFLOAT4X4 dx_dst;
		XMFLOAT3 dx_src_1(v[0], v[1], v[2]);
		XMFLOAT3 dx_src_2(v[3], v[4], v[5]);
		XMFLOAT3 dx_src_3(v[6], v[7], v[8]);

		XMStoreFloat4x4(&dx_dst,
			XMMatrixLookAtLH(
				XMLoadFloat3(&dx_src_1), 
				XMLoadFloat3(&dx_src_2), 
				XMLoadFloat3(&dx_src_3)));

		UMMat44f my_dst1;
		UMVec3f my_src_1(v[0], v[1], v[2]);
		UMVec3f my_src_2(v[3], v[4], v[5]);
		UMVec3f my_src_3(v[6], v[7], v[8]);
		um_matrix_look_at_lh(my_dst1, my_src_1, my_src_2, my_src_3);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixLookAtRH)
{
	for (int n = 0; n < N; ++n) 
	{
		float v[9];
		um_random(v, 9);
		
		XMFLOAT4X4 dx_dst;
		XMFLOAT3 dx_src_1(v[0], v[1], v[2]);
		XMFLOAT3 dx_src_2(v[3], v[4], v[5]);
		XMFLOAT3 dx_src_3(v[6], v[7], v[8]);

		XMStoreFloat4x4(&dx_dst,
			XMMatrixLookAtRH(
				XMLoadFloat3(&dx_src_1), 
				XMLoadFloat3(&dx_src_2), 
				XMLoadFloat3(&dx_src_3)));

		UMMat44f my_dst1;
		UMVec3f my_src_1(v[0], v[1], v[2]);
		UMVec3f my_src_2(v[3], v[4], v[5]);
		UMVec3f my_src_3(v[6], v[7], v[8]);
		um_matrix_look_at_rh(my_dst1, my_src_1, my_src_2, my_src_3);
		
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				BOOST_CHECK_CLOSE(dx_dst.m[k][j], my_dst1.m[k][j], TEST_EPSILON);
			}
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()

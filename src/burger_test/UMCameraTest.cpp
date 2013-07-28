/**
 * @file UMCameraTest.cpp
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


BOOST_AUTO_TEST_SUITE(UMCameraTest)
	
//using namespace DirectX;
using namespace burger;

//----------------------------------------------------------------------------
// UMCamera
//----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(CameraInit)
{
	UMMat44f view_matrix_;
	UMMat44f projection_matrix_;

	float aspect_ = (static_cast<float>(1280) / static_cast<float>(720));
	float fov_y_ = 45.0f;
	float near_ = 0.05f;
	float far_ = 1000.0f;
	UMVec3f position_ = UMVec3f(0, 0, 500);
	UMVec3f target_ = UMVec3f(0, 0, 0);
	UMVec3f up_ = UMVec3f(0, 1, 0);
	float view_plane_distance_ = 500;
	
	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
	um_matrix_perspective_fov_rh(projection_matrix_, um_to_radian(fov_y_), aspect_, near_, far_);
	
	XMFLOAT4X4 dx_projection;
	XMStoreFloat4x4(&dx_projection,
		XMMatrixPerspectiveFovRH(um_to_radian(fov_y_), aspect_, near_, far_));
	
	XMFLOAT3 dx_src_1(position_.x, position_.y, position_.z);
	XMFLOAT3 dx_src_2(target_.x, target_.y, target_.z);
	XMFLOAT3 dx_src_3(up_.x, up_.y, up_.z);

	XMFLOAT4X4 dx_view;
	XMStoreFloat4x4(&dx_view,
		XMMatrixLookAtRH(
			XMLoadFloat3(&dx_src_1), 
			XMLoadFloat3(&dx_src_2), 
			XMLoadFloat3(&dx_src_3)));
	
	for (int j = 0; j < 4; ++j) {
		for (int k = 0; k < 4; ++k) {
			BOOST_CHECK_CLOSE(dx_view.m[k][j], view_matrix_.m[k][j], TEST_EPSILON);
		}
	}

	for (int j = 0; j < 4; ++j) {
		for (int k = 0; k < 4; ++k) {
			BOOST_CHECK_CLOSE(dx_projection.m[k][j], projection_matrix_.m[k][j], TEST_EPSILON);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()

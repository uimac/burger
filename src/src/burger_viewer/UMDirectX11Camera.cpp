/**
 * @file UMDirectX11Camera.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11Camera.h"

namespace burger
{

UMDirectX11Camera::~UMDirectX11Camera()
{
}

/** 
 * initialize
 */
bool UMDirectX11Camera::init(ID3D11Device *device_pointer)
{
	if (!device_pointer) return false;
	UMCameraPtr camera = umcamera();
	if (!camera) return false;

	return true;
}

/**
 * refresh
 */
void UMDirectX11Camera::refresh(ID3D11Device* device_pointer)
{
	if (!device_pointer) return;
	
	UMCameraPtr camera = umcamera();
	if (!camera) return;
	
	camera->refresh();

	view_projection_transposed_ = view_projection_matrix().transposed();
	view_transposed_ = view_matrix().transposed();

	const UMVec3d& pos = camera->position();
	position_ = UMVec4f((float)pos.x, (float)pos.y, (float)pos.z, 0.0f);
}

/**
 * get view projection matrix
 */
UMMat44f UMDirectX11Camera::view_projection_matrix() const
{
	const UMMat44d& vp = umcamera()->view_projection_matrix();
	UMMat44f dst;
	for (int i = 0; i < 4; ++i) {
		for (int k = 0; k < 4; ++k) {
			dst.m[i][k] = static_cast<float>(vp.m[i][k]);
		}
	}
	return dst;
}

/**
 * get view matrix
 */
UMMat44f UMDirectX11Camera::view_matrix() const
{
	const UMMat44d& vp = umcamera()->view_matrix();
	UMMat44f dst;
	for (int i = 0; i < 4; ++i) {
		for (int k = 0; k < 4; ++k) {
			dst.m[i][k] = static_cast<float>(vp.m[i][k]);
		}
	}
	return dst;
}

} // burger

/**
 * @file UMCamera.cpp
 * a Camera
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMCamera.h"

#include "UMMath.h"
#include "UMRay.h"

namespace burger
{

void UMCamera::init(int width, int height)
{
	view_matrix_.identity();
	projection_matrix_.identity();
	view_projection_matrix_.identity();

	aspect_ = (static_cast<double>(width) / static_cast<double>(height));
	fov_y_ = 45.0;
	near_ = 1;
	far_ = 1000.0;
	position_ = UMVec3d(0, 0, 500);
	target_ = UMVec3d(0, 0, 0);
	up_ = UMVec3d(0, 1, 0);
	theta_ = 0.0;
	phi_ = 0.0;
	inverted_width_ = 1.0 / static_cast<double>(width);
	inverted_height_ = 1.0 / static_cast<double>(height);
	
	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
	//um_matrix_perspective_rh(projection_matrix_, (double)width, (double)height, near_, far_);
	um_matrix_perspective_fov_rh(projection_matrix_, um_to_radian(fov_y_), aspect_, near_, far_);

	refresh();
}

/**
 * generate a camera ray
 */
void UMCamera::generate_ray(UMRay& dst, int x, int y) const
{
	const UMMat44d& view_projection = view_projection_matrix();
	UMVec3d right (view_projection.m[0][0], view_projection.m[1][0], view_projection.m[2][0]);
	UMVec3d up (view_projection.m[0][1], view_projection.m[1][1], view_projection.m[2][1]);
	UMVec3d direction (view_projection.m[0][2], view_projection.m[1][2], view_projection.m[2][2]);
	
	double inv_yscale = tan(um_to_radian(fov_y() * 0.5));
	double inv_xscale = aspect() * inv_yscale;
	double plane_distance = 1.0 / inv_yscale;
	right *= inv_xscale;
	up *= inv_yscale;

	double xx = x * inverted_width_ * 2 - 1;
	double yy = y * inverted_height_ * 2 - 1;
	UMVec3d dir = right * xx * aspect() + up * yy + direction * plane_distance;

	dst.set_origin(position());
	dst.set_direction(dir.normalized());
}

/**
 * rotate camera
 */
void UMCamera::rotate(double mx, double my)
{
	if (abs((int)mx) <= 2 && abs((int)my) <= 2) return;

	theta_ -= mx;
	phi_ -= my;
	if ( phi_ >= 90.0f ) phi_ = 89.0f;
	if ( phi_ <= -90.0f ) phi_ = -89.0f;
	
	double radius = position_.length();
	double t = um_to_radian(theta_);
	double p = um_to_radian(phi_);

	position_.x = -radius * sin(t) * cos(p);
	position_.y = radius * sin(p);
	position_.z = radius * cos(t) * cos(p);

	//view_matrix_.identity();
	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
}

/**
 * zoom camera
 */
void UMCamera::zoom(double mx, double my)
{
	if (abs((int)mx) <= 2 && abs((int)my) <= 2) return;

	fov_y_ -= my * 0.2;

	if (fov_y_ > 175) {
		fov_y_ = 175;
	}
	if (fov_y_ < 5) {
		fov_y_ = 5;
	}

	um_matrix_perspective_fov_rh(projection_matrix_, um_to_radian(fov_y_), aspect_, near_, far_);
}

} // burger

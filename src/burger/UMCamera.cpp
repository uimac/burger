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
	zoom_event_ = std::make_shared<UMEvent>(eEventTypeCameraZoom);
	mutable_event_list().push_back(zoom_event_);
	dolly_event_ = std::make_shared<UMEvent>(eEventTypeCameraDolly);
	mutable_event_list().push_back(dolly_event_);

	view_matrix_.identity();
	projection_matrix_.identity();
	view_projection_matrix_.identity();
	generate_ray_x_scale_ = UMVec3d(0);
	generate_ray_y_scale_ = UMVec3d(0);
	generate_ray_adder_ = UMVec3d(0);

	aspect_ = (static_cast<double>(width) / static_cast<double>(height));
	fov_y_ = 45.0;
	near_ = 1;
	far_ = 500.0;
	position_ = UMVec3d(0,  15, 50);
	target_ = UMVec3d(0, 15, 0);
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
void UMCamera::generate_ray(UMRay& dst, const UMVec2d& sample_point, bool is_initial)
{
	if (is_initial)
	{
		const UMMat44d& view_projection = view_projection_matrix();
		UMVec3d right (view_projection.m[0][0], view_projection.m[1][0], view_projection.m[2][0]);
		UMVec3d up (view_projection.m[0][1], view_projection.m[1][1], view_projection.m[2][1]);
		UMVec3d direction (view_projection.m[0][2], view_projection.m[1][2], view_projection.m[2][2]);
	
		double inv_yscale = tan(um_to_radian(fov_y() * 0.5));
		double inv_xscale = aspect() * inv_yscale;
		right *= inv_xscale;
		up *= inv_yscale;
		generate_ray_x_scale_ = right * aspect();
		generate_ray_y_scale_ = up;
		generate_ray_adder_ = direction / inv_yscale;
	}

	double xx = sample_point.x * inverted_width_ * 2 - 1;
	double yy = sample_point.y * inverted_height_ * 2 - 1;
	UMVec3d dir = generate_ray_x_scale_ * xx + generate_ray_y_scale_ * yy + generate_ray_adder_;

	dst.set_origin(position());
	dst.set_direction(dir.normalized());
}

/**
 * rotate camera
 */
void UMCamera::rotate(double mx, double my)
{
	if (fabs(mx) <= 2 && fabs(my) <= 2) return;

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
	if (fabs(mx) <= 2 && fabs(my) <= 2) return;

	fov_y_ -= my * 0.2;

	if (fov_y_ > 175) {
		fov_y_ = 175;
	}
	if (fov_y_ < 5) {
		fov_y_ = 5;
	}

	um_matrix_perspective_fov_rh(projection_matrix_, um_to_radian(fov_y_), aspect_, near_, far_);

	UMAny fovy(fov_y_);
	zoom_event_->set_parameter(fovy);
	zoom_event_->notify();
}

/**
 * dolly camera
 * @param [in] mx x-axis mouse move
 * @param [in] my y-axis mouse move
 */
void UMCamera::dolly(double mx, double my)
{
	if (fabs(mx) <= 2 && fabs(my) <= 2) return;
	
	double dolly_value = -my * 0.2;
		
	double radius = position_.length() + dolly_value;
	double t = um_to_radian(theta_);
	double p = um_to_radian(phi_);

	position_.x = -radius * sin(t) * cos(p);
	position_.y = radius * sin(p);
	position_.z = radius * cos(t) * cos(p);

	//view_matrix_.identity();
	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
		
	double position_length = position_.length();
	UMAny length(position_length);
	dolly_event_->set_parameter(length);
	dolly_event_->notify();
}

} // burger

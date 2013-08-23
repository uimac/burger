/**
 * @file UMAreaLight.cpp
 * an area light
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMAreaLight.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"

#include <random>

namespace burger
{

/**
 * get intensity from previous sample
 */
UMVec3d UMAreaLight::intensity(const UMVec3d& point) const
{
	double r = 0;
	if (linear_fall_off_ != 0 || quadric_fall_off_ != 0)
	{
		r = (last_sample_point_ - point).length();
	}
	double constant = constant_fall_off_;
	double linear = linear_fall_off_ * r;
	double quadric = quadric_fall_off_ * r * r;
	return color() * area_ / (constant + linear + quadric);
}

/** 
 * sample a point
 */
bool UMAreaLight::sample(UMVec3d& intensity, UMVec3d& point, UMVec3d& direction, const UMShaderParameter& parameter, const UMVec2d& random_value)
{
	UMVec3d sample_point(edge1_ * random_value.x + edge2_ * random_value.y + position());
	direction = sample_point - parameter.intersect_point;
	double direction_length_inv = 1.0 / direction.length();
	double cos_theta_in = std::max( parameter.normal.dot(direction) * direction_length_inv, 0.0 );
	double cos_theta_out = std::max( normal_.dot(-direction) * direction_length_inv, 0.0 );
	double factor = cos_theta_in * cos_theta_out * direction_length_inv * direction_length_inv;
	intensity = color() * factor * area_;
	point = sample_point;

	// save sample point
	last_sample_point_ = sample_point;
	return true;
}


}
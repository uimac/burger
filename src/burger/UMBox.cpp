/**
 * @file UMBox.cpp
 * a plane
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMBox.h"
#include "UMMath.h"
#include "UMRay.h"
#include "UMShaderParameter.h"
#include "UMMesh.h"

#include <cmath>
#include <cfloat>
#include <limits>
#include <assert.h>


namespace burger
{
	
/** 
 * extend box by point
 */
void UMBox::extend(const UMVec3d& p)
{
	if (p.x < min_.x) min_.x = p.x;
	if (p.y < min_.y) min_.y = p.y;
	if (p.z < min_.z) min_.z = p.z;
	if (p.x > max_.x) max_.x = p.x;
	if (p.y > max_.y) max_.y = p.y;
	if (p.z > max_.z) max_.z = p.z;
}

/**
 * ray AABB intersection
 */
bool UMBox::intersects(const UMRay& ray, UMShaderParameter& parameter) const
{
	if (is_empty()) return false;
	const UMVec3d& org = ray.origin();
	const UMVec3d& dir = ray.direction();
	double tmin = 0.0;
	double tmax = std::numeric_limits<double>::max();
	for (int i = 0; i < 3; ++i)
	{
		if (fabs(dir[i]) < FLT_EPSILON) {
			if (org[i] < min_[i] || org[i] > max_[i]) return false;
		}
		else
		{
			double inv_dir = 1.0 / dir[i];
			double t1 = (min_[i] - org[i]) * inv_dir;
			double t2 = (max_[i] - org[i]) * inv_dir;
			if (t1 > t2) { std::swap(t1, t2); }
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;
			if (tmin > tmax) return false;
		}
	}
	parameter.distance = tmin;
	parameter.intersect_point = org + dir * parameter.distance;
	parameter.normal = normal(parameter.intersect_point);
	parameter.color = color_;
	return true;
}

/**
 * ray AABB intersection
 */
bool UMBox::intersects(const UMRay& ray) const
{
	if (is_empty()) return false;
	const UMVec3d& org = ray.origin();
	const UMVec3d& dir = ray.direction();
	double tmin = 0.0;
	double tmax = std::numeric_limits<double>::max();
	for (int i = 0; i < 3; ++i)
	{
		if (fabs(dir[i]) < FLT_EPSILON) {
			if (org[i] < min_[i] || org[i] > max_[i]) return false;
		}
		else
		{
			double inv_dir = 1.0 / dir[i];
			double t1 = (min_[i] - org[i]) * inv_dir;
			double t2 = (max_[i] - org[i]) * inv_dir;
			if (t1 > t2) { std::swap(t1, t2); }
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;
			if (tmin > tmax) return false;
		}
	}
	return true;
}

/**
 * get normal at point
 */
UMVec3d UMBox::normal(const UMVec3d& point) const
{
	UMVec3d normal;
	UMVec3d center_to_point = point - center();
	const UMVec3d& size_ = size();
	double min = std::numeric_limits<double>::max();
	double distance = std::abs(size_.x - std::abs(center_to_point.x));
	if (distance < min) {
		min = distance;
		normal.x = 1;
		normal *= um_sign(center_to_point.x);
	}
	distance = std::abs(size_.y - std::abs(center_to_point.y));
	if (distance < min) {
		min = distance;
		normal.y = 1;
		normal *= um_sign(center_to_point.y);
	}
	distance = std::abs(size_.z - std::abs(center_to_point.z));
	if (distance < min) {
		min = distance;
		normal.z = 1;
		normal *= um_sign(center_to_point.z);
	}
	return normal;
}

/**
 * convert to plane mesh
 * TODO: direction
 */
UMMeshPtr UMBox::convert_to_mesh() const
{
	UMMeshPtr mesh(std::make_shared<UMMesh>());

	// TODO !
	assert(0);

	UMMaterialPtr mat = UMMaterial::default_material();
	mat->set_diffuse(UMVec4d(color_, 1.0));
	mat->set_polygon_count(static_cast<int>(mesh->face_list().size()));
	mesh->mutable_material_list().push_back(mat);
	mesh->create_normals(true);
	return mesh;
}

} // burger

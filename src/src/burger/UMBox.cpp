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
	set_minimum(UMVec3d(
		std::min(minimum().x, p.x),
		std::min(minimum().y, p.y),
		std::min(minimum().z, p.z)));
	set_maximum(UMVec3d(
		std::max(maximum().x, p.x),
		std::max(maximum().y, p.y),
		std::max(maximum().z, p.z)));
}

/**
 * get union box
 */
void UMBox::extend(const UMBox& box)
{
	set_minimum(UMVec3d(
		std::min(minimum().x, box.minimum().x),
		std::min(minimum().y, box.minimum().y),
		std::min(minimum().z, box.minimum().z)));
	set_maximum(UMVec3d(
		std::max(maximum().x, box.maximum().x),
		std::max(maximum().y, box.maximum().y),
		std::max(maximum().z, box.maximum().z)));
}

/**
 * ray AABB intersection
 */
bool UMBox::intersects(const UMRay& ray, UMShaderParameter& parameter) const
{
	//if (is_empty()) return false;
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
	parameter.color = UMVec3d(0.7, 0.7, 0.7);
	return true;
}

/**
 * ray AABB intersection
 */
bool UMBox::intersects(const UMRay& ray) const
{
	//if (is_empty()) return false;
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
 * ray AABB intersection
 */
bool UMBox::intersects(const UMRay& ray, UMVec2d& tmin_tmax) const
{
	//if (is_empty()) return false;
	const UMVec3d& org = ray.origin();
	const UMVec3d& dir = ray.direction();
	UMVec3d vtmin = FLT_EPSILON;
	UMVec3d vtmax = std::numeric_limits<double>::max();
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
			if (t1 > vtmin[i]) vtmin[i] = t1;
			if (t2 < vtmax[i]) vtmax[i] = t2;
		}
	}
	tmin_tmax.x = std::max(std::max(vtmin.x, vtmin.y), vtmin.z);
	tmin_tmax.y = std::max(std::max(vtmax.x, vtmax.y), vtmax.z);
	return tmin_tmax.y >= tmin_tmax.x;
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
 * convert to box mesh
 */
UMMeshPtr UMBox::convert_to_mesh() const
{
	UMMeshPtr mesh(std::make_shared<UMMesh>());

	UMVec3d d = max_ - min_;
	
	//   7 -- 6
	//  /|   /| 
	// 2 3- 4 5
	// |/   |/
	// 0 -- 1
	UMVec3d v0(min_.x      , min_.y      , min_.z);
	UMVec3d v1(min_.x + d.x, min_.y      , min_.z);
	UMVec3d v2(min_.x      , min_.y + d.y, min_.z);
	UMVec3d v3(min_.x      , min_.y      , min_.z + d.z);
	UMVec3d v4(min_.x + d.x, min_.y + d.y, min_.z);
	UMVec3d v5(min_.x + d.x, min_.y      , min_.z + d.z);
	UMVec3d v6(min_.x + d.x, min_.y + d.y, min_.z + d.z);
	UMVec3d v7(min_.x      , min_.y + d.y, min_.z + d.z);
	
	mesh->mutable_vertex_list().reserve(8);
	mesh->mutable_vertex_list().push_back(v0);
	mesh->mutable_vertex_list().push_back(v1);
	mesh->mutable_vertex_list().push_back(v2);
	mesh->mutable_vertex_list().push_back(v3);
	mesh->mutable_vertex_list().push_back(v4);
	mesh->mutable_vertex_list().push_back(v5);
	mesh->mutable_vertex_list().push_back(v6);
	mesh->mutable_vertex_list().push_back(v7);

	mesh->mutable_face_list().push_back(UMVec3i(0, 1, 2));
	mesh->mutable_face_list().push_back(UMVec3i(2, 1, 4));
	mesh->mutable_face_list().push_back(UMVec3i(4, 1, 5));
	mesh->mutable_face_list().push_back(UMVec3i(5, 6, 4));
	mesh->mutable_face_list().push_back(UMVec3i(2, 4, 7));
	mesh->mutable_face_list().push_back(UMVec3i(7, 4, 6));
	mesh->mutable_face_list().push_back(UMVec3i(6, 5, 5));
	mesh->mutable_face_list().push_back(UMVec3i(6, 3, 7));
	mesh->mutable_face_list().push_back(UMVec3i(7, 3, 2));
	mesh->mutable_face_list().push_back(UMVec3i(2, 3, 0));

	UMMaterialPtr mat = UMMaterial::default_material();
	mat->set_polygon_count(static_cast<int>(mesh->face_list().size()));
	mesh->mutable_material_list().push_back(mat);
	mesh->create_normals(true);
	return mesh;
}

/**
 * is overlap
 */
bool UMBox::is_overlap(const UMBox& box)
{
	if (box.minimum().length_sq() < max_.length_sq() &&
		box.maximum().length_sq() > min_.length_sq())
	{
		return true;
	}
	return false;
}

} // burger

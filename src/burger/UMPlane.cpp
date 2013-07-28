/**
 * @file UMPlane.cpp
 * a plane
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMPlane.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"
#include "UMMesh.h"

#include <cmath>
#include <cfloat>


namespace burger
{

/**
 * ray plane intersection
 */
bool UMPlane::intersects(const UMRay& ray, UMShaderParameter& parameter) const
{
	UMVec3d ray_dir(ray.direction());
	UMVec3d ray_orig(ray.origin());

	double angle = normal_.dot(ray.direction());
	if (fabs(angle) < FLT_EPSILON) {
		// ray is parallel
		return false;
	}

	double distance = normal_.dot(point_ - ray_orig) / angle;
	if (distance < FLT_EPSILON) {
		// ray is back of plane
		return false;
	}
	
	parameter.color = color_;
	parameter.distance = distance;
	parameter.intersect_point = ray_orig + ray_dir * distance;
	parameter.normal = normal_;

	return true;
}

/**
 * ray plane intersection
 */
bool UMPlane::intersects(const UMRay& ray) const
{
	UMVec3d ray_dir(ray.direction());
	UMVec3d ray_orig(ray.origin());

	double angle = normal_.dot(ray.direction());
	if (fabs(angle) < FLT_EPSILON) {
		// ray is parallel
		return false;
	}

	double distance = normal_.dot(point_ - ray.origin()) / angle;
	if (distance < FLT_EPSILON) {
		// ray is back of plane
		return false;
	}
	return true;
}

/**
 * convert to plane mesh
 * TODO: direction
 */
UMMeshPtr UMPlane::convert_to_mesh(double width, double height) const
{
	UMMeshPtr mesh(std::make_shared<UMMesh>());
	const double hw = width / 2.0;
	const double hh = height / 2.0;

	// front +z
	// v0--v1
	//  |   |
	// v2--v3
	UMVec3d v0 = UMVec3d(-hw, 0, -hh) + point();
	UMVec3d v1 = UMVec3d( hw, 0, -hh) + point();
	UMVec3d v2 = UMVec3d(-hw, 0,  hh) + point();
	UMVec3d v3 = UMVec3d( hw, 0,  hh) + point();
	mesh->mutable_vertex_list().push_back(v0);
	mesh->mutable_vertex_list().push_back(v1);
	mesh->mutable_vertex_list().push_back(v2);
	mesh->mutable_vertex_list().push_back(v3);
	mesh->mutable_normal_list().push_back(normal());
	mesh->mutable_normal_list().push_back(normal());
	mesh->mutable_normal_list().push_back(normal());
	mesh->mutable_normal_list().push_back(normal());
	mesh->mutable_face_list().push_back(UMVec3i(0, 2, 1));
	mesh->mutable_face_list().push_back(UMVec3i(2, 3, 1));

	UMMaterialPtr mat = UMMaterial::default_material();
	mat->set_diffuse(UMVec4d(color_, 1.0));
	mat->set_polygon_count(static_cast<int>(mesh->face_list().size()));
	mesh->mutable_material_list().push_back(mat);
	mesh->create_normals(true);
	mesh->update_box_by_vertex();
	return mesh;
}

} // burger

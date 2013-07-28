/**
 * @file UMTriangle.cpp
 * a plane
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMTriangle.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"

#include <cmath>
#include <cfloat>

namespace burger
{
	

/**
 * ray triangle intersection static version
 */
bool UMTriangle::intersects(
	const UMVec3d& a,
	const UMVec3d& b,
	const UMVec3d& c,
	const UMRay& ray,
	UMShaderParameter& parameter)
{
	
	UMVec3d ray_dir(ray.direction());
	UMVec3d ray_orig(ray.origin());
	
	UMVec3d ab = b - a;
	UMVec3d ac = c - a;
	UMVec3d n = ab.cross(ac);

	// ray is parallel or no reach
	double d = (-ray_dir).dot(n);
	if (d < FLT_EPSILON) return false;
	
	UMVec3d ao = ray_orig - a;
	double t = ao.dot(n);
	if (t < 0) return false;

	// inside triangle ?
	UMVec3d bycentric = (-ray_dir).cross(ao);
	double v = ac.dot(bycentric);
	if (v < 0 || v > d) return false;
	double w = -ab.dot(bycentric);
	if (w < 0 || (v + w) > d) return false;

	double inv_dir = 1.0 / d;
	// v
	parameter.uvw.y = v * inv_dir;
	// w
	parameter.uvw.z = w * inv_dir;
	// u
	parameter.uvw.x = 1.0 - parameter.uvw.y - parameter.uvw.z;

	parameter.distance = t * inv_dir;
	parameter.intersect_point = ray_orig + ray_dir * parameter.distance;
	parameter.normal = n.normalized();

	return true;
}

/**
 * ray triangle intersection
 */
bool UMTriangle::intersects(const UMRay& ray, UMShaderParameter& parameter) const
{
	// 3 points
	UMVec3d a( mesh_->vertex_list().at(index_.x) );
	UMVec3d b( mesh_->vertex_list().at(index_.y) );
	UMVec3d c( mesh_->vertex_list().at(index_.z) );

	if (intersects(a, b, c, ray, parameter))
	{
		parameter.color = color_;
		return true;
	}
	return false;
}


/**
 * ray triangle intersection static version
 */
bool UMTriangle::intersects(
	const UMVec3d& a,
	const UMVec3d& b,
	const UMVec3d& c,
	const UMRay& ray)
{
	UMVec3d ray_dir(ray.direction());
	UMVec3d ray_orig(ray.origin());
	
	UMVec3d ab = b - a;
	UMVec3d ac = c - a;
	UMVec3d n = ab.cross(ac);

	// ray is parallel or no reach
	double d = (-ray_dir).dot(n);
	if (d < FLT_EPSILON) return false;
	
	UMVec3d ao = ray_orig - a;
	double t = ao.dot(n);
	if (t < FLT_EPSILON) return false;

	// inside triangle ?
	UMVec3d bycentric = (-ray_dir).cross(ao);
	double v = ac.dot(bycentric);
	if (v < FLT_EPSILON || v > d) return false;
	double w = -ab.dot(bycentric);
	if (w < FLT_EPSILON || v + w > d) return false;

	return true;
}

/**
 * ray triangle intersection
 */
bool UMTriangle::intersects(const UMRay& ray) const
{
	// 3 points
	UMVec3d a( mesh_->vertex_list().at(index_.x) );
	UMVec3d b( mesh_->vertex_list().at(index_.y) );
	UMVec3d c( mesh_->vertex_list().at(index_.z) );

	return intersects(a, b, c, ray);
}

} // burger

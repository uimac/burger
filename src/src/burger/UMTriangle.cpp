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
	
	const static double kEPSILON = DBL_EPSILON;

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
	
	const UMVec3d& ray_dir(ray.direction());
	const UMVec3d& ray_orig(ray.origin());
	
	UMVec3d ab = b - a;
	UMVec3d ac = c - a;
	UMVec3d n = ab.cross(ac);

	// ray is parallel or no reach
	double d = (-ray_dir).dot(n);
	if (d < 0) return false;
	
	UMVec3d ao = ray_orig - a;
	double t = ao.dot(n);
	if (t < 0) return false;

	double inv_dir = 1.0 / d;
	double distance = t * inv_dir;
	if (distance < ray.tmin()) return false;
	if (distance > ray.tmax()) return false;

	// inside triangle ?
	UMVec3d barycentric = (-ray_dir).cross(ao);
	double v = ac.dot(barycentric);
	if (v < 0 || v > d) return false;
	double w = -ab.dot(barycentric);
	if (w < 0 || (v + w) > d) return false;

	// v
	parameter.uvw.y = v * inv_dir;
	// w
	parameter.uvw.z = w * inv_dir;
	// u
	parameter.uvw.x = 1.0 - parameter.uvw.y - parameter.uvw.z;
	
	parameter.distance = distance;
	parameter.intersect_point = ray_orig + ray_dir * distance;
	//parameter.normal = n.normalized();

	return true;
}

/**
 * ray triangle intersection
 */
bool UMTriangle::intersects(const UMRay& ray, UMShaderParameter& parameter) const
{
	// 3 points
	const UMVec3d& v0 = mesh_->vertex_list()[vertex_index_.x];
	const UMVec3d& v1 = mesh_->vertex_list()[vertex_index_.y];
	const UMVec3d& v2 = mesh_->vertex_list()[vertex_index_.z];

	if (intersects(v0, v1, v2, ray, parameter))
	{
		const UMVec3d& n0 = mesh_->normal_list()[vertex_index_.x];
		const UMVec3d& n1 = mesh_->normal_list()[vertex_index_.y];
		const UMVec3d& n2 = mesh_->normal_list()[vertex_index_.z];
		parameter.normal = (n0 * parameter.uvw.x + n1 * parameter.uvw.y + n2 * parameter.uvw.z).normalized();

		if (UMMaterialPtr material = mesh_->material_from_face_index(face_index_))
		{
			parameter.material = material;

			const UMVec4d& diffuse = material->diffuse();
			parameter.color.x = diffuse.x;
			parameter.color.y = diffuse.y;
			parameter.color.z = diffuse.z;
			parameter.emissive = material->emissive().xyz() * material->emissive_factor();
			if (!mesh_->uv_list().empty() && !material->texture_list().empty()) {
				// uv
				const int base = face_index_ * 3;
				const UMVec2d& uv0 = mesh_->uv_list()[base + 0];
				const UMVec2d& uv1 = mesh_->uv_list()[base + 1];
				const UMVec2d& uv2 = mesh_->uv_list()[base + 2];
				UMVec2d uv = UMVec2d(
					uv0 * parameter.uvw.x +
					uv1 * parameter.uvw.y +
					uv2 * parameter.uvw.z);
				uv.x = um_clip(uv.x);
				uv.y = um_clip(uv.y);
				const UMImagePtr texture = material->texture_list()[0];
				const int x = static_cast<int>(texture->width() * uv.x);
				const int y = static_cast<int>(texture->height() * uv.y);
				const int pixel = y * texture->width() + x;
				const UMVec4d& pixel_color = texture->list()[pixel];
				parameter.color.x *= pixel_color.x;
				parameter.color.y *= pixel_color.y;
				parameter.color.z *= pixel_color.z;
			}
		}
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
	const UMVec3d& ray_dir = ray.direction();
	const UMVec3d& ray_orig = ray.origin();
	
	UMVec3d ab = b - a;
	UMVec3d ac = c - a;
	UMVec3d n = ab.cross(ac);

	// ray is parallel or no reach
	double d = (-ray_dir).dot(n);
	if (d < 0) return false;
	
	UMVec3d ao = ray_orig - a;
	double t = ao.dot(n);
	if (t < 0) return false;
	
	double inv_dir = 1.0 / d;
	double distance = t * inv_dir;
	if (distance < FLT_EPSILON) return false;

	// inside triangle ?
	UMVec3d barycentric = (-ray_dir).cross(ao);
	double v = ac.dot(barycentric);
	if (v < 0 || v > d) return false;
	double w = -ab.dot(barycentric);
	if (w < 0 || (v + w) > d) return false;

	return true;
}

/**
 * ray triangle intersection
 */
bool UMTriangle::intersects(const UMRay& ray) const
{
	// 3 points
	const UMVec3d& v0 = mesh_->vertex_list()[vertex_index_.x];
	const UMVec3d& v1 = mesh_->vertex_list()[vertex_index_.y];
	const UMVec3d& v2 = mesh_->vertex_list()[vertex_index_.z];

	return intersects(v0, v1, v2, ray);
}

/**
 * update box
 */
void UMTriangle::update_box()
{
	box_.init();
	const UMVec3d& v0 = mesh_->vertex_list()[vertex_index_.x];
	const UMVec3d& v1 = mesh_->vertex_list()[vertex_index_.y];
	const UMVec3d& v2 = mesh_->vertex_list()[vertex_index_.z];
	box_.extend(v0);
	box_.extend(v1);
	box_.extend(v2);
}

} // burger

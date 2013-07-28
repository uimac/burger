/**
 * @file UMSphere.cpp
 * a sphere
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMSphere.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"
#include "UMMesh.h"

#include <cmath>
#include <cfloat>

namespace burger
{

/**
 * ray sphere intersection
 */
bool UMSphere::intersects(const UMRay& ray, UMShaderParameter& parameter) const
{
	UMVec3d ray_dir(ray.direction());
	UMVec3d ray_orig(ray.origin());

	UMVec3d center_to_ray = ray_orig - center_;
	double a = ray_dir.dot(ray_dir);
	double b = 2 * ray_dir.dot(center_to_ray);
	double c = center_to_ray.dot(center_to_ray) - radius_ * radius_;
	double d = b * b - 4 * a * c;

	if (d < 0) {
		// not found solution
		return false;
	}

	double e = sqrt(d);
	double distance = (-b - e) / (2 * a);
	if (distance > FLT_EPSILON) {
		// hit
		parameter.color = color_;
		parameter.distance = distance;
		parameter.intersect_point = ray_orig + ray_dir * distance;
		parameter.normal = (parameter.intersect_point - center_) / radius_;
		return true;
	}

	distance = (-b + e) / (2 * a);
	if (distance > FLT_EPSILON) {
		// hit
		parameter.color = color_;
		parameter.distance = distance;
		parameter.intersect_point = ray_orig + ray_dir * distance;
		parameter.normal = (parameter.intersect_point - center_) / radius_;
		return true;
	}

	return false;
}

/**
 * ray sphere intersection
 */
bool UMSphere::intersects(const UMRay& ray) const
{
	UMVec3d ray_dir(ray.direction());
	UMVec3d ray_orig(ray.origin());

	UMVec3d center_to_ray = ray_orig - center_;
	double a = ray_dir.dot(ray_dir);
	double b = 2 * ray_dir.dot(center_to_ray);
	double c = center_to_ray.dot(center_to_ray) - radius_ * radius_;
	double d = b * b - 4 * a * c;

	if (d < 0) {
		// not found solution
		return false;
	}

	double e = sqrt(d);
	double distance = (-b - e) / (2 * a);
	if (distance > FLT_EPSILON) {
		// hit
		return true;
	}

	distance = (-b + e) / (2 * a);
	if (distance > FLT_EPSILON) {
		// hit
		return true;
	}

	return false;
}

/**
 * convert to uv sphere mesh
 */
UMMeshPtr UMSphere::convert_to_mesh(int stacks, int slices) const
{
	UMMeshPtr mesh(std::make_shared<UMMesh>());
	const double half_pi = M_PI / 2.0;
	const double pi_par_stack = M_PI / (double)stacks;
	const double pi_par_slice = M_PI / (double)slices;
	
	for (int i = 0; i < stacks + 1; ++i)
	{
		double latitude1 = pi_par_stack * i - half_pi;
		double sin_lat1 = sin(latitude1);
		double cos_lat1 = cos(latitude1);

		int ksize = slices;
		if (i == 0 || i == stacks ) { ksize = 1; }
		
		for (int k = 0; k < ksize; ++k)
		{
			const double longitude = 2 * pi_par_slice * k;
			const double sin_lon = sin(longitude);
			const double cos_lon = cos(longitude);
			UMVec3d factor1(
				cos_lon * cos_lat1,
				sin_lon * cos_lat1,
				sin_lat1);

			UMVec3d va = factor1 * radius() + center();
			mesh->mutable_vertex_list().push_back(va);
			const int pos_va = static_cast<int>(mesh->vertex_list().size() - 1);

			int pos_vb = pos_va - slices;
			if (i == 0) continue;
			if (i == 1)
			{
				pos_vb = pos_va - k - 1;
				if (k < (slices-1))
				{
					UMVec3i face1(pos_va, pos_vb, pos_va + 1);
					mesh->mutable_face_list().push_back(face1);
				}
				else
				{
					UMVec3i face1(pos_va, pos_vb, pos_va - k);
					mesh->mutable_face_list().push_back(face1);
				}
			}
			else if (i == stacks)
			{
				for (int n = 0; n < slices; ++n)
				{
					pos_vb = pos_va - slices + n;
					if (n < (slices-1))
					{
						UMVec3i face(pos_va, pos_vb, pos_vb + 1);
						mesh->mutable_face_list().push_back(face);
					}
					else
					{
						UMVec3i face(pos_va, pos_vb, pos_vb - n);
						mesh->mutable_face_list().push_back(face);
					}
				}
			}
			else
			{
				if (k < (slices-1))
				{
					UMVec3i face1(pos_va, pos_vb, pos_va + 1);
					UMVec3i face2(pos_va + 1, pos_vb, pos_vb + 1);
					mesh->mutable_face_list().push_back(face1);
					mesh->mutable_face_list().push_back(face2);
				}
				else
				{
					UMVec3i face1(pos_va, pos_vb, pos_va - k);
					UMVec3i face2(pos_va - k, pos_vb, pos_vb - k);
					mesh->mutable_face_list().push_back(face1);
					mesh->mutable_face_list().push_back(face2);
				}
			}
		}
	}
	UMMaterialPtr mat = UMMaterial::default_material();
	mat->set_diffuse(UMVec4d(color_, 1.0));
	mat->set_polygon_count(static_cast<int>(mesh->face_list().size()));
	mesh->mutable_material_list().push_back(mat);
	mesh->create_normals(true);
	mesh->update_box_by_vertex();
	return mesh;
}

} // burger

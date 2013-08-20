/**
 * @file UMRayTracer.cpp
 * a raytracer
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMRayTracer.h"
#include "UMRenderParameter.h"
#include "UMShaderParameter.h"
#include "UMRay.h"
#include "UMScene.h"
#include "UMVector.h"

#include <limits>
#include <algorithm>

namespace
{
	using namespace burger;

	// definition
	UMVec3d trace(const UMRay& ray, const UMScene& scene, UMShaderParameter& parameter);

	UMVec3d map_one(UMVec3d src) {
		double max = std::max(src.x, std::max(src.y, src.z));
		if (max > 1.0) {
			return src / max;
		}
		return src;
	}

	UMVec3d reflect(const UMRay& ray, const UMVec3d& normal)
	{
		UMVec3d in_dir(-ray.direction());
		return -in_dir + normal * in_dir.dot(normal) * 2.0;
	}
	
	class UMIntersection
	{
		DISALLOW_COPY_AND_ASSIGN(UMIntersection);
		public:
			UMIntersection() : 
				closest_distance(std::numeric_limits<double>::max())
			{}

			double closest_distance;
			UMPrimitivePtr closest_primitive;
			UMShaderParameter closest_parameter;
	};

	bool intersect(
		const UMRay& ray, 
		const UMScene& scene, 
		UMShaderParameter& parameter, 
		UMIntersection& intersection)
	{
		UMPrimitiveList::const_iterator it = scene.primitive_list().begin();
		for (int i = 0; it != scene.primitive_list().end(); ++it, ++i)
		{
			UMPrimitivePtr primitive = *it;
			if (primitive->intersects(ray, parameter))
			{
				if (parameter.distance < intersection.closest_distance) 
				{
					intersection.closest_distance = parameter.distance;
					intersection.closest_primitive = primitive;
					intersection.closest_parameter = parameter;
				}
			}
		}
		return intersection.closest_primitive;
	}

	/**
	 * shading function
	 */
	UMVec3d shade(const UMPrimitivePtr current, const UMRay& ray, const UMScene& scene, UMShaderParameter& parameter)
	{
		UMVec3d light_position = scene.light_list().at(0)->position();
		UMVec3d shadow_dir = -(parameter.intersect_point - light_position ).normalized();
		UMVec3d normal(parameter.normal);

		//// reflection ray
		//if (parameter.bounce > 0)
		//{
		//	--parameter.bounce;
		//	UMVec3d refrection_dir = reflect(ray, normal);
		//	UMRay reflection_ray(parameter.intersect_point, refrection_dir);
		//	UMVec3d original_color(parameter.color);
		//	UMVec3d color = trace(reflection_ray, scene, parameter);
		//	UMVec3d nl = parameter.normal.dot(refrection_dir);
		//	return UMVec3d(
		//		original_color.x * color.x * nl.x,
		//		original_color.y * color.y * nl.y,
		//		original_color.z * color.z * nl.z);
		//}

		//// shadow ray
		//UMRay shadow_ray(parameter.intersect_point, shadow_dir);
		//UMIntersection intersection;
		//UMShaderParameter shadow_parameter;
		//if (intersect(shadow_ray, scene, shadow_parameter, intersection))
		//{
		//	return UMVec3d(0);
		//}

		return map_one(parameter.color * (normal.dot(shadow_dir)) * 0.5);
	}

	/**
	 * trace and return color of the hit point
	 */
	UMVec3d trace(const UMRay& ray, const UMScene& scene, UMShaderParameter& parameter)
	{
		UMIntersection intersection;
		if (!intersect(ray, scene, parameter, intersection)){
			return scene.background_color();
		}

		if (intersection.closest_primitive)
		{
			return shade(intersection.closest_primitive, ray, scene, intersection.closest_parameter);
		}
		return scene.background_color();
	}
}

namespace burger
{

/**
 * render
 */
bool UMRayTracer::render(const UMScene& scene, UMRenderParameter& parameter)
{
	if (width_ == 0 || height_ == 0) return false;
	if (!scene.camera()) return false;
	
	const int sample_count = parameter.super_sampling_count().x * parameter.super_sampling_count().y;
	const double inv_sample_count = 1.0 / sample_count;

	UMImage::ImageBuffer& dst_buffer = parameter.mutable_output_image().mutable_list();

	for (int y = 0; y < height_; ++y)
	{
		if (sample_count > 1)
		{
			sampler_.generate_samples(sample_count * width_);

			for (int x = 0; x < width_; ++x)
			{
				const int pos = width_ * y + x;
				for (int s = 0; s < sample_count; ++s)
				{
					UMVec2d sample_point(sampler_.sample(s));
					sample_point.x += x;
					sample_point.y += y;
					scene.camera()->generate_ray(ray_, sample_point, (y == 0 && x == 0 && s == 0));
					UMVec3d color = trace(ray_, scene, shader_param_);
					dst_buffer[pos] += UMVec4d(color, 1.0);
				}
				dst_buffer[pos] *= inv_sample_count;
			}
		}
		else
		{
			for (int x = 0; x < width_; ++x)
			{
				const int pos = width_ * y + x;
				scene.camera()->generate_ray(ray_, UMVec2d(x, y), (y == 0 && x == 0));
				UMVec3d color = trace(ray_, scene, shader_param_);
				dst_buffer[pos] = UMVec4d(color, 1.0);
			}
		}
	}
	
	return true;
}

/**
 * progressive render
 */
bool UMRayTracer::progress_render(const UMScene& scene, UMRenderParameter& parameter)
{
	if (width_ == 0 || height_ == 0) return false;
	if (!scene.camera()) return false;
	
	const int ystep = 10;
	
	const int sample_count = parameter.super_sampling_count().x * parameter.super_sampling_count().y;
	const double inv_sample_count = 1.0 / sample_count;

	for (int& y = current_y_, rows = (y + ystep); y < rows; ++y)
	{
		// end
		if (y == height_) { return false; }
		
		sampler_.generate_samples(sample_count * width_);

		for (int x = 0; x < width_; ++x)
		{
			const int pos = width_ * y + x;
			for (int s = 0; s < sample_count; ++s)
			{
				UMVec2d sample_point(sampler_.sample(s));
				sample_point.x += x;
				sample_point.y += y;
				scene.camera()->generate_ray(ray_, sample_point, (y == 0 && x == 0 && s == 0));
				UMVec3d color = trace(ray_, scene, shader_param_);
				parameter.mutable_output_image().mutable_list()[pos] += UMVec4d(color, 1.0);
			}
			parameter.mutable_output_image().mutable_list()[pos] *= inv_sample_count;
		}
	}
	
	return true;
}

} // burger

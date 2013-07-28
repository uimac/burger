/**
 * @file UMRaytracer.cpp
 * a raytracer
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMRaytracer.h"
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

	/**
	 * shading function
	 */
	UMVec3d shade(const UMPrimitivePtr current, const UMRay& ray, const UMScene& scene, UMShaderParameter& parameter)
	{
		UMVec3d light_position = scene.light_list().at(0)->position();
		UMVec3d to_light_dir = (light_position - parameter.intersect_point).normalized();
		UMVec3d light_dir = light_position.normalized();
		UMVec3d normal(parameter.normal);

		// reflection ray
		if (parameter.bounce > 0)
		{
			--parameter.bounce;
			UMVec3d refrection_dir = reflect(ray, normal);
			UMRay reflection_ray(parameter.intersect_point, refrection_dir);
			UMVec3d original_color(parameter.color);
			UMVec3d color = trace(reflection_ray, scene, parameter);
			UMVec3d nl = parameter.normal.dot(refrection_dir);
			return UMVec3d(
				original_color.x * color.x * nl.x,
				original_color.y * color.y * nl.y,
				original_color.z * color.z * nl.z);
		}

		// shadow ray
		UMRay shadow_ray(parameter.intersect_point, to_light_dir);
		UMScene::PrimitiveList::const_iterator it = scene.primitive_list().begin();
		for (; it != scene.primitive_list().end(); ++it)
		{
			UMPrimitivePtr primitive = *it;
			if (primitive->intersects(shadow_ray))
			{
				return UMVec3d(0);
			}
		}

		return map_one(parameter.color * (normal.dot(light_dir)) * 0.5);
	}

	/**
	 * trace and return color of the hit point
	 */
	UMVec3d trace(const UMRay& ray, const UMScene& scene, UMShaderParameter& parameter)
	{
		double closest_distance = std::numeric_limits<double>::max();
		UMPrimitivePtr closest_primitive;
		UMShaderParameter closest_parameter;
		UMScene::PrimitiveList::const_iterator it = scene.primitive_list().begin();
		for (int i = 0; it != scene.primitive_list().end(); ++it, ++i)
		{
			UMPrimitivePtr primitive = *it;
			if (primitive->intersects(ray, parameter))
			{
				if (parameter.distance < closest_distance) 
				{
					closest_distance = parameter.distance;
					closest_primitive = primitive;
					closest_parameter = parameter;
				}
			}
		}
		if (closest_primitive)
		{
			return shade(closest_primitive, ray, scene, closest_parameter);
		}
		return scene.background_color();
	}
}

namespace burger
{

/**
 * render
 */
bool UMRaytracer::render(const UMScene& scene, UMRenderParameter& parameter)
{
	if (width_ == 0 || height_ == 0) return false;
	if (!scene.camera()) return false;

	for (int y = 0; y < height_; ++y)
	{
		for (int x = 0; x < width_; ++x)
		{
			scene.camera()->generate_ray(ray_, x, y);
			UMVec3d color = trace(ray_, scene, shader_param_);
			parameter.mutable_output_image().mutable_list()[width_ * y + x] = UMVec4d(color, 1.0);
		}
	}
	
	return false;
}

/**
 * progressive render
 */
bool UMRaytracer::progress_render(const UMScene& scene, UMRenderParameter& parameter)
{
	if (width_ == 0 || height_ == 0) return false;
	if (!scene.camera()) return false;
	
	const int ystep = 10;

	for (int& y = current_y_, rows = (y + ystep); y < rows; ++y)
	{
		// end
		if (y == height_) { return false; }

		for (int x = 0; x < width_; ++x)
		{
			scene.camera()->generate_ray(ray_, x, y);
			UMVec3d color = trace(ray_, scene, shader_param_);
			parameter.mutable_output_image().mutable_list()[width_ * y + x] = UMVec4d(color, 1.0);
		}
	}
	
	return true;
}

} // burger

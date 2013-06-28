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

#include <algorithm>

namespace
{
	using namespace burger;

	UMVec3d map_one(UMVec3d src) {
		double max = std::max(src.x, std::max(src.y, src.z));
		if (max > 1.0) {
			return src / max;
		}
		return src;
	}

	/**
	 * shading function
	 */
	UMVec3d shade(const UMPrimitivePtr current, const UMRay& ray, const UMScene& scene, UMShaderParameter& parameter)
	{
		UMVec3d light_position = UMVec3d(1000, 1000, -1000);
		// shadow ray
		UMVec3d shadow_dir = light_position - parameter.intersect_point;
		UMRay shadow_ray(parameter.intersect_point, shadow_dir.normalized());
		
		UMScene::Primitives::const_iterator it = scene.primitives().begin();
		for (; it != scene.primitives().end(); ++it)
		{
			const UMPrimitivePtr primitive = *it;
			if (primitive == current) continue;
			if (primitive->intersects(shadow_ray))
			{
				return UMVec3d(0);
			}
		}
		UMVec3d light_dir = light_position.normalized();
		return map_one(parameter.color * (parameter.normal.normalized().dot(light_dir)) * 0.5);
	}

	/**
	 * trace and return color of the hit point
	 */
	UMVec3d trace(const UMRay& ray, const UMScene& scene, UMShaderParameter& parameter)
	{
		UMScene::Primitives::const_iterator it = scene.primitives().begin();
		for (; it != scene.primitives().end(); ++it)
		{
			const UMPrimitivePtr primitive = *it;
			if (primitive->intersects(ray, parameter))
			{
				return shade(primitive, ray, scene, parameter);
			}
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
	
	const double half_width = width_ >> 1;
	const double half_height = height_ >> 1;
	const int wh = width_ * height_;

	parameter.mutable_output_image().set_width(width_);
	parameter.mutable_output_image().set_height(height_);
	UMImage::ImageBuffer& buffer = parameter.mutable_output_image().mutable_buffer();
	buffer.resize(wh);

	// garbage code for test
	UMRay ray( UMVec3d(0, 0, -500), UMVec3d(0) );
	
	UMShaderParameter shader_param;
	
	for (int y = 0; y < height_; ++y)
	{
		for (int x = 0; x < width_; ++x)
		{
			ray.set_direction(UMVec3d( x - half_width, y - half_height, 256).normalized());
			UMVec3d color = trace(ray, scene, shader_param);
			buffer[width_ * y + x] = UMVec4d(color.x, color.y, color.z, 1.0);
		}
	}
	
	return false;
}

} // burger

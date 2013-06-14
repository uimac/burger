/// UMSphere.cpp ///
#include "UMSphere.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"

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
	if (distance > DBL_EPSILON) {
		// hit
		parameter.color = color_;
		parameter.distance = distance;
		parameter.intersect_point = ray_orig + ray_dir * distance;
		parameter.normal = (parameter.intersect_point - center_) / radius_;
		return true;
	}

	distance = (-b + e) / (2 * a);
	if (distance > DBL_EPSILON) {
		// hit
		parameter.color = color_;
		parameter.distance = distance;
		parameter.intersect_point = ray_orig + ray_dir * distance;
		parameter.normal = (parameter.intersect_point - center_) / radius_;
		return true;
	}

	return false;
}

} // burger

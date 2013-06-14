/// UMPlane.h ///
#include "UMPlane.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"

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
	if (fabs(angle) < DBL_EPSILON) {
		// ray is parallel
		return false;
	}

	double distance = normal_.dot(point_ - ray.origin()) / angle;
	if (distance < DBL_EPSILON) {
		// ray is back of plane
		return false;
	}
	
	parameter.color = color_;
	parameter.distance = distance;
	parameter.intersect_point = ray_orig + ray_dir * distance;
	parameter.normal = normal_;

	return true;
}

} // burger

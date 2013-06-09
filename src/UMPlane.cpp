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
	
	parameter.color = UMVec3d(1.0, 0, 0);
	parameter.distance = distance;

	return true;
}

} // burger

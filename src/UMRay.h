/// UMRay.h ///
#pragma once

#include "UMVector.h"

namespace burger
{

/**
 * a ray
 */
class UMRay
{
public:
	UMRay() :
		origin_(0),
		direction_(0) {}

	UMRay(const UMVec3d& origin, const UMVec3d& direction) :
		origin_(origin),
		direction_(direction) {}

	~UMRay() {}

	/**
	 * get origin
	 */
	inline UMVec3d origin() const { return origin_; }
	
	/**
	 * set origin
	 * @parameter [in] direction source origin
	 */
	inline void set_origin(const UMVec3d& origin) { origin_ = origin; }

	/**
	 * get direction
	 */
	inline UMVec3d direction() const { return direction_; }
	
	/**
	 * set direction
	 * @parameter [in] direction source direction
	 */
	inline void set_direction(const UMVec3d& direction) { direction_ = direction; }

private:
	UMVec3d origin_;
	UMVec3d direction_;
};

} // burger

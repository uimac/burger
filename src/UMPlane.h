/// UMPlane.h ///
#pragma once

#include "UMPrimitive.h"
#include "UMVector.h"

namespace burger
{

class UMPlane;
typedef std::shared_ptr<UMPlane> UMPlanePtr;

class UMRay;
class UMShaderParameter;

/**
 * a plane
 */
class UMPlane : public UMPrimitive
{
public:
	UMPlane() :
		point_(0),
		normal_(0, 1, 0) {}

	UMPlane(const UMVec3d& point, const UMVec3d& normal) :
		point_(point),
		normal_(normal) {}

	~UMPlane() {}

	/**
	 * get point
	 */
	inline UMVec3d point() const { return point_; }
	
	/**
	 * set point
	 * @param [in] point source point
	 */
	inline void set_point(const UMVec3d& point) { point_ = point; }

	/**
	 * get normal
	 */
	inline UMVec3d normal() const { return normal_; }
	
	/**
	 * set normal
	 * @param [in] normal source normal
	 */
	inline void set_normal(const UMVec3d& normal) { normal_ = normal; }

	/**
	 * ray plane intersection
	 * @param [in] ray a ray
	 * @param [in|out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& parameter) const;

private:
	UMVec3d point_;
	UMVec3d normal_;

};

} // burger

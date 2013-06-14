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
		normal_(0, 1, 0),
		color_(0){}

	UMPlane(const UMVec3d& point, const UMVec3d& normal) :
		point_(point),
		normal_(normal),
		color_(0){}

	~UMPlane() {}

	/**
	 * get point
	 */
	UMVec3d point() const { return point_; }
	
	/**
	 * set point
	 * @param [in] point source point
	 */
	void set_point(const UMVec3d& point) { point_ = point; }

	/**
	 * get normal
	 */
	UMVec3d normal() const { return normal_; }
	
	/**
	 * set normal
	 * @param [in] normal source normal
	 */
	void set_normal(const UMVec3d& normal) { normal_ = normal; }

	/**
	 * set color
	 * @param [in] color source color
	 */
	void set_color(const UMVec3d& color) { color_ = color; }

	/**
	 * ray plane intersection
	 * @param [in] ray a ray
	 * @param [in|out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& parameter) const;

private:
	UMVec3d point_;
	UMVec3d normal_;
	
	UMVec3d color_;
};

} // burger

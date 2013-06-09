/// UMSphere.h ///
#pragma once

#include <memory>
#include "UMVector.h"
#include "UMPrimitive.h"

namespace burger
{

class UMSphere;
typedef std::shared_ptr<UMSphere> UMSpherePtr;

class UMRay;
class UMShaderParameter;

/**
 * a sphere
 */
class UMSphere : public UMPrimitive
{
public:
	UMSphere() :
		center_(0),
		radius_(0) {}

	UMSphere(const UMVec3d& center, double radius) :
		center_(center),
		radius_(radius) {}

	~UMSphere() {}
	
	/**
	 * get center
	 */
	inline UMVec3d center() const { return center_; }
	
	/**
	 * set center
	 * @param [in] center source center
	 */
	inline void set_center(const UMVec3d& center) { center_ = center; }
	
	/**
	 * get radius
	 */
	inline double radius() const { return radius_; }
	
	/**
	 * set radius
	 * @param [in] radius source radius
	 */
	inline void set_radius(double radius) { radius_ = radius; }
	
	/**
	 * set color
	 * @param [in] color source color
	 */
	inline void set_color(const UMVec3d& color) { color_ = color; }

	/**
	 * ray sphere intersection
	 * @param [in] ray a ray
	 * @param [in|out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& param) const;

private:
	UMVec3d center_;
	double radius_;

	UMVec3d color_;

};

} // burger
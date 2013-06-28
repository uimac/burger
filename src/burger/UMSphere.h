/**
 * @file UMSphere.h
 * a sphere
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
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
	DISALLOW_COPY_AND_ASSIGN(UMSphere);
public:
	UMSphere() :
		center_(0),
		radius_(0),
		color_(0) {}
	
	/**
	 * @param [in] center center
	 * @param [in] radius radius
	 */
	UMSphere(const UMVec3d& center, double radius) :
		center_(center),
		radius_(radius),
		color_(0) {}
	
	~UMSphere() {}
	
	/**
	 * get center
	 */
	UMVec3d center() const { return center_; }
	
	/**
	 * set center
	 * @param [in] center source center
	 */
	void set_center(const UMVec3d& center) { center_ = center; }
	
	/**
	 * get radius
	 */
	double radius() const { return radius_; }
	
	/**
	 * set radius
	 * @param [in] radius source radius
	 */
	void set_radius(double radius) { radius_ = radius; }
	
	/**
	 * set color
	 * @param [in] color source color
	 */
	void set_color(const UMVec3d& color) { color_ = color; }

	/**
	 * ray sphere intersection
	 * @param [in] ray a ray
	 * @param [in,out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& param) const;
	
	/**
	 * ray sphere intersection
	 * @param [in] ray a ray
	 */
	virtual bool intersects(const UMRay& ray) const;

private:
	UMVec3d center_;
	double radius_;

	UMVec3d color_;

};

} // burger

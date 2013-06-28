/**
 * @file UMRay.h
 * a ray
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"
#include "UMVector.h"

namespace burger
{

/**
 * a ray
 */
class UMRay
{
	DISALLOW_COPY_AND_ASSIGN(UMRay);

public:
	UMRay() :
		origin_(0),
		direction_(0) {}
	
	/**
	 * @param [in] origin origin
	 * @param [in] direction direction
	 */
	UMRay(const UMVec3d& origin, const UMVec3d& direction) :
		origin_(origin),
		direction_(direction) {}

	~UMRay() {}

	/**
	 * get origin
	 */
	UMVec3d origin() const { return origin_; }
	
	/**
	 * set origin
	 * @param [in] origin source origin
	 */
	void set_origin(const UMVec3d& origin) { origin_ = origin; }

	/**
	 * get direction
	 */
	UMVec3d direction() const { return direction_; }
	
	/**
	 * set direction
	 * @param [in] direction source direction
	 */
	void set_direction(const UMVec3d& direction) { direction_ = direction; }

private:
	UMVec3d origin_;
	UMVec3d direction_;
};

} // burger

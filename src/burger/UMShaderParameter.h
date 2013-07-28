/**
 * @file UMShaderParameter.h
 * shading parameters
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"

namespace burger
{
	
/**
 * shading parameters
 */
class UMShaderParameter
{
public:
	UMShaderParameter() : bounce(-1), max_bounce(1) {}
	~UMShaderParameter() {}
	
	/**
	 * color
	 */
	UMVec3d color;

	/**
	 * distance
	 */
	double distance;

	/**
	 * normal
	 */
	UMVec3d normal;

	/**
	 * intersect point
	 */
	UMVec3d intersect_point;

	/**
	 * triangle bycentic parameter
	 */
	UMVec3d uvw;

	/**
	 * bounce
	 */
	int bounce;

	/**
	 * max bounce
	 */
	int max_bounce;
};

} // burger

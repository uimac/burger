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
	DISALLOW_COPY_AND_ASSIGN(UMShaderParameter);
public:
	UMShaderParameter() {}
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
};

} // burger

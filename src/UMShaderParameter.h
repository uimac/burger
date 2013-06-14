/// UMShaderParameter.h ///
#pragma once

namespace burger
{
	
/**
 * shading parameters
 */
class UMShaderParameter
{
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

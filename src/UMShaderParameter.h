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
};

} // burger

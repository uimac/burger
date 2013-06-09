/// UMRenderParameter.h ///
#pragma once

#include <vector>
#include "UMVector.h"

namespace burger
{

/**
 * rendering parameters
 */
class UMRenderParameter
{
public:
	typedef std::vector<UMVec3d> ImageBuffer;

	UMRenderParameter() {}
	~UMRenderParameter() {}

	/**
	 * get output image
	 */
	const ImageBuffer& output_image() { return output_image_; } 
	
	/**
	 * get output image
	 */
	ImageBuffer&  mutable_output_image() { return output_image_; } 

private:
	ImageBuffer output_image_;
};

} // burger

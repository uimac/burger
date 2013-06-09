/// UMRenderParameter.h ///
#pragma once

#include <vector>

#include "UMImage.h"
#include "UMVector.h"

namespace burger
{

/**
 * rendering parameters
 */
class UMRenderParameter
{
public:

	UMRenderParameter() {}
	~UMRenderParameter() {}

	/**
	 * get output image
	 */
	const UMImage& output_image() const { return output_image_; } 
	
	/**
	 * get output image
	 */
	UMImage&  mutable_output_image() { return output_image_; } 

private:
	UMImage output_image_;
};

} // burger

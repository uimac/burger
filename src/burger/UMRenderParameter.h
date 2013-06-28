/**
 * @file UMRenderParameter.h
 * rendering parameters
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <vector>
#include "UMMacro.h"
#include "UMImage.h"
#include "UMVector.h"

namespace burger
{

/**
 * rendering parameters
 */
class UMRenderParameter
{
	DISALLOW_COPY_AND_ASSIGN(UMRenderParameter);
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

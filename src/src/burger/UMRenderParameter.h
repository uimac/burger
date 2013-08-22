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
	UMRenderParameter() 
		: super_sampling_count_(2, 2),
		sample_count_(1024)
	{}

	~UMRenderParameter() {}

	/**
	 * get output image
	 */
	const UMImage& output_image() const { return output_image_; } 
	
	/**
	 * get output image
	 */
	UMImage&  mutable_output_image() { return output_image_; } 

	/** 
	 * get sample count par pixel
	 */
	int sample_count() const { return sample_count_; }

	/**
	 * get super sampling
	 */
	UMVec2i super_sampling_count() const { return super_sampling_count_; }
	
private:
	UMImage output_image_;
	UMImage temporary_image_;
	int sample_count_;
	UMVec2i super_sampling_count_;
};

} // burger

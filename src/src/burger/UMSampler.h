/**
 * @file UMSampler.h
 * a raytracer
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <memory>
#include <vector>
#include "UMVector.h"
#include "UMMacro.h"

namespace burger
{

class UMSampler;
typedef std::shared_ptr<UMSampler> UMSamplerPtr;

/**
 * a sampler
 */
class UMSampler
{
	DISALLOW_COPY_AND_ASSIGN(UMSampler);
public:

	typedef std::vector<UMVec2d> SampleList;

	/**
	 * sampler types
	 */
	enum SamplerType {
		eRandom,
		eJittered
	};
	
	UMSampler()
		: type_(eRandom),
		pixel_count_(0),
		sample_count_(0)
	{}

	~UMSampler() {}

	/**
	 * get renderer type
	 */
	SamplerType type() const { return type_; }
	
	/**
	 * get pixel count
	 */
	int pixel_count() const { return pixel_count_; }

	/**
	 * get sample count
	 */
	int sample_count() const { return sample_count_; }
	
	/**
	 * get sample
	 * @param [in] sample_index sample index in pixel
	 */
	const UMVec2d& sample(int sample_index);

	/**
	 * get sample list
	 */
	const SampleList& sample_list() const { return sample_list_; }
	
	/**
	 * generate samples
	 * @param [in] sample_count sample counts in a pixel
	 * @param [in] pixel_count pixel counts
	 * @retval bool success or failed
	 */
	bool generate_samples(const int sample_count, const int pixel_count) {
		sample_count_ = sample_count;
		pixel_count_ = pixel_count;
		return generate();
	}
	
	/**
	 * generate samples
	 * @param [in] sample_count sample counts in a pixel
	 * @retval bool success or failed
	 */
	bool generate_samples(const int sample_count){
		sample_count_ = sample_count;
		pixel_count_ = 1;
		return generate();
	}

protected:
	/**
	 * generate samples
	 * @retval bool success or failed
	 */
	virtual bool generate() = 0;
	
	/**
	 * get sample list
	 */
	SampleList& mutable_sample_list() { return sample_list_; }
	
private:
	SampleList sample_list_;
	SamplerType type_;
	int pixel_count_;
	int sample_count_;
};

} // burger

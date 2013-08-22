/**
 * @file UMRandomSampler.h
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
#include <random>
#include "UMMacro.h"
#include "UMSampler.h"

namespace burger
{

class UMRandomSampler;
typedef std::shared_ptr<UMRandomSampler> UMRandomSamplerPtr;

/**
 * a sampler
 */
class UMRandomSampler : public UMSampler
{
	DISALLOW_COPY_AND_ASSIGN(UMRandomSampler);
public:
	UMRandomSampler();

protected:
	/**
	 * generate samples
	 * @retval bool success or failed
	 */
	virtual bool generate();

private:
	std::mt19937 mt_;
};

} // burger

/**
 * @file UMRandomSampler.cpp
 * a raytracer
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMRandomSampler.h"
#include "UMMath.h"
#include <random>

namespace
{
	std::uniform_real_distribution<> random_range(0.0, 1.0);
} // anonymouse namespace

namespace burger
{

UMRandomSampler::UMRandomSampler()
{
	std::random_device random_device;
	std::vector<unsigned int> seed(10);
	std::seed_seq seq(seed.begin(), seed.end());
	mt_.seed(seq);
}

/**
 * generate samples
 */
bool UMRandomSampler::generate()
{
	const int pixels = pixel_count();
	const int samples = sample_count();
	const int total_size = pixels * samples;
	if (total_size <= 0) { return false; }

	mutable_sample_list().resize(total_size);

	for (int i = 0; i < pixels; ++i)
	{
		for (int k = 0; k < samples; ++k)
		{
			UMVec2d p(random_range(mt_), random_range(mt_));
			mutable_sample_list()[i * samples + k] = p;
		}
	}
	return true;
}

} // burger

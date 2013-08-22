/**
 * @file UMSampler.cpp
 * a raytracer
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <memory>
#include <vector>
#include "UMMacro.h"
#include "UMSampler.h"
#include "UMMath.h"
#include "UMShaderParameter.h"

namespace burger
{
	
	//// from pbrt
	//UMVec3d CosineHemisphereSample(double u1, double u2) 
	//{
	//	UMVec3d ret;
	//	double r, theta;
	//	// Map uniform random numbers
	//	double sx = 2.0 * u1 - 1.f;
	//	double sy = 2.0 * u2 - 1.f;

	//	// Handle degeneracy at the origin
	//	if (sx == 0.f && sy == 0.f) {
	//		ret.x = 0.0;
	//		ret.y = 0.0;
	//	}
	//	else
	//	{
	//		if (sx >= -sy) {
	//			if (sx > sy) {
	//				//Handle first region of disk
	//				r = sx;
	//				if (sy > 0.f) 
	//				{
	//					theta = sy/r;
	//				}
	//				else 
	//				{
	//					theta = 8.f + sy/r;
	//				}
	//			}
	//			else
	//			{
	//				//Handle second region of disk
	//				r = sy;
	//				theta = 2.f - sx/r;
	//			}
	//		}
	//		else
	//		{
	//			if (sx <= sy) 
	//			{
	//				//Handle third region of dist
	//				r = -sx;
	//				theta = 4.f - sy/r;
	//			}
	//			else
	//			{
	//				//Handle fourth region
	//				r = -sy;
	//				theta = 6.f + sx/r;
	//			}
	//		}
	//		theta *= M_PI / 4.0;
	//		ret.x = r * cos(theta);
	//		ret.y = r * sin(theta);
	//	}
	//	ret.z = sqrt(std::max(0.0, 1.0 - ret.x*ret.x - ret.y*ret.y));
	//	return ret;
	//}


const UMVec2d& UMSampler::sample(int sample_index) 
{
	const int size = static_cast<int>(sample_list_.size());
	if (sample_index < size)
	{
		return sample_list_.at(sample_index);
	}
	else
	{
		return sample_list_.at(sample_index % size);
	}
}

} // burger

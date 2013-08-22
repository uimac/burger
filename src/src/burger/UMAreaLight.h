/**
 * @file UMAreaLight.h
 * an area light
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
#include "UMPrimitive.h"
#include "UMVector.h"
#include "UMLight.h"
#include "UMRandomSampler.h"

namespace burger
{

class UMAreaLight;
typedef std::shared_ptr<UMAreaLight> UMAreaLightPtr;
typedef std::weak_ptr<UMAreaLight> UMAreaLightWeakPtr;
typedef std::vector< UMAreaLightPtr > UMAreaLightList;

/**
 * a light
 */
class UMAreaLight : public UMLight
{
	DISALLOW_COPY_AND_ASSIGN(UMAreaLight);
public:

	UMAreaLight(
		const UMVec3d& position,
		const UMVec3d& edge1,
		const UMVec3d& edge2,
		const double constant_fall_off,
		const double linear_fall_off,
		const double quadric_fall_off
		)
		: area_(0),
		constant_fall_off_(constant_fall_off),
		linear_fall_off_(linear_fall_off),
		quadric_fall_off_(quadric_fall_off),
		edge1_(edge1),
		edge2_(edge2),
		normal_(0, -1, 0),
		last_sample_point_(0),
		random_sampler_(std::make_shared<UMRandomSampler>()),
		UMLight(position)
	{
		normal_ = edge1_.cross(edge2_);
		area_ = normal_.length();
		normal_ = normal_.normalized();
	}
	
	~UMAreaLight() {}

	/** 
	 * sample a point
	 * @param [out] intensity light intensity
	 * @param [out] point sampling point
	 * @param [out] direction light direction
	 * @param [in] parameter shader parameter on sample point
	 */
	virtual bool sample(UMVec3d& intensity, UMVec3d& point, UMVec3d& direction, const UMShaderParameter& parameter);
	
	/**
	 * get intensity from previous sample
	 */
	UMVec3d intensity(const UMVec3d& point) const;

private:
	double area_;
	double constant_fall_off_;
	double linear_fall_off_;
	double quadric_fall_off_;

	UMVec3d edge1_;
	UMVec3d edge2_;
	UMVec3d normal_;
	UMVec3d last_sample_point_;

	UMSamplerPtr random_sampler_;
};

} // burger

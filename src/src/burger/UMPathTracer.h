/**
 * @file UMPathTracer.h
 * a pathtracer
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
#include <random>
#include "UMMacro.h"
#include "UMRenderer.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"
#include "UMRandomSampler.h"
#include "UMImage.h"
#include "UMEvent.h"

namespace burger
{

class UMPathTracer;
typedef std::shared_ptr<UMPathTracer> UMPathTracerPtr;

class UMScene;
class UMRenderParameter;
class UMIntersection;

/**
 * a raytracer
 */
class UMPathTracer : public UMRenderer
{
	DISALLOW_COPY_AND_ASSIGN(UMPathTracer);
public:
	UMPathTracer();

	~UMPathTracer() {}
	
	/**
	 * initialize
	 * @note needs a context
	 */
	virtual bool init() {
		current_sample_count_ = 0;
		return true;
	}

	/**
	 * get renderer type
	 */
	virtual RendererType type() const { return ePathTracer; }
	
	/**
	 * render
	 * @param [in] scene target scene
	 * @param [in,out] parameter parameters for rendering
	 * @retval success or failed
	 */
	virtual bool render(const UMScene& scene, UMRenderParameter& parameter);
	
	/**
	 * progressive render
	 * @param [in] scene target scene
	 * @param [in,out] parameter parameters for rendering
	 * @retval true still render
	 * @retval false render finished or failed
	 */
	virtual bool progress_render(const UMScene& scene, UMRenderParameter& parameter);

private:
	/**
	 * trace
	 */
	UMVec3d trace(
		const UMRay& ray, 
		const UMScene& scene, 
		UMShaderParameter& parameter, 
		std::mt19937& mt);

	/**
	 * direct lighting
	 */
	UMVec3d illuminate_direct(
		const UMRay& ray, 
		const UMScene& scene, 
		const UMIntersection& intersection, 
		UMShaderParameter& parameter,
		std::mt19937& mt);

	/**
	 * indirect lighting
	 */
	UMVec3d illuminate_indirect(
		const UMRay& ray, 
		const UMScene& scene, 
		const UMIntersection& intersection, 
		UMShaderParameter& parameter, 
		std::mt19937& mt);
	

	// for progress render
	int current_sample_count_;
	int current_subpixel_x_;
	int current_subpixel_y_;
	int max_sample_count_;
	UMRandomSampler sampler_;
	UMImage temporary_image_;
	UMEventPtr sample_event_;
};

} // burger

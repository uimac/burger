/**
 * @file UMRaytracer.h
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
#include "UMMacro.h"
#include "UMRenderer.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"

namespace burger
{

class UMRaytracer;
typedef std::shared_ptr<UMRaytracer> UMRaytracerPtr;

class UMScene;
class UMRenderParameter;

/**
 * a raytracer
 */
class UMRaytracer : public UMRenderer
{
	DISALLOW_COPY_AND_ASSIGN(UMRaytracer);
public:
	UMRaytracer() : 
		current_x_(0),
		current_y_(0),
		ray_( UMVec3d(0, 0, 500), UMVec3d(0) )
	{}

	~UMRaytracer() {}
	
	/**
	 * initialize
	 * @note needs a context
	 */
	virtual bool init() {
		current_x_ = 0;
		current_y_ = 0;
		ray_.set_origin( UMVec3d(0, 0, 500) );
		ray_.set_direction( UMVec3d(0) );
		return true;
	}

	/**
	 * get renderer type
	 */
	virtual RendererType type() const { return eRaytraceRenderer; }
	
	/**
	 * render
	 * @param [in] scene target scene
	 * @param [in,out] parameter parameters for rendering
	 * @retval true still render
	 * @retval false render finished or failed
	 */
	virtual bool render(const UMScene& scene, UMRenderParameter& parameter);
	
	/**
	 * progressive render
	 * @param [in] scene target scene
	 * @param [in,out] parameter parameters for rendering
	 * @retval render finished or still render
	 */
	virtual bool progress_render(const UMScene& scene, UMRenderParameter& parameter);

private:
	// for progress render
	int current_x_;
	int current_y_;
	UMRay ray_;
	UMShaderParameter shader_param_;
};

} // burger

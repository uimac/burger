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

namespace burger
{

class UMRaytracer;
typedef std::shared_ptr<UMRaytracer> UMRaytracerPtr;

class UMScene;
class UMRenderParameter;
class UMShaderParameter;

/**
 * a raytracer
 */
class UMRaytracer : public UMRenderer
{
	DISALLOW_COPY_AND_ASSIGN(UMRaytracer);
public:
	UMRaytracer() {}
	~UMRaytracer() {}
	
	/**
	 * initialize
	 * @note needs a context
	 */
	virtual bool init() { return true; }

	/**
	 * get renderer type
	 */
	virtual RendererType type() const { return eRaytraceRenderer; }
	
	/**
	 * render
	 * @param [in] scene target scene
	 * @param [in,out] parameter parameters for rendering
	 */
	virtual bool render(const UMScene& scene, UMRenderParameter& parameter);
};

} // burger

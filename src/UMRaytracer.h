/// UMRaytracer.h ///
#pragma once

#include <memory>
#include <vector>
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
	 * @param [in|out] parameter parameters for rendering
	 */
	virtual bool render(const UMScene& scene, UMRenderParameter& parameter);
};

} // burger

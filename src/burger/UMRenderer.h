/**
 * @file UMRenderer.h
 * an interface of renderer
 */
#pragma once

#include <memory>
#include "UMMacro.h"

namespace burger
{

class UMRenderer;
typedef std::shared_ptr<UMRenderer> UMRendererPtr;

class UMScene;
class UMRenderParameter;

/**
 * an interface of renderer
 */
class UMRenderer
{
	DISALLOW_COPY_AND_ASSIGN(UMRenderer);
public:
	/**
	 * renderer types
	 */
	enum RendererType {
		eRaytraceRenderer,
		eDirectX11Renderer
	};
	
	UMRenderer() : width_(0), height_(0) {}
	
	~UMRenderer() {}
	
	/**
	 * initialize
	 * @note needs a context
	 */
	virtual bool init() = 0;

	/**
	 * get renderer type
	 */
	virtual RendererType type() const = 0;
	
	/**
	 * render
	 * @param [in] scene target scene
	 * @param [in,out] parameter parameters for rendering
	 */
	virtual bool render(const UMScene& scene, UMRenderParameter& parameter) = 0;
	
	/** 
	 * set client width
	 */
	virtual void set_width(int width) { width_ = width; }

	/** 
	 * set client height
	 */
	virtual void set_height(int height) { height_ = height; }

	/** 
	 * get client width
	 */
	virtual int width() const { return width_; }

	/** 
	 * get client height
	 */
	virtual int height() const { return height_; }
	
protected:
	int width_;
	int height_;
};

} // burger

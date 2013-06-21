/**
 * @file UMDirectX11Renderer.h
 */
#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include "UMRenderer.h"
#include "UMDirectX11ShaderManager.h"

namespace burger
{

/**
 * directx 11 renderer
 */
class UMDirectX11Renderer : public UMRenderer 
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11Renderer);

public:
	UMDirectX11Renderer(ID3D11Device *device_pointer);

	virtual ~UMDirectX11Renderer();

	/**
	 * initialize
	 * @note needs a context
	 */
	virtual bool init();

	/**
	 * get renderer type
	 */
	virtual RendererType type() const { return eDirectX11Renderer; }
	
	/**
	 * render
	 * @param [in] scene target scene
	 * @param [in,out] parameter parameters for rendering
	 */
	virtual bool render(const UMScene& scene, UMRenderParameter& parameter);
	
	/**
	 * get shader manager
	 */
	const UMDirectX11ShaderManager& shader_manager() const { return *shader_manager_; }

	/**
	 * get shader manager
	 */
	UMDirectX11ShaderManager& mutable_shader_manager() { return *shader_manager_; }

protected:
	UMDirectX11ShaderManagerPtr shader_manager_;
	
	ID3D11RasterizerState *rasterizaer_state_pointer_;
	ID3D11DepthStencilState *depth_stencil_state_pointer_;
	ID3D11Device* device_pointer_;
};

} // burger

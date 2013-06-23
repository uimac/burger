/**
 * @file UMDirectX11Viewer.h
 */
#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include "UMRenderer.h"
#include "UMDirectX11ShaderManager.h"
#include "UMRenderer.h"

namespace burger
{

/**
 * directx 11 viewer
 */
class UMDirectX11Viewer
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11Viewer);

public:
	UMDirectX11Viewer();

	virtual ~UMDirectX11Viewer();

	/**
	 * initialize
	 */
	virtual bool init(HWND hWnd, int width, int height);

	/**
	 * refresh frame
	 */
	bool refresh();
	
private:
	/**
	 * get shader manager
	 */
	UMDirectX11ShaderManagerPtr shader_manager() const { return shader_manager_; }

	/**
	 * get shader manager
	 */
	UMDirectX11ShaderManager& mutable_shader_manager() { return *shader_manager_; }
	
	/**
	 * initialize devices
	 */
	bool init_devices(HWND hWnd, int width, int height);

	UMDirectX11ShaderManagerPtr shader_manager_;

	// DXGI
	IDXGIFactory *dxgi_factory_pointer_;
	IDXGIAdapter *dxgi_adapter_pointer_;
	IDXGIDevice1 *dxgi_device_pointer_;
	IDXGISwapChain *dxgi_swap_chain_pointer_;
	DXGI_SWAP_CHAIN_DESC dxgi_swap_chain_desc_;
	
	// ID3D11
	ID3D11Device* device_pointer_;
	ID3D11DeviceContext *device_context_pointer_;
	ID3D11RenderTargetView* render_target_view_pointer_;
	ID3D11Texture2D* depth_stencil_texture_pointer_;
	ID3D11DepthStencilView* depth_stencil_view_pointer_;
	ID3D11DepthStencilState *depth_stencil_state_pointer_;
	ID3D11RasterizerState *rasterizaer_state_pointer_;

	// test
	ID3D11Buffer *vertex_buffer_pointer_;
	ID3D11Texture2D * render_result_texture_pointer_;
	ID3D11ShaderResourceView * render_result_srv_pointer_;
	ID3D11SamplerState * render_result_sampler_state_pointer_;

	ID3D11Debug* d3d11_debug_pointer_;

	UMRendererPtr renderer_;
};

} // burger

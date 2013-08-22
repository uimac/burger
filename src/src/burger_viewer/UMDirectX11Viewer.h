/**
 * @file UMDirectX11Viewer.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include "UMScene.h"
#include "UMListener.h"

#include "UMTime.h"
#include "UMDirectX11Scene.h"
#include "UMDirectX11ShaderManager.h"
#include "UMDirectX11Texture.h"

namespace burger
{

class UMDirectX11Viewer;
typedef std::shared_ptr<UMDirectX11Viewer> UMDirectX11ViewerPtr;
typedef std::weak_ptr<UMDirectX11Viewer> UMDirectX11ViewerWeakPtr;

/**
 * directx 11 viewer
 */
class UMDirectX11Viewer : public UMListener
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11Viewer);

public:
	static UMDirectX11ViewerPtr create();

	virtual ~UMDirectX11Viewer();

	/**
	 * initialize
	 */
	virtual bool init(HWND hWnd, int width, int height);

	/**
	 * refresh frame
	 */
	bool refresh();
	
	/**
	 * left button down
	 */
	void on_left_button_down(HWND hWnd, short x, short y);
	
	/**
	 * left button up
	 */
	void on_left_button_up(HWND hWnd, short x, short y);
	
	/**
	 * right button down
	 */
	void on_right_button_down(HWND hWnd, short x, short y);
	
	/**
	 * right button up
	 */
	void on_right_button_up(HWND hWnd, short x, short y);

	/**
	 * mouse move
	 */
	void on_mouse_move(HWND hWnd, short x, short y);

	/**
	 * keyboard pressed
	 */
	void on_key_down(HWND hWnd, unsigned int key_code);
	
	/**
	 * keyboard up
	 */
	void on_key_up(HWND hWnd, unsigned int key_code);

	/**
	 * event notify
	 */
	virtual void update(UMEventType event_type, UMAny& parameter);
	
private:
	UMDirectX11Viewer();

	HWND handle_;

	UMDirectX11ViewerWeakPtr self_reference_;

	/**
	 * initialize devices
	 */
	bool init_devices(HWND hWnd, int width, int height);

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

	ID3D11Debug* d3d11_debug_pointer_;

	UMDirectX11ScenePtr scene_;

	int pre_x_;
	int pre_y_;
	bool is_left_button_down_;
	bool is_right_button_down_;
	bool is_ctrl_button_down_;
};

} // burger

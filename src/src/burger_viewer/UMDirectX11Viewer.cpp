/**
 * @file UMDirectX11Viewer.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11Viewer.h"
#include <shellapi.h>
#include <delayimp.h>
#include <dxgi.h>
#include <d3d11.h>

#include "UMDirectX11ShaderManager.h"
#include "UMDirectX11Renderer.h"
#include "UMPath.h"
#include "UMStringUtil.h"
#include "UMDirectX11Board.h"
#include "UMDirectX11Texture.h"
#include "UMRenderer.h"
#include "UMAny.h"
#include "UMListener.h"

#include "UMRayTracer.h"
#include "UMScene.h"
#include "UMRenderParameter.h"
#include "UMTime.h"
#include "UMImage.h"
#include "UMTriangle.h"
#include "UMBvh.h"

#include <tchar.h>
#include <shlwapi.h>
#include "UMIO.h"

namespace burger
{
	
UMDirectX11ViewerPtr UMDirectX11Viewer::create()
{
	UMDirectX11ViewerPtr viewer = UMDirectX11ViewerPtr(new UMDirectX11Viewer);
	viewer->self_reference_ = viewer;
	return viewer;
}

UMDirectX11Viewer::UMDirectX11Viewer()
	: 
	// IDXGI
	dxgi_factory_pointer_(NULL),
	dxgi_adapter_pointer_(NULL),
	dxgi_device_pointer_(NULL),
	dxgi_swap_chain_pointer_(NULL),
	// ID3D11
	device_pointer_(NULL),
	device_context_pointer_(NULL),
	render_target_view_pointer_(NULL),
	depth_stencil_texture_pointer_(NULL),
	depth_stencil_view_pointer_(NULL),
	depth_stencil_state_pointer_(NULL),
	rasterizaer_state_pointer_(NULL),
	// other
	d3d11_debug_pointer_(NULL),
	is_left_button_down_(false),
	is_ctrl_button_down_(false),
	pre_x_(0),
	pre_y_(0),
	scene_(std::make_shared<UMDirectX11Scene>())
{
}

UMDirectX11Viewer::~UMDirectX11Viewer()
{
	// goto window mode
	if (dxgi_swap_chain_pointer_)
	{
		BOOL fullscreen;
		dxgi_swap_chain_pointer_->GetFullscreenState(&fullscreen, NULL);
		if (fullscreen)
		{
			dxgi_swap_chain_pointer_->SetFullscreenState(FALSE, NULL);
		}
	}

	// IDXGI
	SAFE_RELEASE(dxgi_factory_pointer_);
	SAFE_RELEASE(dxgi_adapter_pointer_);
	SAFE_RELEASE(dxgi_device_pointer_);
	SAFE_RELEASE(dxgi_swap_chain_pointer_);
	// ID3D11
	SAFE_RELEASE(device_pointer_);
	SAFE_RELEASE(device_context_pointer_);
	SAFE_RELEASE(render_target_view_pointer_);
	SAFE_RELEASE(depth_stencil_texture_pointer_);
	SAFE_RELEASE(depth_stencil_view_pointer_);
	SAFE_RELEASE(depth_stencil_state_pointer_);
	SAFE_RELEASE(rasterizaer_state_pointer_);
	
	/*
	if (d3d11_debug_pointer_)
	{
		d3d11_debug_pointer_->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	*/
	SAFE_RELEASE(d3d11_debug_pointer_);
}

/**
 * initialize
 */
bool UMDirectX11Viewer::init(HWND hWnd, int width, int height)
{
	if (!init_devices(hWnd, width, height))
	{
		return false;
	}

	if (device_pointer_ && device_context_pointer_)
	{
		// rasterizer state
		{
			D3D11_RASTERIZER_DESC desc;
			ZeroMemory( &desc, sizeof( D3D11_RASTERIZER_DESC ) );
			desc.CullMode = D3D11_CULL_BACK; // backface culling
			desc.FrontCounterClockwise =TRUE; // CCW
			//desc.FillMode = D3D11_FILL_WIREFRAME;
			desc.FillMode = D3D11_FILL_SOLID; 
			desc.DepthClipEnable = TRUE;
			device_pointer_->CreateRasterizerState( &desc, &rasterizaer_state_pointer_ );
			device_context_pointer_->RSSetState(rasterizaer_state_pointer_);
		}

		// depth stencil state
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory( &desc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
			desc.DepthEnable	= TRUE;
			desc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc	= D3D11_COMPARISON_LESS;
			desc.StencilEnable	= FALSE;
			device_pointer_->CreateDepthStencilState( &desc, &depth_stencil_state_pointer_ );
		}

		// init member
		handle_ = hWnd;
		scene_->connect(self_reference_.lock());

		return true;
	}
	return false;
}

/**
 * initialize devices
 * @param [in] hWnd main window handle
 */
bool UMDirectX11Viewer::init_devices(HWND hWnd, int width, int height)
{
	D3D_FEATURE_LEVEL FeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,		// DirectX11.0 GPU
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1 GPU
		D3D_FEATURE_LEVEL_10_0,		// DirectX10.0 GPU
		D3D_FEATURE_LEVEL_9_3,		// DirectX9/ShaderModel3 GPU
		D3D_FEATURE_LEVEL_9_2,		// DirectX9/ShaderModel2 GPU
		D3D_FEATURE_LEVEL_9_1,		// DirectX9/ShaderModel2 GPU
	};

	// create device
	{
		D3D_FEATURE_LEVEL feature_level;
		HRESULT hr = D3D11CreateDevice(
			// adapter
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			// module
			NULL,
			D3D11_CREATE_DEVICE_DEBUG,
			FeatureLevels,
			sizeof(FeatureLevels) / sizeof(D3D_FEATURE_LEVEL),
			D3D11_SDK_VERSION,
			&device_pointer_,
			&feature_level,
			&device_context_pointer_ );

		if FAILED( hr )
		{
			return false;
		}
	}

	// get dxgi adapter, factory
	{
		// get IDXGIDevice from ID3D11Device
		if FAILED(device_pointer_->QueryInterface( __uuidof(IDXGIDevice1), (void**)&dxgi_device_pointer_))
		{
			return false;
		}

		if FAILED(device_pointer_->QueryInterface( __uuidof(ID3D11Debug), (void**)&d3d11_debug_pointer_))
		{
			return false;
		}
	
		// get adapter from IDXGIDevice
		if FAILED(dxgi_device_pointer_->GetAdapter( &dxgi_adapter_pointer_))
		{
			return false;
		}
	
		// get factory from IDXGIDevice
		if FAILED(dxgi_adapter_pointer_->GetParent( __uuidof(IDXGIFactory), (void**)&dxgi_factory_pointer_))
		{
			return false;
		}
	}

	// swap chan parameters
	{
		dxgi_swap_chain_desc_.BufferDesc.Width = width;
		dxgi_swap_chain_desc_.BufferDesc.Height = height;
		dxgi_swap_chain_desc_.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		dxgi_swap_chain_desc_.BufferDesc.RefreshRate.Denominator = 1;
		dxgi_swap_chain_desc_.BufferDesc.RefreshRate.Numerator= 60;
		dxgi_swap_chain_desc_.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		dxgi_swap_chain_desc_.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		// multi sampling count
		dxgi_swap_chain_desc_.SampleDesc.Count = 1;
		// multi sampling quality
		dxgi_swap_chain_desc_.SampleDesc.Quality = 0;
		// backbuffer
		dxgi_swap_chain_desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgi_swap_chain_desc_.BufferCount = 3;
		// output window
		dxgi_swap_chain_desc_.OutputWindow = hWnd;
		// window mode ?
		dxgi_swap_chain_desc_.Windowed = 1;
		// swap
		dxgi_swap_chain_desc_.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		dxgi_swap_chain_desc_.Flags	= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// create swap chan from D3D11Device
		if FAILED(dxgi_factory_pointer_->CreateSwapChain(
				device_pointer_, 
				&dxgi_swap_chain_desc_, 
				&dxgi_swap_chain_pointer_))
		{
			return false;
		}
	}

	// create render taget
	D3D11_TEXTURE2D_DESC back_buffer_desc;
	{
		// get back buffer
		ID3D11Texture2D *back_buffer = NULL;
		if FAILED(dxgi_swap_chain_pointer_->GetBuffer(
				0,
				__uuidof(ID3D11Texture2D),
				(LPVOID*)&back_buffer))
		{
			return false;
		}

		// get back buffer description
		back_buffer->GetDesc(&back_buffer_desc);

		// create render target view
		if FAILED(device_pointer_->CreateRenderTargetView(
				back_buffer,
				NULL,
				&render_target_view_pointer_))
		{
			return false;
		}

		if (back_buffer)
		{
			back_buffer->Release();
			back_buffer = NULL;
		}
	}

	// set viewport
	{
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		device_context_pointer_->RSSetViewports( 1, &vp );
	}

	// create depth texture
	{
	//	back_buffer_desc.Width
	//	back_buffer_desc.Height
		back_buffer_desc.MipLevels = 1;
		back_buffer_desc.ArraySize = 1;
		back_buffer_desc.Format = DXGI_FORMAT_D32_FLOAT;
	//	back_buffer_desc.SampleDesc.Count   = 
	//	back_buffer_desc.SampleDesc.Quality = 
		back_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		back_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		back_buffer_desc.CPUAccessFlags = 0; // don't access from cpu
		back_buffer_desc.MiscFlags = 0;

		if FAILED(device_pointer_->CreateTexture2D(
				&back_buffer_desc,
				NULL,
				&depth_stencil_texture_pointer_))
		{
			return false;
		}
	}

	// create depth stencil view
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depth_view_desc;
		depth_view_desc.Format = back_buffer_desc.Format;
		depth_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_view_desc.Flags = 0;
		depth_view_desc.Texture2D.MipSlice = 0;

		if FAILED(device_pointer_->CreateDepthStencilView(
				depth_stencil_texture_pointer_,
				&depth_view_desc,
				&depth_stencil_view_pointer_))
		{
			return false;
		}
	}

	// set render target
	{
		// set render target view
		device_context_pointer_->OMSetRenderTargets( 1, &render_target_view_pointer_, depth_stencil_view_pointer_ ); 
	}

	// create test scene
	{
#ifndef HONBAN
		UMTime loadtime("loadtime", true);
#endif
		scene_->init(device_pointer_, width, height);
		
		std::string models[] = {
			//"•́ε•̀٥.bos",
			//"miku10.bos",
			//"cornellbox_empty.bos",
			//"cornallbox_akari.bos",
			//"buddha.bos",
			//"q3.bos",
			//"bunny.bos",
			//"monkey2.bos",
			"q3_1.bos",
			//"piyo.bos",
			//"cornellbox.bos"
			//"cornellbox_nolight.bos",
			//"miku_alegro.bos"
			//"dabrovic-sponza.bos",
		};

		for (int i = 0, isize = sizeof(models) / sizeof(models[0]); i < isize; ++i)
		{
			std::u16string inpath = UMPath::resource_absolute_path(UMStringUtil::utf8_to_utf16(models[i]));
			scene_->load(device_pointer_, inpath);
		}
		const int size = static_cast<int>(scene_->mutable_render_scene()->mutable_primitive_list().size());
		//scene_->mutable_render_scene().mutable_primitive_list().at(size-1)->emission = UMVec3d(10.0);
		//scene_->mutable_render_scene().mutable_primitive_list().at(size-2)->emission = UMVec3d(10.0);

		//std::string debug_count = "primitive count:" + UMStringUtil::number_to_string(
		//	scene_->mutable_render_scene().mutable_primitive_list().size());
		//::MessageBoxA(NULL, debug_count.c_str(), "hoge", MB_OK);
	}

	// build scene
	{
#ifndef HONBAN
		UMTime buildtime("buildtime", true);
#endif
		scene_->mutable_render_scene()->update_bvh();
	}

	// for rendering1h
	scene_->rendering1h().set_scene(scene_);

	//// (debug) convert bvh to directx11
	//{
	//	scene_->load_bvh(device_pointer_);
	//}

#ifdef HONBAN
	scene_->render(device_pointer_, true);
#endif

	return true;
}

/**
 * render
 */
bool UMDirectX11Viewer::refresh()
{
	float clear_color[] = { 0.21f, 0.21f, 0.21f, 1.0f };
	device_context_pointer_->ClearRenderTargetView( render_target_view_pointer_, clear_color );
	device_context_pointer_->ClearDepthStencilView( depth_stencil_view_pointer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
	
	// refresh scene
	if (!scene_->refresh(device_pointer_))
	{
		return false;
	}

	if (dxgi_swap_chain_pointer_)
	{
		dxgi_swap_chain_pointer_->Present(0, 0);
	}

	return true;
}

/**
 * left button down
 */
void UMDirectX11Viewer::on_left_button_down(HWND hWnd, short x, short y)
{
	pre_x_ = x;
	pre_y_ = y;
	is_left_button_down_ = true;
	SetCapture(hWnd);
}
	
/**
 * left button up
 */
void UMDirectX11Viewer::on_left_button_up(HWND hWnd, short x, short y)
{
	if (is_left_button_down_)
	{
		ReleaseCapture();
	}
	is_left_button_down_ = false;
}

/**
 * right button down
 */
void UMDirectX11Viewer::on_right_button_down(HWND hWnd, short x, short y)
{
	if (scene_->is_rendering()) return;
	pre_x_ = x;
	pre_y_ = y;
	is_right_button_down_ = true;
	SetCapture(hWnd);
}
	
/**
 * right button up
 */
void UMDirectX11Viewer::on_right_button_up(HWND hWnd, short x, short y)
{
	if (scene_->is_rendering()) return;
	if (is_right_button_down_)
	{
		ReleaseCapture();
		
		::SetWindowTextA(handle_, std::string("burger_viewer").c_str());
	}
	is_right_button_down_ = false;
}

/**
 * mouse move
 */
void UMDirectX11Viewer::on_mouse_move(HWND hWnd, short x, short y)
{
	if (scene_->is_rendering()) return;
	if (GetCapture() == hWnd)
	{
		if (is_left_button_down_)
		{
			if (UMCameraPtr camera = scene_->render_scene()->camera())
			{
				camera->rotate(pre_x_ - x, pre_y_ - y);
				pre_x_ = x;
				pre_y_ = y;
			}
		}
		if (is_right_button_down_)
		{
			if (UMCameraPtr camera = scene_->render_scene()->camera())
			{
				if (is_ctrl_button_down_)
				{
					camera->zoom(pre_x_ - x, pre_y_ - y);
					pre_x_ = x;
					pre_y_ = y;
				}
				else
				{
					camera->dolly(pre_x_ - x, pre_y_ - y);
					pre_x_ = x;
					pre_y_ = y;
				}
			}
		}
	}
}

/**
 * keyboard pressed
 */
void UMDirectX11Viewer::on_key_down(HWND hWnd, unsigned int key_code)
{
	if (key_code == VK_SPACE)
	{
		if (!scene_->is_rendering())
		{
			scene_->render(device_pointer_, false);
		}
	}
	if (key_code == VK_RETURN)
	{
		if (!scene_->is_rendering())
		{
			scene_->render(device_pointer_, true);
		}
	}
	if (key_code == VK_CONTROL)
	{
		is_ctrl_button_down_ = true;
		//scene_->render_scene()->mutable_camera()->rotate(-20, -45);
		//scene_->render_scene()->mutable_camera()->zoom(0, -10);
	}
}

/**
 * keyboard pressed
 */
void UMDirectX11Viewer::on_key_up(HWND hWnd, unsigned int key_code)
{
	if (key_code == VK_CONTROL)
	{
		is_ctrl_button_down_ = false;
	}
}
void UMDirectX11Viewer::update(UMEventType event_type, UMAny& parameter)
{
	if (event_type == eEventTypeRenderUpdate)
	{
		::Sleep(1);
	}
	else if (event_type == eEventTypeRenderProgressSample)
	{
		int count = any_cast<int>(parameter);
		::SetWindowTextA(handle_, std::string("burger - rendering.. sample count: " + UMStringUtil::number_to_string(count)).c_str());
	}
	else if (event_type == eEventTypeCameraZoom)
	{
		double fovy = any_cast<double>(parameter);
		::SetWindowTextA(handle_, std::string("zoom fovy: " + UMStringUtil::number_to_string((int)fovy)).c_str());
	}
	else if (event_type == eEventTypeCameraDolly)
	{
		double length = any_cast<double>(parameter);
		::SetWindowTextA(handle_, std::string("dolly length: " + UMStringUtil::number_to_string(length)).c_str());
	}
}


} // burger

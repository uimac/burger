/**
 * @file UMDirectX11Viewer.cpp
 */
#include "UMDirectX11Viewer.h"
#include <shellapi.h>
#include <delayimp.h>
#include <dxgi.h>
#include <d3d11.h>

#include "UMDirectX11ShaderManager.h"
#include "UMDirectX11Renderer.h"

#include "UMRaytracer.h"
#include "UMScene.h"
#include "UMRenderParameter.h"

namespace burger
{

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
	// renderer
	renderer_(std::make_shared<UMRaytracer>()),
	// other
	d3d11_debug_pointer_(NULL)
{}

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
	if (dxgi_factory_pointer_)
	{
		dxgi_factory_pointer_->Release();
		dxgi_factory_pointer_ = NULL;
	}

	if (dxgi_adapter_pointer_)
	{
		dxgi_adapter_pointer_->Release();
		dxgi_adapter_pointer_ = NULL;
	}

	if (dxgi_device_pointer_)
	{
		dxgi_device_pointer_->Release();
		dxgi_device_pointer_ = NULL;
	}

	if (dxgi_swap_chain_pointer_)
	{
		dxgi_swap_chain_pointer_->Release();
		dxgi_swap_chain_pointer_ = NULL;
	}
	
	// ID3D11
	if (device_pointer_)
	{
		device_pointer_->Release();
		device_pointer_ = NULL;
	}

	if (device_context_pointer_)
	{
		device_context_pointer_->Release();
		device_context_pointer_ = NULL;
	}

	if (render_target_view_pointer_)
	{
		render_target_view_pointer_->Release();
		render_target_view_pointer_ = NULL;
	}

	if (depth_stencil_texture_pointer_)
	{
		depth_stencil_texture_pointer_->Release();
		depth_stencil_texture_pointer_ = NULL;
	}
	
	if (depth_stencil_view_pointer_)
	{
		depth_stencil_view_pointer_->Release();
		depth_stencil_view_pointer_ = NULL;
	}

	if (depth_stencil_state_pointer_)
	{
		depth_stencil_state_pointer_->Release();
		depth_stencil_state_pointer_ = NULL;
	}

	if (rasterizaer_state_pointer_)
	{
		rasterizaer_state_pointer_->Release();
		rasterizaer_state_pointer_ = NULL;
	}

	if (d3d11_debug_pointer_)
	{
		//d3d11_debug_pointer_->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		d3d11_debug_pointer_->Release();
		d3d11_debug_pointer_ = NULL;
	}

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

	if (device_pointer_)
	{
		// create shader manager
		{
			shader_manager_ = std::make_shared<UMDirectX11ShaderManager>(device_pointer_);
		}

		// rasterizer state
		{
			D3D11_RASTERIZER_DESC desc;
			ZeroMemory( &desc, sizeof( D3D11_RASTERIZER_DESC ) );
			desc.CullMode = D3D11_CULL_NONE;//D3D11_CULL_BACK;
			desc.FillMode = D3D11_FILL_SOLID; 
			desc.DepthClipEnable = TRUE;
			device_pointer_->CreateRasterizerState( &desc, &rasterizaer_state_pointer_ );
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

	// create test scene and render it
	{
		UMScene scene;
		scene.create_sample_scene_1();
		UMRenderParameter parameter;
		renderer_->init();
		renderer_->set_width(width);
		renderer_->set_height(height);
		renderer_->render(scene, parameter);
	}

	return true;
}

/**
 * render
 */
bool UMDirectX11Viewer::refresh()
{
	/*
	std::vector<UMDirectX11BufferObject>& objects = mutable_model_loader().mutable_buffer_objects();
	UINT stride = 3;//sizeof(um_vector3);
	UINT index_count = 0;
	UINT offset = 0;
	for (size_t i = 0, size = objects.size(); i < size; ++i) {
		device_context_pointer_->IASetVertexBuffers(0, 1, objects[i].pp_vertex_buffer(), &stride, &offset);
		device_context_pointer_->IASetIndexBuffer(objects[i].p_index_buffer(), DXGI_FORMAT_R32_SINT, 0);
		index_count += objects[i].face_size() * 3;
	}
	*/

	float clear_color[] = { 0.21f, 0.21f, 0.21f, 1.0f };
	device_context_pointer_->ClearRenderTargetView( render_target_view_pointer_, clear_color );
	device_context_pointer_->ClearDepthStencilView( depth_stencil_view_pointer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	// constant buffer list
	if (shader_manager_)
	{
		const UMDirectX11ShaderManager::BufferPointerList& constant_buffer = shader_manager_->constant_buffer_list();

		// put camera to constant buffer
		if (!constant_buffer.empty())
		{
			ID3D11Buffer* first_constant = constant_buffer[0];
			D3D11_MAPPED_SUBRESOURCE resource;

			HRESULT hr = device_context_pointer_->Map(first_constant, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
			if( FAILED( hr ) ) { return false; }
			// TODO
			//::CopyMemory( resource.pData, &scene.camera()->mutable_world_view_projection_matrix(), sizeof(um_matrix) );
		
			device_context_pointer_->Unmap( first_constant, 0 );
		}

		// shader list
		const UMDirectX11ShaderManager::ShaderList& shaders = shader_manager_->shader_list();
	
		// apply constant buffer
		if (!constant_buffer.empty())
		{
			device_context_pointer_->VSSetConstantBuffers( 0, 1, &(constant_buffer[0]) );
		}

		// apply vertex shader
		if (device_context_pointer_ && !shaders.empty())
		{
			device_context_pointer_->VSSetShader( shaders[0]->vertex_shader_pointer(), NULL, 0 );

			// apply hull shader
			device_context_pointer_->HSSetShader( NULL, NULL, 0 );

			// apply domain shader
			device_context_pointer_->DSSetShader( NULL, NULL, 0 );

			// apply geometry shader
			device_context_pointer_->GSSetShader( NULL, NULL, 0 );

			// apply pixel shader
			device_context_pointer_->PSSetShader( shaders[1]->pixel_shader_pointer(), NULL, 0 );

			// apply compute shader
			device_context_pointer_->CSSetShader( NULL, NULL, 0 );

			//// draw
			//int index_count = 0;
			//device_context_pointer_->DrawIndexed(index_count, 0, 0 );
		}
	}

	if (dxgi_swap_chain_pointer_)
	{
		dxgi_swap_chain_pointer_->Present(0, 0);
	}

	return true;
}

} // burger
/**
 * @file UMDirectX11Renderer.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11Renderer.h"
#include "UMDirectX11ShaderManager.h"

namespace burger
{

UMDirectX11Renderer::UMDirectX11Renderer(ID3D11Device *device_pointer)
	: shader_manager_(std::make_shared<UMDirectX11ShaderManager>()),
	device_pointer_(device_pointer)
{}

UMDirectX11Renderer::~UMDirectX11Renderer()
{
}

/**
 * initialize
 */
bool UMDirectX11Renderer::init()
{
	if (device_pointer_)
	{
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
 * render
 */
bool UMDirectX11Renderer::render(const UMScene& scene, UMRenderParameter& parameter)
{
	//todo
	ID3D11DeviceContext *device_context_pointer(NULL);

	/*
	std::vector<UMDirectX11BufferObject>& objects = mutable_model_loader().mutable_buffer_objects();
	UINT stride = 3;//sizeof(um_vector3);
	UINT index_count = 0;
	UINT offset = 0;
	for (size_t i = 0, size = objects.size(); i < size; ++i) {
		device_context_pointer->IASetVertexBuffers(0, 1, objects[i].pp_vertex_buffer(), &stride, &offset);
		device_context_pointer->IASetIndexBuffer(objects[i].p_index_buffer(), DXGI_FORMAT_R32_SINT, 0);
		index_count += objects[i].face_size() * 3;
	}
	*/

	// constant buffer list
	const UMDirectX11ShaderManager::BufferPointerList& constant_buffer = shader_manager().constant_buffer_list();

	// put camera to constant buffer
	{
		ID3D11Buffer* first_constant = constant_buffer[0];
		D3D11_MAPPED_SUBRESOURCE resource;

		HRESULT hr = device_context_pointer->Map(first_constant, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		if( FAILED( hr ) ) { return false; }
		// TODO
		//::CopyMemory( resource.pData, &scene.camera()->mutable_world_view_projection_matrix(), sizeof(um_matrix) );
		
		device_context_pointer->Unmap( first_constant, 0 );
	}

	// shader list
	const UMDirectX11ShaderManager::ShaderList& shaders = shader_manager().shader_list();
	
	// apply constant buffer
	device_context_pointer->VSSetConstantBuffers( 0, 1, &(constant_buffer[0]) );

	// apply vertex shader
	device_context_pointer->VSSetShader( shaders[0]->vertex_shader_pointer(), NULL, 0 );

	// apply hull shader
	device_context_pointer->HSSetShader( NULL, NULL, 0 );

	// apply domain shader
	device_context_pointer->DSSetShader( NULL, NULL, 0 );

	// apply geometry shader
	device_context_pointer->GSSetShader( NULL, NULL, 0 );

	// apply pixel shader
	device_context_pointer->PSSetShader( shaders[1]->pixel_shader_pointer(), NULL, 0 );

	// apply compute shader
	device_context_pointer->CSSetShader( NULL, NULL, 0 );

	// draw
	int index_count = 0;
	device_context_pointer->DrawIndexed(index_count, 0, 0 );

	return true;
}

} // burger
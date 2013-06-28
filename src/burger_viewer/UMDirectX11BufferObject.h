/**
 * @file UMDirectX11BufferObject.h
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
#include "UMMacro.h"

namespace burger
{

/**
 * directx buffers of single model data
 */
class UMDirectX11BufferObject 
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11BufferObject);
public:

	UMDirectX11BufferObject(ID3D11Device *device);

	UMDirectX11BufferObject() {}

	ID3D11Buffer *index_buffer_pointer() { return reinterpret_cast<ID3D11Buffer*>(index_buffer_pointer_); };
	ID3D11Buffer *vertex_buffer_pointer() { return reinterpret_cast<ID3D11Buffer*>(vertex_buffer_pointer_); };
	ID3D11Buffer *normal_buffer_pointer() { return reinterpret_cast<ID3D11Buffer*>(normal_buffer_pointer_); };
	ID3D11Buffer *vertex_color_buffer_pointer() { return reinterpret_cast<ID3D11Buffer*>(vertex_color_buffer_pointer_); };
	ID3D11Buffer *uv_buffer_pointer() { return reinterpret_cast<ID3D11Buffer*>(uv_buffer_pointer_); };

	// StreamSource layout
	ID3D11InputLayout *input_layout_pointer() { return reinterpret_cast<ID3D11InputLayout*>(input_layout_pointer_); };
	
private:
	ID3D11Resource *index_buffer_pointer_;
	ID3D11Resource *vertex_buffer_pointer_;
	ID3D11Resource *normal_buffer_pointer_;
	ID3D11Resource *vertex_color_buffer_pointer_;
	ID3D11Resource *uv_buffer_pointer_;
	ID3D11InputLayout *input_layout_pointer_;

};

} // burger
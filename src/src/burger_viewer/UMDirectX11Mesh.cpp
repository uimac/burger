/**
 * @file UMDirectX11Mesh.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11Mesh.h"

#include "UMMacro.h"

namespace burger
{

UMDirectX11Mesh::UMDirectX11Mesh()
	:
	index_buffer_pointer_(NULL),
	normal_buffer_pointer_(NULL),
	vertex_color_buffer_pointer_(NULL),
	uv_buffer_pointer_(NULL)
{}

UMDirectX11Mesh::~UMDirectX11Mesh()
{
	SAFE_RELEASE(index_buffer_pointer_);
	SAFE_RELEASE(vertex_buffer_pointer_);
	SAFE_RELEASE(normal_buffer_pointer_);
	SAFE_RELEASE(vertex_color_buffer_pointer_);
	SAFE_RELEASE(uv_buffer_pointer_);
}

} // burger
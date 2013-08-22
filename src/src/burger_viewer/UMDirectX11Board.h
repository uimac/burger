/**
 * @file UMDirectX11Board.h
 * a textured rectangle
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
#include "UMVector.h"
#include "UMDirectX11ShaderManager.h"

namespace burger
{

class UMDirectX11Board;
typedef std::shared_ptr<UMDirectX11Board> UMDirectX11BoardPtr;

class UMDirectX11Texture;

/**
 * a textured rectangle
 */
class UMDirectX11Board
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11Board);

public:
	
	/**
	 * constructor. create board on x-y plane
	 * @param [in] left_top left top position
	 * @param [in] right_bottom right bottom position
	 * @param [in] z z position
	 */
	UMDirectX11Board(
		UMVec2f left_top,
		UMVec2f right_bottom,
		float z);

	/**
	 * destructor
	 */
	~UMDirectX11Board();

	/** 
	 * initialize
	 */
	bool init(ID3D11Device *device_pointer);
	
	/**
	 * refresh board
	 */
	void refresh(
		ID3D11Device* device_pointer,
		UMDirectX11Texture& texture);

private:
	UMVec3f p1_;
	UMVec2f uv1_;

	UMVec3f p2_;
	UMVec2f uv2_;

	UMVec3f p3_;
	UMVec2f uv3_;

	UMVec3f p4_;
	UMVec2f uv4_;
	
	UMDirectX11ShaderManagerPtr shader_manager_;
	ID3D11Buffer *vertex_buffer_pointer_;
};

} // burger

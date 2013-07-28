/**
 * @file UMDirectX11Texture.h
 * a texture
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
#include <string>

#include "UMMacro.h"
#include "UMImage.h"

namespace burger
{

/**
 * a texture
 */
class UMDirectX11Texture
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11Texture);

public:
	/**
	 * constructor
	 * @param [in] can_overwrite if true,  can overwrite()
	 */
	explicit UMDirectX11Texture(bool can_overwrite) : 
		can_overwrite_(can_overwrite), 
		resource_view_pointer_(NULL),
		texture_2d_(NULL)
	{}

	/**
	 * destructor
	 */
	~UMDirectX11Texture() { 
		SAFE_RELEASE(resource_view_pointer_);
		SAFE_RELEASE(texture_2d_);
	}
	
	/**
	 * get resource view pointer
	 */
	ID3D11ShaderResourceView* resource_view_pointer() { 
		return resource_view_pointer_;
	}

	/**
	 * load an image file as a directx texture
	 * @param [in] device_pointer directx11 device pointer
	 * @param [in] file_path absolute texture file path
	 * @retval success or fail
	 */
	bool load(
		ID3D11Device *device_pointer,
		const std::u16string& file_path);

	/**
	 * convert UMImage to DirectX11 Texture
	 * @param [in] device_pointer  directx11 device pointer
	 * @param [in] image source image
	 * @retval success or fail
	 */
	bool convert_from_image(
		ID3D11Device *device_pointer,
		const UMImage& image);

	/**
	 * convert DirectX texture buffer to memory
	 * @param [in] device_pointer  directx11 device pointer
	 * @param [in] device_context_pointer directx11 device pointer context
	 * @retval UMImagePtr converted image buffer
	 */
	UMImagePtr convert_to_image(
		ID3D11Device *device_pointer,
		ID3D11DeviceContext *device_context_pointer) const;

	/**
	 * overwrite texture by image
	 * @info only valid when can_overwrite_ is true
	 */
	bool overwrite(
		ID3D11Device *device_pointer,
		ID3D11DeviceContext *device_context_pointer,
		const UMImage& image);

private:
	bool can_overwrite_;
	ID3D11ShaderResourceView* resource_view_pointer_;
	ID3D11Texture2D * texture_2d_;
};

} // burger

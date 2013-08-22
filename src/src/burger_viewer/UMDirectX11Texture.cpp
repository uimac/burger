/**
 * @file UMDirectX11Texture.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11Texture.h"

#include <d3dx11.h>

#include "UMVector.h"
#include "UMStringUtil.h"

namespace burger
{

/**
 * load an image file as a directx texture
 * @param [in] device_pointer directx11 device pointer
 * @param [in] file_path absolute texture file path
 */
bool UMDirectX11Texture::load(
		ID3D11Device *device_pointer,
		const std::u16string& file_path)
{
	if (!device_pointer) return false;
	if (resource_view_pointer_) return false;
	if (texture_2d_) return false;

	if FAILED(D3DX11CreateShaderResourceViewFromFile(
		device_pointer,
		UMStringUtil::utf16_to_wstring(file_path).c_str(),
		NULL,
		NULL,
		&resource_view_pointer_,
		NULL))
	{
		resource_view_pointer_ = NULL;
		return false;
	}
	
	ID3D11DeviceContext* device_context(NULL);
	device_pointer->GetImmediateContext(&device_context);
	
	// create default sampler state
	{
		D3D11_SAMPLER_DESC sampler_desc;
		::ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		if FAILED(device_pointer->CreateSamplerState(
			&sampler_desc, 
			&sampler_state_pointer_ ))
		{
			SAFE_RELEASE(device_context);
			return false;
		}
	}

	if (can_overwrite_)
	{
		UMImagePtr image = convert_to_image(device_pointer, device_context);
		if (image)
		{
			UMImagePtr fliped = image->create_flip_image(false, true);
			if (fliped)
			{
				SAFE_RELEASE(resource_view_pointer_);
				convert_from_image(device_pointer, *fliped);
			}
		}
	}

	SAFE_RELEASE(device_context);

	return true;
}

/**
 * convert UMImage to DirectX11 Texture
 * @param [in] image source image
 * @retval success or fail
s */
bool UMDirectX11Texture::convert_from_image(
	ID3D11Device *device_pointer,
	const UMImage& image)
{
	if (resource_view_pointer_) return false;
	if (texture_2d_) return false;
	if (!image.is_valid()) return false;

	UMImagePtr filped = image.create_flip_image(false, true);
	if (!filped) return false;

	UMImage::R8G8B8A8Buffer buffer;
	filped->create_r8g8b8a8_buffer(buffer);

	D3D11_TEXTURE2D_DESC texture_desc;
	::ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
	texture_desc.Width = image.width();
	texture_desc.Height = image.height();
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_desc.SampleDesc.Count   = 1;
	texture_desc.SampleDesc.Quality = 0;
	if (can_overwrite_)
	{
		texture_desc.Usage = D3D11_USAGE_DYNAMIC;
		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.CPUAccessFlags = 0; // don't access from cpu
	}
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA  data;
	::ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &buffer.front();
	data.SysMemPitch = image.width() * 4;

	if FAILED(device_pointer->CreateTexture2D(
		&texture_desc, 
		&data, 
		&texture_2d_))
	{
		return false;
	}
	if FAILED(device_pointer->CreateShaderResourceView(
		texture_2d_, 
		NULL, 
		&resource_view_pointer_))
	{
		return false;
	}
	
	ID3D11DeviceContext* device_context(NULL);
	device_pointer->GetImmediateContext(&device_context);

	// create default sampler state
	if (!sampler_state_pointer_)
	{
		D3D11_SAMPLER_DESC sampler_desc;
		::ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		if FAILED(device_pointer->CreateSamplerState(
			&sampler_desc, 
			&sampler_state_pointer_ ))
		{
			SAFE_RELEASE(device_context);
			return false;
		}
	}
	SAFE_RELEASE(device_context);

	return true;
}

/**
 * convert DirectX texture buffer to memory
 * @param [in] device_pointer directx11 device pointer
 * @retval UMImagePtr converted image buffer
 */
UMImagePtr UMDirectX11Texture::convert_to_image(
	ID3D11Device *device_pointer,
	ID3D11DeviceContext *device_context_pointer) const
{
	if (!device_pointer) return UMImagePtr();
	if (!device_context_pointer) return UMImagePtr();
	if (!resource_view_pointer_) return UMImagePtr();

	// simple 2d texture ?
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	resource_view_pointer_->GetDesc(&desc);
	if (desc.ViewDimension != D3D11_SRV_DIMENSION_TEXTURE2D) {
		return UMImagePtr();
	}

	// validate format
	if (desc.Format == DXGI_FORMAT_R8G8B8A8_UNORM ||
		desc.Format == DXGI_FORMAT_R8G8B8A8_UINT)
	{
		// get original texture resrouce
		ID3D11Resource * texture = NULL;
		resource_view_pointer_->GetResource(&texture);

		// get original texture2d desc
		D3D11_TEXTURE2D_DESC  texture2d_desc;
		if (ID3D11Texture2D* texture2d = static_cast<ID3D11Texture2D*>(texture))
		{
			texture2d->GetDesc(&texture2d_desc);
		}

		// create readable copy
		D3D11_TEXTURE2D_DESC  copy_desc;
		ZeroMemory(&copy_desc, sizeof(copy_desc));
		copy_desc.Width = texture2d_desc.Width;
		copy_desc.Height = texture2d_desc.Height;
		copy_desc.MipLevels = texture2d_desc.MipLevels;
		copy_desc.ArraySize  = texture2d_desc.ArraySize;
		copy_desc.Format = texture2d_desc.Format;
		copy_desc.SampleDesc = texture2d_desc.SampleDesc;
		copy_desc.Usage = D3D11_USAGE_STAGING;
		copy_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		ID3D11Texture2D* copy_texture = NULL;
		if FAILED(device_pointer->CreateTexture2D(&copy_desc, NULL, &copy_texture))
		{
			return false;
		}
		device_context_pointer->CopyResource(copy_texture, texture);
		
		// release original
		if (texture) { SAFE_RELEASE(texture); }

		// map texture
		D3D11_MAPPED_SUBRESOURCE subresource;
		if FAILED(device_context_pointer->Map(copy_texture, 0, D3D11_MAP_READ, 0, &subresource))
		{
			if (copy_texture)  { SAFE_RELEASE(copy_texture); }
			return false;
		}

		// create buffer and read pixels
		UMImagePtr image(std::make_shared<UMImage>());
		image->set_width(texture2d_desc.Width);
		image->set_height(texture2d_desc.Height);
		image->mutable_list().resize(image->width() * image->height());
		const double ffinv = 1.0 / static_cast<double>(0xFF);
		unsigned char* data_pointer = reinterpret_cast<unsigned char*>(subresource.pData);
		for (int y = 0, height = image->height(); y < height; ++y)
		{
			for (int x = 0, width = image->width(); x < width; ++x)
			{
				const int pos = y * width * 4 + x * 4;
				if (desc.Format == DXGI_FORMAT_R8G8B8A8_UNORM)
				{
					int r = data_pointer[ pos + 0 ];
					int g = data_pointer[ pos + 1 ];
					int b = data_pointer[ pos + 2 ];
					int a = data_pointer[ pos + 3 ];
					UMVec4d& dst = image->mutable_list()[ y * width + x ];
					dst.x = r*ffinv;
					dst.y = g*ffinv;
					dst.z = b*ffinv;
					dst.w = a*ffinv;
				}
			}
		}

		// unmap texture
		device_context_pointer->Unmap(copy_texture, 0);
		if (copy_texture) { SAFE_RELEASE(copy_texture); }

		return image;
	}
	return UMImagePtr();
}


/**
 * overwrite texture by image
 * @info only valid when can_overwrite_ is true
 */
bool UMDirectX11Texture::overwrite(
	ID3D11Device *device_pointer,
	ID3D11DeviceContext *device_context_pointer,
	const UMImage& image)
{
	if (!can_overwrite_) return false;
	if (!device_pointer) return false;
	if (!device_context_pointer) return false;
	if (!resource_view_pointer_) return false;

	// simple 2d texture ?
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	resource_view_pointer_->GetDesc(&desc);
	if (desc.ViewDimension != D3D11_SRV_DIMENSION_TEXTURE2D) {
		return false;
	}

	// validate format
	if (desc.Format == DXGI_FORMAT_R8G8B8A8_UNORM ||
		desc.Format == DXGI_FORMAT_R8G8B8A8_UINT)
	{
		// get original texture resrouce
		ID3D11Resource * texture = texture_2d_;
		if (!texture_2d_)
		{
			resource_view_pointer_->GetResource(&texture);
		}
		
		// map texture
		D3D11_MAPPED_SUBRESOURCE subresource;
		if FAILED(device_context_pointer->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource))
		{
			if (texture)  { SAFE_RELEASE(texture); }
			return false;
		}
		
		const double inv_gamma = 1.0 / 2.2;
		unsigned char* data_pointer = reinterpret_cast<unsigned char*>(subresource.pData);
		for (int y = 0, height = image.height(); y < height; ++y)
		{
			for (int x = 0, width = image.width(); x < width; ++x)
			{
				const int pos = y * width * 4 + x * 4;
				if (desc.Format == DXGI_FORMAT_R8G8B8A8_UNORM)
				{
					const UMVec4d& color = image.list()[ (height-y-1) * width + x ];
					data_pointer[ pos + 0 ] = static_cast<int>(pow(color.x, inv_gamma) * 0xFF + 0.5);
					data_pointer[ pos + 1 ] = static_cast<int>(pow(color.y, inv_gamma) * 0xFF + 0.5);
					data_pointer[ pos + 2 ] = static_cast<int>(pow(color.z, inv_gamma) * 0xFF + 0.5);
					data_pointer[ pos + 3 ] = static_cast<int>(pow(color.w, inv_gamma) * 0xFF + 0.5);
				}
			}
		}

		// unmap texture
		device_context_pointer->Unmap(texture, 0);
		if (!texture_2d_)
		{
			SAFE_RELEASE(texture);
		}
		return true;
	}
	return false;
}

} // burger

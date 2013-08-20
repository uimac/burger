/**
 * @file UMDirectX11Material.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11Material.h"
#include "UMDirectX11Texture.h"

namespace
{
	using namespace burger;

	UMVec2f to_dx(const burger::UMVec2d& v) { return UMVec2f((float)v.x, (float)v.y); }

	UMVec3f to_dx(const burger::UMVec3d& v) { return UMVec3f((float)v.x, (float)v.y, (float)v.z); }

	UMVec4f to_dx(const burger::UMVec4d& v) { return UMVec4f((float)v.x, (float)v.y, (float)v.z, (float)v.w); } 

} // anonymouse namespace

namespace burger
{

UMDirectX11Material::~UMDirectX11Material()
{
}

/** 
 * initialize
 */
bool UMDirectX11Material::init(ID3D11Device *device_pointer)
{
	if (!device_pointer) return false;
	
	ID3D11DeviceContext* device_context_pointer(NULL);
	device_pointer->GetImmediateContext(&device_context_pointer);

	if (UMMaterialPtr material = ummaterial())
	{
		set_ambient(to_dx(material->ambient()));
		set_diffuse(to_dx(material->diffuse()));
		set_specular(to_dx(material->specular()));
		set_polygon_count(material->polygon_count());

		UMMaterial::TexturePathList::const_iterator it = material->texture_path_list().begin();
		for (; it != material->texture_path_list().end(); ++it)
		{
			const std::u16string& path = *it;
			if (diffuse_texture_->load(device_pointer, path))
			{
				UMImagePtr image = diffuse_texture_->convert_to_image(device_pointer, device_context_pointer);
				material->mutable_texture_list().push_back(image);
			}
		}
	}

	// load first texture
	// TODO: multiple texture loading
	if (!texture_path_list_.empty())
	{
		diffuse_texture_->load(device_pointer, texture_path_list_.at(0));
	}

	SAFE_RELEASE(device_context_pointer);

	return true;
}

/**
 * refresh
 */
void UMDirectX11Material::refresh(ID3D11Device* device_pointer)
{
	if (!device_pointer) return;
	
	UMMaterialPtr material = ummaterial();
	if (!material) return;
	
}

} // burger

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
	UMMaterialPtr material = ummaterial();
	if (!material) return false;
	
	set_ambient(to_dx(material->ambient()));
	set_diffuse(to_dx(material->diffuse()));
	set_specular(to_dx(material->specular()));
	set_polygon_count(material->polygon_count());

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

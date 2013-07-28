/**
 * @file UMDirectX11Material.h
 * a material
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
#include <vector>

#include "UMMacro.h"
#include "UMMath.h"
#include "UMMaterial.h"

namespace burger
{

class UMDirectX11Material;
typedef std::shared_ptr<UMDirectX11Material> UMDirectX11MaterialPtr;
typedef std::vector<UMDirectX11MaterialPtr> UMDirectX11MaterialList;

/**
 * material
 */
class UMDirectX11Material
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11Material);

public:
	
	/**
	 * constructor.
	 */
	explicit UMDirectX11Material(UMMaterialPtr ummaterial) :
		ummaterial_(ummaterial),
		polygon_count_(0)
	{}

	/**
	 * destructor
	 */
	~UMDirectX11Material();

	static UMDirectX11MaterialPtr default_material() { 
		UMDirectX11MaterialPtr material(std::make_shared<UMDirectX11Material>(UMMaterialPtr()));
		material->set_diffuse(UMVec4f(0.0f, 0.7f, 0.7f, 1.0f));
		material->set_specular(UMVec4f(0.9f, 0.9f, 0.9f, 1.0f));
		material->set_ambient(UMVec4f(0.3f, 0.3f, 0.3f, 1.0f));
		return material;
	}

	/** 
	 * initialize
	 */
	bool init(ID3D11Device *device_pointer);
	
	/**
	 * refresh
	 */
	void refresh(ID3D11Device* device_pointer);

	/**
	 * get diffuse
	 */
	const UMVec4f& diffuse() const { return diffuse_; }
	
	/**
	 * set diffuse
	 */
	void set_diffuse(const UMVec4f& diffuse) { diffuse_ = diffuse; }

	/**
	 * get specular
	 */
	const UMVec4f& specular() const { return specular_; }
	
	/**
	 * set ambient
	 */
	void set_specular(const UMVec4f& specular) { specular_ = specular; }

	/**
	 * get ambient
	 */
	const UMVec4f& ambient() const { return ambient_; }

	/**
	 * set ambient
	 */
	void set_ambient(const UMVec4f& ambient) { ambient_ = ambient; }

	/**
	 * get polygon count
	 */
	int polygon_count() const { return polygon_count_; }

	/**
	 * set polygon count
	 */
	void set_polygon_count(int count) { polygon_count_ = count; }

	/**
	 * set ummaterial
	 */
	void set_ummaterial(UMMaterialPtr ummaterial) { ummaterial_ = ummaterial; }

	/**
	 * get ummaterial
	 */
	UMMaterialPtr ummaterial() { return ummaterial_.lock(); }
	
	/**
	 * get ummaterial
	 */
	UMMaterialPtr ummaterial() const { return ummaterial_.lock(); }

private:
	UMVec4f diffuse_;
	// .w is specular_factor;
	UMVec4f specular_;
	UMVec4f ambient_;
	
	int polygon_count_;
	UMMaterialWeakPtr ummaterial_;
};

} // burger

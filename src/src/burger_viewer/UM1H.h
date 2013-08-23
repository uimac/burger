/**
 * @file UM1H.h
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

#include <memory>
#include "UMMacro.h"
#include "UMDirectX11Texture.h"

namespace burger
{

class UM1H;
typedef std::shared_ptr<UM1H> UM1HPtr;

class UMDirectX11Scene;
typedef std::shared_ptr<UMDirectX11Scene> UMDirectX11ScenePtr;
typedef std::weak_ptr<UMDirectX11Scene> UMDirectX11SceneWeakPtr;

/**
 * for #rendering1h
 */
class UM1H
{
	DISALLOW_COPY_AND_ASSIGN(UM1H);

public:

	/**
	 * constructor
	 */
	UM1H();

	~UM1H();

	bool refresh(ID3D11Device* device_pointer, UMDirectX11Texture& texture);

	void set_scene(UMDirectX11ScenePtr scene) { scene_ = scene; }

	UMDirectX11ScenePtr mutable_scene() { return scene_.lock(); }

private:
	unsigned long initial_time_;
	unsigned int current_number_;

	UMDirectX11SceneWeakPtr scene_;

	bool change_model(ID3D11Device* device_pointer, unsigned long minutes);
};

} // burger

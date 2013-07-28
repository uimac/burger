/**
 * @file UMDirectX11Scene.h
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
#include "UMScene.h"
#include "UMRenderer.h"
#include "UMRenderParameter.h"

#include "UMMeshGroup.h"
#include "UMDirectX11MeshGroup.h"
#include "UMDirectX11ShaderManager.h"
#include "UMDirectX11Texture.h"
#include "UMDirectX11Board.h"
#include "UMDirectX11Light.h"
#include "UMDirectX11Camera.h"
#include "UMTime.h"

namespace burger
{

/**
 * directx11 secne.
 */
class UMDirectX11Scene 
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11Scene);
public:

	UMDirectX11Scene();

	~UMDirectX11Scene();
	
	/**
	 * init scene
	 */
	bool init(ID3D11Device *device, int width, int height);
	
	/**
	 * load *.bos file
	 */
	bool load(ID3D11Device* device_pointer, const std::u16string& absolute_bos_filepath);

	/**
	 * refresh scene
	 */
	void refresh(ID3D11Device* device_pointer);

	/**
	 * render by burger
	 */
	void render(bool progressive);

	/**
	 * get burger renderer scene
	 */
	const UMScene& render_scene() const { return render_scene_; }

	/**
	 * get dx11 mesh group list
	 */
	const UMDirectX11MeshGroupList& dx11_mesh_group_list() const { return dx11_mesh_group_list_; }

	/** 
	 * is rendering or not
	 */
	bool is_rendering() const { return is_rendering_; }

	/**
	 * sample scene
	 */
	void create_sample_scene(ID3D11Device* device_pointer);

private:
	// burger scene
	UMScene render_scene_;
	
	// burger render parameters
	UMRendererPtr renderer_;
	UMRenderParameter render_parameter_;
	UMTimePtr render_time_;
	bool is_rendering_;
	bool is_rendering_done_;

	// shader
	UMDirectX11ShaderManagerPtr shader_manager_;

	// drawable objects
	UMDirectX11MeshGroupList dx11_mesh_group_list_;
	UMDirectX11LightList dx11_light_list_;
	UMDirectX11BoardPtr board_;
	UMDirectX11CameraPtr camera_;

	// output texture
	ID3D11SamplerState * render_result_sampler_state_pointer_;
	UMDirectX11Texture output_texture_;
};

} // burger

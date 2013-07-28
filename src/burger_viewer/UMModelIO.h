/**
 * @file UMModelIO.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"

#include "UMIO.h"
#include "UMDirectX11Scene.h"

namespace burger
{

/**
 * model in/out
 */
class UMModelIO
{
	DISALLOW_COPY_AND_ASSIGN(UMModelIO);

public:
	
	/** 
	 * import burger mesh list
	 * @param [out] dst distination mesh list
	 * @param [in] src source object
	 */
	static bool import_mesh_list(
		UMMeshList& dst, 
		const umio::UMObjectPtr src);
	
	/**
	 * import directx11 mesh list
	 * @param [in] device directx11 device pointer
	 * @param [out] dst distination mesh list
	 * @param [in] src source object
	 */
	static bool import_dx11_mesh_list(
		ID3D11Device *device_pointer,
		UMDirectX11MeshList& dst, 
		const umio::UMObjectPtr src);

	/**
	 * convert burger mesh to directx11 mesh
	 * @param [in] device directx11 device pointer
	 * @param [in] src source burger mesh
	 * @retval UMDirectX11MeshPtr converted dx11 mesh or none
	 */
	static UMDirectX11MeshPtr convert_mesh_to_dx11_mesh(
		ID3D11Device *device_pointer,
		UMMeshPtr src);
	
	/**
	 * convert burger mesh group to directx11 mesh
	 * @param [in] device directx11 device pointer
	 * @param [in] src source burger mesh group
	 * @retval UMMeshGroupPtr converted dx11 mesh group or none
	 */
	static UMDirectX11MeshGroupPtr convert_mesh_group_to_dx11_mesh_group(
		ID3D11Device *device_pointer,
		UMMeshGroupPtr src);
	
	/**
	 * convert burger light to directx11 light
	 * @param [in] device directx11 device pointer
	 * @param [in] src source burger light
	 * @retval UMDirectX11LightPtr converted dx11 light or none
	 */
	static UMDirectX11LightPtr convert_light_to_dx11_light(
		ID3D11Device *device_pointer,
		UMLightPtr src);
	
	/**
	 * convert burger material to directx11 material
	 * @param [in] device directx11 device pointer
	 * @param [in] src source burger material
	 * @retval UMDirectX11MaterialPtr converted dx11 material or none
	 */
	static UMDirectX11MaterialPtr convert_material_to_dx11_material(
		ID3D11Device *device_pointer,
		UMMaterialPtr src);
	
	/**
	 * convert burger camera to directx11 camera
	 * @param [in] device directx11 device pointer
	 * @param [in] src source burger camera
	 * @retval UMCameraPtr converted dx11 camera or none
	 */
	static UMDirectX11CameraPtr convert_camera_to_dx11_camera(
		ID3D11Device *device_pointer,
		UMCameraPtr src);
};

} // burger

/**
 * @file UMDirectX11Scene.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11Scene.h"

#include "UMRaytracer.h"
#include "UMMesh.h"
#include "UMTriangle.h"
#include "UMSphere.h"
#include "UMPlane.h"
#include "UMMeshGroup.h"

#include "UMIO.h"
#include "UMStringUtil.h"
#include "UMTime.h"
#include "UMDirectX11Board.h"
#include "UMModelIO.h"

namespace burger
{

UMDirectX11Scene::UMDirectX11Scene()
	: shader_manager_(std::make_shared<UMDirectX11ShaderManager>()),
	output_texture_(true),
	// renderer
	renderer_(std::make_shared<UMRaytracer>()),
	is_rendering_(false),
	is_rendering_done_(false)
{
//	D3D11_INPUT_ELEMENT_DESC desc;
//	device->CreateInputLayout( vElementPOSNORCOL, input_layout_pointer() );
}

UMDirectX11Scene::~UMDirectX11Scene()
{
	SAFE_RELEASE(render_result_sampler_state_pointer_);
}

/**
 * init scene
 */
bool UMDirectX11Scene::init(ID3D11Device *device_pointer, int width, int height)
{
	if (!device_pointer) return false;
	if (!shader_manager_) return false;
	
	ID3D11DeviceContext *device_context_pointer = NULL;
	device_pointer->GetImmediateContext(&device_context_pointer);

	renderer_->init();
	renderer_->set_width(width);
	renderer_->set_height(height);

	render_scene_.init(width, height);
	create_sample_scene(device_pointer);

	// light
	UMLightPtr umlight = std::make_shared<UMLight>();
	umlight->set_position(UMVec3d(1000, 500, 500));
	render_scene_.mutable_light_list().push_back(umlight);
	
	UMDirectX11LightPtr light = UMModelIO::convert_light_to_dx11_light(device_pointer, umlight);
	dx11_light_list_.push_back(light);

	// camera
	camera_ = UMModelIO::convert_camera_to_dx11_camera(device_pointer, render_scene_.camera());

	// init shader manager for camera
	shader_manager_->init(device_pointer, UMDirectX11ShaderManager::eConstants);

	// init output image
	render_parameter_.mutable_output_image().set_width(width);
	render_parameter_.mutable_output_image().set_height(height);
	UMImage::ImageBuffer& buffer = render_parameter_.mutable_output_image().mutable_list();
	buffer.resize(width*height);

	// create texture
	if (render_parameter_.output_image().is_valid())
	{
		const UMImage& image = render_parameter_.output_image();
		output_texture_.convert_from_image(device_pointer, image);

		// sampler state
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
			&render_result_sampler_state_pointer_ ))
		{
			SAFE_RELEASE(device_context_pointer);
			return false;
		}
	}
	
	// create a board
	{
		board_ = std::make_shared<UMDirectX11Board>(
			UMVec2f(-1.0f, 1.0f),
			UMVec2f(1.0f, -1.0f),
			0.5f);
		board_->init(device_pointer);
	}
	SAFE_RELEASE(device_context_pointer);

	return true;
}

/**
 * render by burger
 */
void UMDirectX11Scene::render(bool is_progressive)
{
	const int width = renderer_->width();
	const int height = renderer_->height();
	
	if (!is_rendering_ && !is_rendering_done_)
	{
		render_time_ = std::make_shared<UMTime>("render_time", true);
		is_rendering_ = true;
	}
	
	if (is_progressive)
	{
		if (!renderer_->progress_render(render_scene(), render_parameter_))
		{
			is_rendering_done_ = true;
		}
	}
	else
	{
		renderer_->render(render_scene(), render_parameter_);
		is_rendering_done_ = true;
	}
	
	if (is_rendering_done_)
	{
		is_rendering_ = false;
		is_rendering_done_ = false;
		render_time_ = UMTimePtr();

		render_parameter_.mutable_output_image().clear();
		renderer_->init();
	}
}

/**
 * refresh scene
 */
void UMDirectX11Scene::refresh(ID3D11Device* device_pointer)
{
	if (!device_pointer) return;
	if (!shader_manager_) return;

	ID3D11DeviceContext *device_context_pointer = NULL;
	device_pointer->GetImmediateContext(&device_context_pointer);
	
	// refresh lights
	{
		UMDirectX11LightList::iterator it = dx11_light_list_.begin();
		for (; it != dx11_light_list_.end(); ++it)
		{
			UMDirectX11LightPtr light = (*it);
			light->refresh(device_pointer);
		}
	}

	// update camera
	if (camera_)
	{
		camera_->refresh(device_pointer);
	}
	
	// apply constant buffer
	const UMDirectX11ShaderManager::BufferPointerList& constant_buffer = shader_manager_->constant_buffer_list();
	if (!constant_buffer.empty())
	{
		// put camera to constant buffer
		{
			ID3D11Buffer* constant = constant_buffer[0];
			UMDirectX11CameraPtr camera = camera_;
			device_context_pointer->UpdateSubresource(constant, 0, NULL, &(*camera), 0, 0);
			device_context_pointer->VSSetConstantBuffers( 0, 1, &constant );
		}

		// put light to constant buffer
		{
			ID3D11Buffer* constant = constant_buffer[1];
			UMDirectX11LightPtr light = dx11_light_list_.front();
			device_context_pointer->UpdateSubresource(constant, 0, NULL, &(*light), 0, 0);
			device_context_pointer->VSSetConstantBuffers( 1, 1, &constant );
			device_context_pointer->PSSetConstantBuffers( 1, 1, &constant );
		}
	}

	// refresh models
	{
		UMDirectX11MeshGroupList::iterator it = dx11_mesh_group_list_.begin();
		for (; it != dx11_mesh_group_list_.end(); ++it)
		{
			(*it)->refresh(device_pointer);
		}
	}

	if (is_rendering_)
	{
		// render
		render(true);

		// update image
		// TODO: this is slow
		const UMImage& image = render_parameter_.output_image();
		output_texture_.overwrite(device_pointer, device_context_pointer, image);

		// refresh board
		board_->refresh(device_pointer, render_result_sampler_state_pointer_, output_texture_);
	}

	SAFE_RELEASE(device_context_pointer);
}

/**
 * load *.bos file
 */
bool UMDirectX11Scene::load(
	ID3D11Device* device_pointer, 
	const std::u16string& absolute_bos_filepath)
{
	//UMTime time("mesh load time", true);

	std::wstring utf16path = UMStringUtil::utf16_to_wstring(absolute_bos_filepath);
	umio::UMIO io;
	umio::UMObjectPtr obj;
	obj = io.load(utf16path, umio::UMIOSetting());
	if (!obj) return false;
	
	// import to burger
	UMMeshGroupPtr mesh_group(std::make_shared<UMMeshGroup>());
	{
		if (!UMModelIO::import_mesh_list(
			mesh_group->mutable_mesh_list(), 
			obj))
		{
			return false;
		}
		render_scene_.mutable_mesh_group_list().push_back(mesh_group);

		UMMeshList::const_iterator it = mesh_group->mesh_list().begin();
		for (; it != mesh_group->mesh_list().end(); ++it)
		{
			UMMeshPtr mesh = *it;
			render_scene_.mutable_primitive_list().push_back(mesh);
		}
	}

	// import to directx11
	UMDirectX11MeshGroupPtr dx11_mesh_group = UMModelIO::convert_mesh_group_to_dx11_mesh_group(device_pointer, mesh_group);
	if (dx11_mesh_group) {
		dx11_mesh_group_list_.push_back(dx11_mesh_group);
	}
	return true;
}

/// sample scene
void UMDirectX11Scene::create_sample_scene(ID3D11Device* device_pointer)
{
	if (!device_pointer) return;

	// spheres
	UMSpherePtr sphere1(std::make_shared<UMSphere>(UMVec3d(-20, -10, -20), 20.0));
	UMSpherePtr sphere2(std::make_shared<UMSphere>(UMVec3d(20, -10, -20), 20.0));
	UMSpherePtr sphere3(std::make_shared<UMSphere>(UMVec3d(0, 25, -20), 20.0));
	sphere1->set_color(UMVec3d(0.0, 1.0, 0.0));
	sphere2->set_color(UMVec3d(0.0, 0.0, 1.0));
	sphere3->set_color(UMVec3d(1.0, 0.0, 0.0));

	// plane
	UMPlanePtr plane1(std::make_shared<UMPlane>(UMVec3d(0, -30, 0), UMVec3d(0, 1, 0)));
	plane1->set_color(UMVec3d(0.5, 1.0, 0.5));
	
	// convert to mesh
	UMMeshPtr mesh1 = sphere1->convert_to_mesh(32, 32);
	UMMeshPtr mesh2 = sphere2->convert_to_mesh(32, 32);
	UMMeshPtr mesh3 = sphere3->convert_to_mesh(32, 32);
	UMMeshPtr mesh4 = plane1->convert_to_mesh(5000, 5000);
	
	// add to render scene
	UMMeshGroupPtr umgroup(std::make_shared<UMMeshGroup>());
	umgroup->mutable_mesh_list().push_back(mesh1);
	umgroup->mutable_mesh_list().push_back(mesh2);
	umgroup->mutable_mesh_list().push_back(mesh3);
	umgroup->mutable_mesh_list().push_back(mesh4);
	render_scene_.mutable_mesh_group_list().push_back(umgroup);
	render_scene_.mutable_primitive_list().push_back(UMPrimitivePtr(sphere1));
	render_scene_.mutable_primitive_list().push_back(UMPrimitivePtr(sphere2));
	render_scene_.mutable_primitive_list().push_back(UMPrimitivePtr(sphere3));
	render_scene_.mutable_primitive_list().push_back(UMPrimitivePtr(plane1));

	// convert to dx11 group
	UMDirectX11MeshGroupPtr converted = UMModelIO::convert_mesh_group_to_dx11_mesh_group(device_pointer, umgroup);
	if (converted) {
		dx11_mesh_group_list_.push_back(converted);
	}
}

} // burger
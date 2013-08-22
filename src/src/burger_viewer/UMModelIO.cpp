/**
 * @file UMModelIO.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <string>
#include <assert.h>

#include "UMModelIO.h"
#include "UMStringUtil.h"
#include "UMPath.h"

namespace
{
	using namespace burger;

	double import_scale_for_debug = 1.0;
	
	UMVec2d to_um(const umio::UMVec2d& v) { return UMVec2d(v.x, v.y); }

	UMVec3d to_um(const umio::UMVec3d& v) { return UMVec3d(v.x, v.y, v.z); }

	UMVec4d to_um(const umio::UMVec4d& v) { return UMVec4d(v.x, v.y, v.z, v.w); }

	UMVec4d to_um_material_value(const umio::UMVec4d& v) { return UMVec4d(v.x, v.y, v.z, 1.0); }
	
	UMVec2i to_um(const umio::UMVec2i& v) { return UMVec2i(v.x, v.y); }

	UMVec3i to_um(const umio::UMVec3i& v) { return UMVec3i(v.x, v.y, v.z); }

	UMVec4i to_um(const umio::UMVec4i& v) { return UMVec4i(v.x, v.y, v.z, v.w); }
	
	UMVec2f to_dx(const umio::UMVec2d& v) { return UMVec2f((float)v.x, (float)v.y); }

	UMVec3f to_dx(const umio::UMVec3d& v) { return UMVec3f((float)v.x, (float)v.y, (float)v.z); }

	UMVec4f to_dx(const umio::UMVec4d& v) { return UMVec4f((float)v.x, (float)v.y, (float)v.z, (float)v.w); } 

	//----------------------------------------------------------------------------

	/** 
	 * load index from burger to dx11
	 */
	void load_vertex_index(
		ID3D11DeviceContext* device_context,
		UMDirectX11MeshPtr dst_mesh, 
		UMMeshPtr src_mesh)
	{	
		if (!device_context) return;
		D3D11_MAPPED_SUBRESOURCE resource;
		device_context->Map(dst_mesh->index_buffer_pointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		UMVec3i* index = (UMVec3i*) resource.pData;
		const UMMesh::Vec3iList& face_list = src_mesh->face_list();
		for (size_t i = 0, i_size = face_list.size(); i < i_size; ++i) {
			const UMVec3i& in = face_list.at(i);
			index[i].x = in.x;
			index[i].y = in.y;
			index[i].z = in.z;
		}
		device_context->Unmap(dst_mesh->index_buffer_pointer(), 0);
	}
	
	/**
	 * load vertex from burger to dx11
	 */
	void load_vertex(
		ID3D11DeviceContext* device_context,
		UMDirectX11MeshPtr dst_mesh, 
		UMMeshPtr src_mesh)
	{
		if (!device_context) return;
		D3D11_MAPPED_SUBRESOURCE resource;
		device_context->Map(dst_mesh->vertex_buffer_pointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		UMVec3f* dx_vertex = (UMVec3f*)resource.pData;
		const UMMesh::Vec3iList& face_list = src_mesh->face_list();
		const UMMesh::Vec3dList& vertex_list = src_mesh->vertex_list();

		for (size_t i = 0, i_size = face_list.size(); i < i_size; ++i) {
			const UMVec3i& face = face_list.at(i);
			for (int k = 0; k < 3; ++k)
			{
				const UMVec3d& vertex = vertex_list.at(face[k]);
				dx_vertex[i * 3 + k].x = static_cast<float>(vertex.x);
				dx_vertex[i * 3 + k].y = static_cast<float>(vertex.y);
				dx_vertex[i * 3 + k].z = static_cast<float>(vertex.z);
			}
		}
		device_context->Unmap(dst_mesh->vertex_buffer_pointer(), 0);
	}

	/**
	 * load normal from burger to dx11
	 */
	void load_normal(
		ID3D11DeviceContext* device_context,
		UMDirectX11MeshPtr dst_mesh, 
		UMMeshPtr src_mesh)
	{
		if (!device_context) return;
		D3D11_MAPPED_SUBRESOURCE resource;
		device_context->Map(dst_mesh->normal_buffer_pointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		UMVec3f* dx_normal = (UMVec3f*)resource.pData;
		const UMMesh::Vec3iList& face_list = src_mesh->face_list();
		const UMMesh::Vec3dList& normal_list = src_mesh->normal_list();
		
		for (size_t i = 0, i_size = face_list.size(); i < i_size; ++i) {
			const UMVec3i& face = face_list.at(i);
			for (int k = 0; k < 3; ++k)
			{
				const UMVec3d& normal = normal_list.at(face[k]);
				dx_normal[i * 3 + k].x = static_cast<float>(normal.x);
				dx_normal[i * 3 + k].y = static_cast<float>(normal.y);
				dx_normal[i * 3 + k].z = static_cast<float>(normal.z);
			}
		}
		device_context->Unmap(dst_mesh->normal_buffer_pointer(), 0);
	}
	
	/** 
	 * load uv from burger to dx11
	 */
	void load_uv(
		ID3D11DeviceContext* device_context,
		UMDirectX11MeshPtr dst_mesh, 
		UMMeshPtr src_mesh)
	{
		if (!device_context) return;
		D3D11_MAPPED_SUBRESOURCE resource;
		device_context->Map(dst_mesh->uv_buffer_pointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		UMVec2f* dx_uv = (UMVec2f*) resource.pData;
		const UMMesh::Vec2dList& uv_list = src_mesh->uv_list();
		
		for (size_t i = 0, i_size = uv_list.size(); i < i_size; ++i) {
			const UMVec2d& uv = uv_list.at(i);
			dx_uv[i].x = static_cast<float>(uv.x);
			dx_uv[i].y = static_cast<float>(uv.y);
		}
		device_context->Unmap(dst_mesh->uv_buffer_pointer(), 0);
	}

	//----------------------------------------------------------------------------

	/**
	 * load vertex index from umio to burger
	 */
	void load_vertex_index(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.vertex_index_list().size());
		mesh->mutable_face_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::IntList& index = ummesh.vertex_index_list().at(i);
			UMVec3i umface( index.at(0), index.at(1), index.at(2) );
			mesh->mutable_face_list().at(i) = umface;
		}
	}

	/**
	 * load vertex from umio to burger
	 */
	void load_vertex(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.vertex_list().size());
		mesh->mutable_vertex_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::DoubleList& vertex = ummesh.vertex_list().at(i);
			UMVec3d umvertex( vertex.at(0), vertex.at(1), vertex.at(2) );
			mesh->mutable_vertex_list().at(i) = umvertex * import_scale_for_debug;
		}
	}
	
	/**
	 * load normal from umio to burger
	 */
	void load_normal(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.normal_list().size());
		mesh->mutable_normal_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::DoubleList& normal = ummesh.normal_list().at(i);
			UMVec3d umnormal( normal.at(0), normal.at(1), normal.at(2) );
			mesh->mutable_normal_list().at(i) = umnormal;
		}
	}
	
	/**
	 * load uv  from umio to burger
	 */
	void load_uv(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.uv_list().size());
		mesh->mutable_uv_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::DoubleList& uv = ummesh.uv_list().at(i);
			UMVec2d umuv( uv.at(0), 1.0 - uv.at(1) );
			mesh->mutable_uv_list().at(i) = umuv;
		}
	}
	
	/**
	 * load material from umio to burger
	 */
	void load_material(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.material_list().size());
		mesh->mutable_material_list().resize(size);

		for (int i = 0; i < size; ++i)
		{
			const umio::UMMaterial& material = ummesh.material_list().at(i);
			UMMaterialPtr ummaterial = std::make_shared<UMMaterial>();
			ummaterial->set_ambient(to_um_material_value(material.ambient()));
			ummaterial->set_diffuse(to_um_material_value(material.diffuse()));
			ummaterial->set_specular(to_um_material_value(material.specular()));
			ummaterial->set_emissive(to_um_material_value(material.emissive()));
			ummaterial->set_refrection(to_um_material_value(material.refrection()));
			ummaterial->set_transparent(to_um_material_value(material.transparent()));
			ummaterial->set_shininess(material.shininess());
			ummaterial->set_transparency_factor(material.transparency_factor());
			ummaterial->set_reflection_factor(material.reflection_factor());
			ummaterial->set_diffuse_factor(material.diffuse_factor());
			ummaterial->set_specular_factor(material.specular_factor());
			ummaterial->set_emissive_factor(material.emissive_factor());
			ummaterial->set_ambient_factor(material.ambient_factor());
			
			// load texture through dx11
			const int texture_count = static_cast<int>(material.texture_list().size());
			for (int k = 0; k < texture_count; ++k)
			{
				const umio::UMTexture& texture = material.texture_list().at(k);
				const std::string& absolute_or_relative_file_name = texture.file_name();
				std::u16string str = UMStringUtil::utf8_to_utf16(absolute_or_relative_file_name);
				std::u16string file_name = UMPath::get_file_name(str);
				std::u16string path = UMPath::resource_absolute_path(file_name);
				ummaterial->mutable_texture_path_list().push_back(path);
			}

			const int polygon_count = static_cast<int>(std::count(
				ummesh.material_index_list().begin(), 
				ummesh.material_index_list().end(),
				i));

			ummaterial->set_polygon_count(polygon_count);
			mesh->mutable_material_list().at(i) = ummaterial;
		}

		// default material
		if (size == 0)
		{
			mesh->mutable_material_list().resize(1);
			UMMaterialPtr ummaterial = UMMaterial::default_material();
			const int polygon_count = static_cast<int>(ummesh.vertex_index_list().size());
			ummaterial->set_polygon_count(polygon_count);
			mesh->mutable_material_list().at(0) = ummaterial;
		}
	}

	//----------------------------------------------------------------------------
	
	typedef std::pair<int,int> IndexPair;

	/**
	 * sort umio mesh elements by material
	 */
	void sort_by_material(umio::UMMesh& src_mesh)
	{
		// sort material index with index
		const int index_size = static_cast<int>(src_mesh.material_index_list().size());
		std::vector< IndexPair > index_pair_list;
		index_pair_list.resize(index_size);
		for (int i = 0; i < index_size; ++i)
		{
			index_pair_list[i] = IndexPair(src_mesh.material_index_list().at(i), i);
		}
		std::sort(index_pair_list.begin(), index_pair_list.end());
		
		// store vertex index and material index by material order
		umio::IntListVec sorted_vertex_index;
		umio::IntList sorted_material_index;
		umio::DoubleListVec sorted_uv;
		sorted_vertex_index.resize(index_size);
		sorted_material_index.resize(index_size);
		if (!src_mesh.uv_list().empty())
		{
			sorted_uv.resize(index_size * 3);
		}

		for (int i = 0; i < index_size; ++i)
		{
			IndexPair& pair = index_pair_list[i];
			sorted_material_index[i] = pair.first;
			sorted_vertex_index[i] = src_mesh.vertex_index_list().at(pair.second);
			if (!src_mesh.uv_list().empty())
			{
				for (int k = 0; k < 3; ++k)
				{
					sorted_uv[i * 3 + k] = src_mesh.uv_list().at(pair.second * 3 + k);
				}
			}
		}
		src_mesh.mutable_vertex_index_list().swap(sorted_vertex_index);
		src_mesh.mutable_material_index().swap(sorted_material_index);
		if (!src_mesh.uv_list().empty())
		{
			src_mesh.mutable_uv_list().swap(sorted_uv);
		}
	}

	/**
	 * convert vertices to triangle list
	 */
	void convert_to_triangle_list(umio::UMMesh& src_mesh)
	{
		const int vertex_index_size = static_cast<int>(src_mesh.vertex_index_list().size());
		umio::DoubleListVec triangle_list;
		triangle_list.resize(vertex_index_size * 3);
		
		umio::DoubleListVec normal_list;
		normal_list.resize(vertex_index_size * 3);
		
		for (int i = 0; i < vertex_index_size; ++i)
		{
			const umio::IntList& vertex_index = src_mesh.vertex_index_list().at(i);
			for (int k = 0; k < 3; ++k)
			{
				triangle_list[i * 3 + k] = src_mesh.vertex_list().at(vertex_index[k]);
				normal_list[i * 3 + k] = src_mesh.normal_list().at(vertex_index[k]);
			}
		}
		src_mesh.mutable_vertex_list().swap(triangle_list);
		src_mesh.mutable_normal_list().swap(normal_list);
	}

} // anonymouse namespace


namespace burger
{

/** 
 * import burger mesh list
 */
bool UMModelIO::import_mesh_list(UMMeshList& dst, const umio::UMObjectPtr src)
{
	if (!src) return false;

	bool result = false;
	umio::UMMesh::IDToMeshMap::iterator it = src->mutable_mesh_map().begin();
	for (; it != src->mutable_mesh_map().end(); ++it)
	{
		umio::UMMesh& ummesh = (*it).second;

		UMMeshPtr mesh(std::make_shared<UMMesh>());
		dst.push_back(mesh);
		result = true;
		
		sort_by_material(ummesh);
		load_material(mesh, ummesh);
		load_vertex_index(mesh, ummesh);
		load_vertex(mesh, ummesh);
		load_normal(mesh, ummesh);
		load_uv(mesh, ummesh);
		mesh->update_box();
	}
	return result;
}

/**
 * convert burger mesh to directx11 mesh
 */
UMDirectX11MeshPtr UMModelIO::convert_mesh_to_dx11_mesh(
	ID3D11Device *device_pointer,
	UMMeshPtr src)
{
	if (!device_pointer) return UMDirectX11MeshPtr();
	if (!src) return UMDirectX11MeshPtr();
	
	ID3D11DeviceContext* device_context(NULL);
	device_pointer->GetImmediateContext(&device_context);

	UMDirectX11MeshPtr mesh(std::make_shared<UMDirectX11Mesh>());
	
	// create index buffer
	if (src->face_list().size() > 0) {
		size_t size = src->face_list().size();

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth      = static_cast<UINT>(sizeof(UMVec3i) * size);
		desc.Usage          = D3D11_USAGE_DYNAMIC;
		desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device_pointer->CreateBuffer(&desc, NULL, mesh->p_index_buffer_pointer());
		if (!mesh->index_buffer_pointer()) {
			SAFE_RELEASE(device_context);
			return UMDirectX11MeshPtr();
		} else {
			load_vertex_index(device_context, mesh, src);
		}
	}

	// create vertex buffer
	if (src->vertex_list().size() > 0) {
		// create full triangle verts for uv
		size_t size = src->face_list().size() * 3;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth      = static_cast<UINT>(sizeof(UMVec3f) * size);
		desc.Usage          = D3D11_USAGE_DYNAMIC;
		desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device_pointer->CreateBuffer(&desc, NULL, mesh->p_vertex_buffer_pointer());	
		if (!mesh->vertex_buffer_pointer()) {
			SAFE_RELEASE(device_context);
			return UMDirectX11MeshPtr();
		} else {
			load_vertex(device_context, mesh, src);
		}
	}

	// create normal buffer
	if (src->normal_list().size() > 0) {
		// create full triangle normals for uv
		size_t size = src->face_list().size() * 3;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth      = static_cast<UINT>(sizeof(UMVec3f) * size);
		desc.Usage          = D3D11_USAGE_DYNAMIC;
		desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device_pointer->CreateBuffer(&desc, 0, mesh->p_normal_buffer_pointer());	
		if (!mesh->normal_buffer_pointer()) {
			SAFE_RELEASE(device_context);
			return UMDirectX11MeshPtr();
		} else {
			load_normal(device_context, mesh, src);
		}
	}
		
	// create uv buffer
	if (src->uv_list().size() > 0) {
		size_t size = src->uv_list().size();

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth      = static_cast<UINT>(sizeof(UMVec2f) * size);
		desc.Usage          = D3D11_USAGE_DYNAMIC;
		desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device_pointer->CreateBuffer(&desc, NULL, mesh->p_uv_buffer_pointer());
		if (!mesh->uv_buffer_pointer()) {
			SAFE_RELEASE(device_context);
			return UMDirectX11MeshPtr();
		} else {
			load_uv(device_context, mesh, src);
		}
	}

	// convert material
	if (src->material_list().size() > 0) {
		const int material_size = static_cast<int>(src->material_list().size());
		for (int i = 0; i < material_size; ++i)
		{
			UMMaterialPtr material = src->material_list().at(i);
			UMDirectX11MaterialPtr dx_material = UMModelIO::convert_material_to_dx11_material(device_pointer, material);
			if (dx_material)
			{
				mesh->mutable_material_list().push_back(dx_material);
			}
		}
	}

	mesh->set_ummesh(src);
		
	SAFE_RELEASE(device_context);
	return mesh;
}

/**
 * convert burger mesh group to directx11 mesh
 */
UMDirectX11MeshGroupPtr UMModelIO::convert_mesh_group_to_dx11_mesh_group(
	ID3D11Device *device_pointer,
	UMMeshGroupPtr src)
{
	if (!device_pointer) return UMDirectX11MeshGroupPtr();
	if (!src) return UMDirectX11MeshGroupPtr();
	
	UMDirectX11MeshGroupPtr group(std::make_shared<UMDirectX11MeshGroup>());
	group->init(device_pointer);
	group->set_ummesh_group(src);
	UMMeshList::const_iterator it = src->mesh_list().begin();
	for (; it != src->mesh_list().end(); ++it)
	{
		UMMeshPtr ummesh = *it;
		UMDirectX11MeshPtr mesh = UMModelIO::convert_mesh_to_dx11_mesh(device_pointer, ummesh);
		if (!mesh) return  UMDirectX11MeshGroupPtr();

		mesh->set_ummesh(ummesh);
		group->mutable_dx11_mesh_list().push_back(mesh);
	}
	return group;
}

/**
 * convert burger light to directx11 light
 */
UMDirectX11LightPtr UMModelIO::convert_light_to_dx11_light(
	ID3D11Device *device_pointer,
	UMLightPtr src)
{
	if (!device_pointer) return UMDirectX11LightPtr();
	if (!src) return UMDirectX11LightPtr();

	UMDirectX11LightPtr light(std::make_shared<UMDirectX11Light>(src));
	light->init(device_pointer);
	return light;
}

/**
 * convert burger material to directx11 material
 */
UMDirectX11MaterialPtr UMModelIO::convert_material_to_dx11_material(
	ID3D11Device *device_pointer,
	UMMaterialPtr src)
{
	if (!device_pointer) return UMDirectX11MaterialPtr();
	if (!src) return UMDirectX11MaterialPtr();

	UMDirectX11MaterialPtr material(std::make_shared<UMDirectX11Material>(src));
	material->init(device_pointer);
	return material;
}

/**
 * convert burger camera to directx11 camera
 */
UMDirectX11CameraPtr UMModelIO::convert_camera_to_dx11_camera(
	ID3D11Device *device_pointer,
	UMCameraPtr src)
{
	if (!device_pointer) return UMDirectX11CameraPtr();
	if (!src) return UMDirectX11CameraPtr();

	UMDirectX11CameraPtr camera(std::make_shared<UMDirectX11Camera>(src));
	camera->init(device_pointer);
	return camera;
}

} // burger

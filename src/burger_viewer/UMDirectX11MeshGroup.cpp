/**
 * @file UMDirectX11MeshGroup.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11MeshGroup.h"

#include "UMStringUtil.h"
#include "UMTime.h"

namespace burger
{
	
/**
 * init meseh group
 */
bool UMDirectX11MeshGroup::init(ID3D11Device *device_pointer)
{
	if (!device_pointer) return false;
	
	// init shader manager
	shader_manager_->init(device_pointer, UMDirectX11ShaderManager::eModel);

	return true;
}

/**
 * refresh mesh group
 */
void UMDirectX11MeshGroup::refresh(ID3D11Device* device_pointer)
{
	if (!device_pointer) return;
	
	ID3D11DeviceContext *device_context_pointer = NULL;
	device_pointer->GetImmediateContext(&device_context_pointer);
		
	UINT stride = sizeof(UMVec3f);

	UMDirectX11MeshList::iterator it = dx11_mesh_list_.begin();
	for (; it != dx11_mesh_list_.end(); ++it)
	{
		UMDirectX11MeshPtr mesh = *it;
		if (!mesh->ummesh()) continue;
		
		UINT vertex_offset = 0;
		UINT index_offset = 0;

		UMDirectX11MaterialList::const_iterator mt = mesh->material_list().begin();
		for (; mt != mesh->material_list().end(); ++mt)
		{
			UMDirectX11MaterialPtr material = *mt;
		
			device_context_pointer->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			device_context_pointer->IASetVertexBuffers(0, 1, mesh->p_vertex_buffer_pointer(), &stride, &vertex_offset);
			device_context_pointer->IASetVertexBuffers(1, 1, mesh->p_normal_buffer_pointer(), &stride, &vertex_offset);
			device_context_pointer->IASetIndexBuffer(mesh->index_buffer_pointer(), DXGI_FORMAT_R32_UINT, index_offset);

			index_offset += material->polygon_count() * sizeof(UINT32);
			vertex_offset += material->polygon_count() * stride;
		
			UINT index_count = material->polygon_count() * 3;
	
			// apply constant buffer
			const UMDirectX11ShaderManager::BufferPointerList& constant_buffer = shader_manager_->constant_buffer_list();
			if (!constant_buffer.empty())
			{
				// put material to constant buffer
				{
					ID3D11Buffer* constant = constant_buffer[0];
					device_context_pointer->UpdateSubresource(constant, 0, NULL, &(*material), 0, 0);
					device_context_pointer->VSSetConstantBuffers( 2, 1, &constant );
					device_context_pointer->PSSetConstantBuffers( 2, 1, &constant );
				}
			}

			// shader list
			const UMDirectX11ShaderManager::ShaderList& shaders = shader_manager_->shader_list();

			// apply vertex shader
			if (device_context_pointer && !shaders.empty())
			{
				// apply input layout
				if (shader_manager_->input_layout_pointer())
				{
					device_context_pointer->IASetInputLayout(shader_manager_->input_layout_pointer());
				}

				// apply vertex shader
				device_context_pointer->VSSetShader( shaders[0]->vertex_shader_pointer(), NULL, 0 );
		
				// apply hull shader
				device_context_pointer->HSSetShader( NULL, NULL, 0 );

				// apply domain shader
				device_context_pointer->DSSetShader( NULL, NULL, 0 );

				// apply geometry shader
				device_context_pointer->GSSetShader( NULL, NULL, 0 );

				// apply pixel shader
				device_context_pointer->PSSetShader( shaders[1]->pixel_shader_pointer(), NULL, 0 );

				// apply compute shader
				device_context_pointer->CSSetShader( NULL, NULL, 0 );

				// draw
				device_context_pointer->DrawIndexed(index_count, 0, 0 );
			}
			}
	}

	SAFE_RELEASE(device_context_pointer);
}

} // burger
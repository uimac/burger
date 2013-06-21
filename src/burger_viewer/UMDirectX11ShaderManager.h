/**
 * @file UMDirectX11Shader.h
 */
#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <vector>
#include <memory>
#include "UMMacro.h"
#include "UMDirectX11Shader.h"

namespace burger
{

class UMDirectX11ShaderManager;
typedef std::shared_ptr<UMDirectX11ShaderManager> UMDirectX11ShaderManagerPtr;

/**
 * shader manager
 */
class UMDirectX11ShaderManager 
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11ShaderManager);
public:
	typedef std::vector<UMDirectX11ShaderPtr> ShaderList;
	typedef std::vector<ID3D11Buffer*> BufferPointerList;

	/**
	 * @param [in] device_pointer directx11 device
	 */
	UMDirectX11ShaderManager(ID3D11Device *device_pointer) ;

	~UMDirectX11ShaderManager() {
		for (size_t i = 0, size = constant_buffer_list_.size(); i < size; ++i) {
			if (constant_buffer_list_[i]) { 
				constant_buffer_list_[i]->Release();
			}
		}
		constant_buffer_list_.clear();
	}
	
	/**
	 * get feature level
	 */
	D3D_FEATURE_LEVEL feature_level() const { return feature_level_; }
	
	/**
	 * get constant buffer list
	 */
	const BufferPointerList& constant_buffer_list() const { return constant_buffer_list_; }
	
	/**
	 * get shader list
	 */
	const ShaderList& shader_list() const { return shader_list_; }

	/**
	 * get shader list
	 */
	ShaderList& mutable_shader_list() { return shader_list_; }

private:
	D3D_FEATURE_LEVEL feature_level_;
	ShaderList shader_list_;
	BufferPointerList constant_buffer_list_;
};

} // burger

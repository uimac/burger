
#include "UMDirectX11ShaderManager.h"
#include <d3dcompiler.h>
#include <tchar.h>
#include "UMVector.h"
#include <shlwapi.h>

namespace burger
{

UMDirectX11ShaderManager::UMDirectX11ShaderManager(ID3D11Device *device_pointer)
{
	feature_level_ = device_pointer->GetFeatureLevel();
	
	// create contant buffers
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags      = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.ByteWidth      = sizeof(UMVec4f) * 4;
	
	ID3D11Buffer *buffer = NULL;

	HRESULT hr = device_pointer->CreateBuffer(&buffer_desc, NULL, &buffer);
	if (FAILED(hr)) {

	}
	constant_buffer_list_.push_back(buffer);

	feature_level_ = device_pointer->GetFeatureLevel();

	// change current directory for test 
	TCHAR path[1024];
	GetModuleFileName(NULL, path, sizeof(path) / sizeof(TCHAR));
	PathRemoveFileSpec(path);
	SetCurrentDirectory(path);
	SetCurrentDirectory(_T("../../resource/"));
	GetCurrentDirectory(1024, path);

	// vertex shader
	{
		UMDirectX11ShaderPtr shader(std::make_shared<UMDirectX11Shader>());

		std::wstring vs_path = path + std::wstring(_T("\\default_vs.hlsl"));

		shader->create_shader_from_file(
			device_pointer,
			vs_path.c_str(), 
			"VS_Main", 
			UMDirectX11Shader::vs);
		
		mutable_shader_list().push_back(shader);
	}

	// pixel shader
	{
		UMDirectX11ShaderPtr shader(std::make_shared<UMDirectX11Shader>());

		std::wstring ps_path = path + std::wstring(_T("\\default_ps.hlsl"));

		shader->create_shader_from_file(
			device_pointer,
			ps_path.c_str(), 
			"PS_Main", 
			UMDirectX11Shader::ps);

		mutable_shader_list().push_back(shader);
	}
}

} // burger
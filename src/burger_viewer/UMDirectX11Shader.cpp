/**
 * @file UMDirectX11Shader.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMDirectX11Shader.h"
#include <dxgi.h>
#include <d3d11.h>
#include <string>
#include <d3dcompiler.h>

namespace
{

	// shader version strs
	// VS
	const std::string vs_5_0("vs_5_0");
	const std::string vs_4_1("vs_4_1");
	const std::string vs_4_0("vs_4_0");
	const std::string vs_4_0_9_3("vs_4_0_level_9_3");
	const std::string vs_4_0_9_1("vs_4_0_level_9_1");
	// PS
	const std::string ps_5_0("ps_5_0");
	const std::string ps_4_1("ps_4_1");
	const std::string ps_4_0("ps_4_0");
	const std::string ps_4_0_9_3("ps_4_0_level_9_3");
	const std::string ps_4_0_9_1("ps_4_0_level_9_1");
	// CS
	const std::string cs_5_0("cs_5_0");
	const std::string cs_4_1("cs_4_1");
	const std::string cs_4_0("cs_4_0");
	// GS
	const std::string gs_5_0("gs_5_0");
	const std::string gs_4_1("gs_4_1");
	const std::string gs_4_0("gs_4_0");
	// DS
	const std::string ds_5_0("ds_5_0");
	// HS
	const std::string hs_5_0("hs_5_0");
	// none
	const std::string shader_none("");

} // anonymouse namespace

namespace burger
{

/// constructor
UMDirectX11Shader::UMDirectX11Shader()
	: feature_level_(D3D_FEATURE_LEVEL_9_1),
	input_layout_pointer_(NULL),
	blob_pointer_(NULL),
	vertex_shader_pointer_(NULL),
	pixel_shader_pointer_(NULL),
	compute_shader_pointer_(NULL),
	domain_shader_pointer_(NULL),
	hull_shader_pointer_(NULL)
	{}

/// destructor
UMDirectX11Shader::~UMDirectX11Shader()
	{
	if (input_layout_pointer_)
	{
		input_layout_pointer_->Release();
		input_layout_pointer_ = NULL;
	}
	if (blob_pointer_)
	{
		blob_pointer_->Release();
		blob_pointer_ = NULL;
	}
	if (vertex_shader_pointer_)
	{
		vertex_shader_pointer_->Release();
		vertex_shader_pointer_ = NULL;
	}
	if (pixel_shader_pointer_)
	{
		pixel_shader_pointer_->Release();
		pixel_shader_pointer_ = NULL;
	}
	if (compute_shader_pointer_)
	{
		compute_shader_pointer_->Release();
		compute_shader_pointer_ = NULL;
	}
	if (domain_shader_pointer_)
	{
		domain_shader_pointer_->Release();
		domain_shader_pointer_ = NULL;
	}
	if (hull_shader_pointer_)
	{
		hull_shader_pointer_->Release();
		hull_shader_pointer_ = NULL;
	}
}

/**
 * get valid shader version
 */
const std::string& UMDirectX11Shader::get_valid_shader_version(UMDirectX11Shader::ShaderType type) const
{
	D3D_FEATURE_LEVEL level = feature_level_;
	if (level == D3D_FEATURE_LEVEL_11_0)
	{
		if (type == UMDirectX11Shader::vs) { return vs_5_0; }
		if (type == UMDirectX11Shader::ps) { return ps_5_0; }
		if (type == UMDirectX11Shader::cs) { return cs_5_0; }
		if (type == UMDirectX11Shader::gs) { return gs_5_0; }
		if (type == UMDirectX11Shader::ds) { return ds_5_0; }
		if (type == UMDirectX11Shader::hs) { return hs_5_0; }
	}
	if (level == D3D_FEATURE_LEVEL_10_1)
	{
		if (type == UMDirectX11Shader::vs) { return vs_4_1; }
		if (type == UMDirectX11Shader::ps) { return ps_4_1; }
		if (type == UMDirectX11Shader::cs) { return cs_4_1; }
		if (type == UMDirectX11Shader::gs) { return gs_4_1; }
	}
	if (level == D3D_FEATURE_LEVEL_10_0)
	{
		if (type == UMDirectX11Shader::vs) { return vs_4_0; }
		if (type == UMDirectX11Shader::ps) { return ps_4_0; }
		if (type == UMDirectX11Shader::cs) { return cs_4_0; }
		if (type == UMDirectX11Shader::gs) { return gs_4_0; }
	}
	if (level == D3D_FEATURE_LEVEL_9_3)
	{
		if (type == UMDirectX11Shader::vs) { return vs_4_0_9_3; }
		if (type == UMDirectX11Shader::ps) { return ps_4_0_9_3; }
	}
	if (level == D3D_FEATURE_LEVEL_9_2)
	{
		if (type == UMDirectX11Shader::vs) { return vs_4_0_9_1; }
		if (type == UMDirectX11Shader::ps) { return ps_4_0_9_1; }
	}
	if (level == D3D_FEATURE_LEVEL_9_1)
	{
		if (type == UMDirectX11Shader::vs) { return vs_4_0_9_1; }
		if (type == UMDirectX11Shader::ps) { return ps_4_0_9_1; }
	}
	return shader_none; //defeult empty
}

/**
 * create shader from string
 */
bool UMDirectX11Shader::create_shader_from_string(
	ID3D11Device *device_pointer,
	const std::string& shader_str, 
	const std::string& entry_point_str,
	const ShaderType type)
{
	if (!device_pointer) return false;

	feature_level_ = device_pointer->GetFeatureLevel();

	ID3DBlob* error_blob(NULL);
	
	HRESULT hr = D3DCompile(
		shader_str.c_str(), 
		shader_str.size(), 
		NULL, // shader filename
		NULL, // macro
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // include
		// entry point function name
		entry_point_str.c_str(),
		// target 
		get_valid_shader_version(type).c_str(),
		// shader compile option
		D3D10_SHADER_OPTIMIZATION_LEVEL1,
		// effect compile option
		0,
		(LPD3DBLOB*)&blob_pointer_,
		(LPD3DBLOB*)&error_blob);

	if ( FAILED(hr) )
	{
		return false;
	}

	if (error_blob) {
		error_blob->Release();
		error_blob = NULL;
	}

	return true;
}

/**
 * create shader from file
 */
bool UMDirectX11Shader::create_shader_from_file(
	ID3D11Device *device_pointer,
	const std::wstring& file_path, 
	const std::string& entry_point_str,
	const ShaderType type)
{
	if (!device_pointer) return false;
	
	feature_level_ = device_pointer->GetFeatureLevel();

	ID3DBlob* error_blob(NULL);

	HRESULT hr = D3DCompileFromFile(
		file_path.c_str(), 
		NULL, 
		//include
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		// entry point function name
		entry_point_str.c_str(),
		// target 
		get_valid_shader_version(type).c_str(),
		// flag1: shader compile option
		D3D10_SHADER_OPTIMIZATION_LEVEL1,
		// flag2:effect compile option
		0,
		(LPD3DBLOB*)&blob_pointer_,
		(LPD3DBLOB*)&error_blob);

	if ( FAILED(hr) )
	{
		return false;
	}

	if (error_blob) {
		error_blob->Release();
		error_blob = NULL;
	}

	ID3DBlob* blob = blob_pointer_;

	if (type == UMDirectX11Shader::vs) {
		hr = device_pointer->CreateVertexShader(
			blob->GetBufferPointer(), 
			blob->GetBufferSize(), NULL, &vertex_shader_pointer_);
		if (FAILED(hr)) {
			return false;
		}
	} else if (type == UMDirectX11Shader::ps) {
		hr = device_pointer->CreatePixelShader(
			blob->GetBufferPointer(), 
			blob->GetBufferSize(), NULL, &pixel_shader_pointer_);
		if (FAILED(hr)) {
			return false;
		}
	} else if (type == UMDirectX11Shader::cs) {
		hr = device_pointer->CreateComputeShader(
			blob->GetBufferPointer(), 
			blob->GetBufferSize(), NULL, &compute_shader_pointer_);
		if (FAILED(hr)) {
			return false;
		}
	} else if (type == UMDirectX11Shader::ds) {
		hr = device_pointer->CreateDomainShader(
			blob->GetBufferPointer(), 
			blob->GetBufferSize(), NULL, &domain_shader_pointer_);
		if (FAILED(hr)) {
			return false;
		}
	} else if (type == UMDirectX11Shader::hs) {
		hr = device_pointer->CreateHullShader(
			blob->GetBufferPointer(), 
			blob->GetBufferSize(), NULL, &hull_shader_pointer_);
		if (FAILED(hr)) {
			return false;
		}
	}
	return true;
}

/**
 * create shader input layout
 */
ID3D11InputLayout* UMDirectX11Shader::create_input_layout(ID3D11Device *device_pointer)
{
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if FAILED(device_pointer->CreateInputLayout(
		layout, 
		_countof(layout), 
		buffer_pointer(), 
		buffer_size(), 
		&input_layout_pointer_))
	{
		return NULL;
	}
	
	if (input_layout_pointer_)
	{
		// set layout to device context
		ID3D11DeviceContext* device_context(NULL);
		device_pointer->GetImmediateContext(&device_context);
		if (device_context)
		{
			device_context->IASetInputLayout(input_layout_pointer_);
		}
		device_context->Release();
	}

	return input_layout_pointer_;
}

} // burger
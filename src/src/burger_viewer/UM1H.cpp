/**
 * @file UM1H.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <windows.h>
#include <Mmsystem.h>

#include <string>
#include <tchar.h>
#include <shlwapi.h>

#include "UMStringUtil.h"
#include "UM1H.h"
#include "UMImage.h"
#include "UMTga.h"
#include "UMPath.h"

namespace burger
{
	
/// constructor
UM1H::UM1H()
{
	current_number_ = 1;
	initial_time_ = ::timeGetTime();
}

/// destructor
UM1H::~UM1H()
{

}

static void set_current_directory_to_out_directory()
{
	TCHAR path[1024];
	GetModuleFileName(NULL, path, sizeof(path) / sizeof(TCHAR));
	PathRemoveFileSpec(path);
	SetCurrentDirectory(path);
	SetCurrentDirectory(_T("./out/"));
}

bool UM1H::refresh(ID3D11Device* device_pointer, UMDirectX11Texture& texture)
{
	// milliseconds
	unsigned long time = ::timeGetTime() - initial_time_;

	unsigned long minutes = time / 60000;
	unsigned long seconds = (time - minutes * 60) / 1000;
	//unsigned long mills = time - seconds * 1000;

	if (minutes > current_number_) {
		ID3D11DeviceContext* device_context(NULL);
		device_pointer->GetImmediateContext(&device_context);

		UMImagePtr image = texture.convert_to_image(device_pointer, device_context);
		image = image->create_flip_image(false, true);

		UMTga tga;
		std::string number = UMStringUtil::number_to_sequence_string(current_number_, 4);
		std::string filename = number + ".tga";

		set_current_directory_to_out_directory();
		tga.save(filename, *image);

		++current_number_;
		SAFE_RELEASE(device_context);
	}

	if (minutes >= 50)
	{
		// todo end
		return false;
	}
	return true;
}

} // burger

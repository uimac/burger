/**
 * @file UMPath.cpp
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

#include "UMPath.h"
#include "UMStringUtil.h"

namespace burger
{

std::u16string UMPath::resource_absolute_path(const std::wstring& file_name)
{
	TCHAR path[1024];
	GetModuleFileName(NULL, path, sizeof(path) / sizeof(TCHAR));
	PathRemoveFileSpec(path);
	SetCurrentDirectory(path);
	SetCurrentDirectory(_T("../../../resource/"));
	GetCurrentDirectory(1024, path);
	std::wstring inpath = path + std::wstring(_T("\\")) + file_name;
	return UMStringUtil::wstring_to_utf16(inpath);
}

} // burger

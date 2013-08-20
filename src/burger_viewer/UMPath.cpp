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

std::u16string UMPath::resource_absolute_path(const std::u16string& file_name)
{
	TCHAR path[1024];
	GetModuleFileName(NULL, path, sizeof(path) / sizeof(TCHAR));
	PathRemoveFileSpec(path);
	SetCurrentDirectory(path);
	SetCurrentDirectory(_T("../../../resource/"));
	GetCurrentDirectory(1024, path);
	std::wstring inpath = path + std::wstring(_T("\\")) + UMStringUtil::utf16_to_wstring(file_name);
	return UMStringUtil::wstring_to_utf16(inpath);
}

std::u16string UMPath::get_file_name(const std::u16string& file_path)
{
	std::wstring path = UMStringUtil::utf16_to_wstring(file_path);
	std::wstring filename(::PathFindFileName(path.c_str()));
	return UMStringUtil::wstring_to_utf16(filename);
}

} // burger

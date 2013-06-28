/**
 * @file UMStringUtil.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <string>
#include <sstream>
#include "UMMacro.h"

namespace burger
{

/**
 * string utility
 */
template <typename T>
class UMStringUtil
{
	DISALLOW_COPY_AND_ASSIGN(UMStringUtil);

public:
	
	static std::wstring to_wstring(T value)
	{
		std::wstringstream converter;
		std::wstring  wstr;
		converter << value;
		converter >> wstr;
		return wstr;
	}

	static std::string to_string(T value)
	{
		std::stringstream converter;
		std::string  str;
		converter << value;
		converter >> str;
		return str;
	}
};

} // burger

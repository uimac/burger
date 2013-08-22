/**
 * @file UMPath.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <string>
#include "UMMacro.h"

namespace burger
{

/**
 * path utility
 */
class UMPath
{
	DISALLOW_COPY_AND_ASSIGN(UMPath);

public:
	static std::u16string resource_absolute_path(const std::u16string& file_name);

	static std::u16string get_file_name(const std::u16string& file_name);

	static void set_current_directory_to_module_path();
};

} // burger

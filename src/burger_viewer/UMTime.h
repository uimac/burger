/**
 * @file UMTime.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"

namespace burger
{

/**
 * print time on destructor
 */
class UMTime
{
	DISALLOW_COPY_AND_ASSIGN(UMTime);

public:

	/**
	 * constructor
	 * @param [in] print_time print time on destructor
	 */
	UMTime(const std::string& message);

	~UMTime();

private:
	bool print_time_;
	const std::string& message_;
	unsigned long initial_time_;
};

} // burger

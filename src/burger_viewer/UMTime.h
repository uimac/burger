/**
 * @file UMTime.h
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

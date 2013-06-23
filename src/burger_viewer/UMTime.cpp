/**
 * @file UMTime.cpp
 */
#include <windows.h>
#include <Mmsystem.h>

#include <string>

#include "UMTime.h"
#include "UMStringUtil.h"

namespace burger
{
	
/// constructor
UMTime::UMTime(const std::string& message)
	: message_(message)
{
	initial_time_ = ::timeGetTime();
}

/// destructor
UMTime::~UMTime()
{
	// milliseconds
	unsigned long time = ::timeGetTime() - initial_time_;

	unsigned long seconds = time / 1000;
	unsigned long mills = time - seconds * 1000;
	std::string message(
		message_ 
		+ ": " 
		+ UMStringUtil<unsigned long>::to_string(seconds)
		+ "s "
		+ UMStringUtil<unsigned long>::to_string(mills)
		+ "ms"
		);

	::OutputDebugStringA(message.c_str());
}

} // burger

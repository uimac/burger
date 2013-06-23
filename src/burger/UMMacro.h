/**
 * @file UMMacro.h
 * global macros
 */
#pragma once

namespace burger
{

/**
 * Restrict copy constructor
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	private: TypeName(const TypeName&); \
	void operator=(const TypeName&)

/**
 * for directx com release
 */
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

} // burger

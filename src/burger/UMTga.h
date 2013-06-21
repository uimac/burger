/**
 * @file UMTga.h
 * TGA IO
 */
#pragma once

#include "UMMacro.h"
#include "UMImage.h"

namespace burger
{

/**
 * TGA IO
 */
class UMTga
{
	DISALLOW_COPY_AND_ASSIGN(UMTga);
public:
	UMTga() {}
	~UMTga() {}
	
	/**
	 * save tga to a file
	 * @param [in] path save path
	 * @param [in] image output image
	 */
	bool save(const std::string& path, const UMImage& image) const;
};

} // burger

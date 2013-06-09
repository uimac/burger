/// UMTga.h ///
#pragma once

#include "UMImage.h"

namespace burger
{

/**
 * rendering parameters
 */
class UMTga
{
public:
	UMTga() {}
	~UMTga() {}

	bool save(const std::string& path, const UMImage& image) const;
};

} // burger

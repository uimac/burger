/**
 * @file UMTga.cpp
 * TGA IO
 */
#pragma once

#include "UMTga.h"
#include "UMImage.h"
#include "UMVector.h"

#include <iostream>
#include <fstream>

namespace burger
{

/**
 * save tga
 */
bool UMTga::save(const std::string& path, const UMImage& image) const
{
	if (image.buffer().empty()) return false;

	try
	{
		std::ofstream ofs(path.c_str(), std::ios::out|std::ios::binary|std::ios::trunc);
		if (!ofs) return false;
		
		// header
		unsigned char header[18] = { 
			0, // id field
			0, // colormap
			2, // format
			0, 0, // colormap entry
			0, // colormap entry size
			0, 0, // x
			0, 0, // y
			0, 0, // w
			0, 0, // h
			0, // bits
			0, // param
		};
		// w
		header[12] = static_cast<unsigned char>((image.width() & 0x00FF));
		header[13] = static_cast<unsigned char>((image.width() & 0xFF00) >> 8);
		// h
		header[14] = static_cast<unsigned char>((image.height() & 0x00FF));
		header[15] = static_cast<unsigned char>((image.height() & 0xFF00) >> 8);
		// bits
		header[16] = 24;
		ofs.write(reinterpret_cast<const char*>(header), 18);

		// floating image to 8bit rgb
		const double inv_gamma = 1.0 / 2.2;
		std::vector<unsigned char> img;
		img.resize(image.width() * image.height() * 3);
		for (int y = 0; y < image.height(); ++y)
		{
			for (int x = 0; x < image.width(); ++x)
			{
				int pos = image.width() * y + x;
				const UMVec3d col = image.buffer().at(pos);
				img[pos * 3 + 0] = static_cast<int>(pow(col.z, inv_gamma) * 0xFF + 0.5);
				img[pos * 3 + 1] = static_cast<int>(pow(col.y, inv_gamma) * 0xFF + 0.5);
				img[pos * 3 + 2] = static_cast<int>(pow(col.x, inv_gamma) * 0xFF + 0.5);
			}
		}
		ofs.write(reinterpret_cast<const char*>(&(*img.begin())), img.size());

		// footer
		unsigned char footer[26] = { 
			"\0\0\0\0\0\0\0\0"
			"TRUEVISION-TARGA"
			"\0" };
		ofs.write(reinterpret_cast<const char*>(footer), 26);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

}

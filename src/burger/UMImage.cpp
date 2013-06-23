/**
 * @file UMImage.cpp
 * Image
 */
#include "UMImage.h"

namespace burger
{

void UMImage::create_r8g8b8a8_buffer(UMImage::R8G8B8A8Buffer& img) const 
{
	const double inv_gamma = 1.0 / 2.2;
	img.resize(width() * height() * 4);
	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < width(); ++x)
		{
			int pos = width() * y + x;
			const UMVec4d col = buffer().at(pos);
			img[pos * 4 + 0] = static_cast<int>(pow(col.x, inv_gamma) * 0xFF + 0.5);
			img[pos * 4 + 1] = static_cast<int>(pow(col.y, inv_gamma) * 0xFF + 0.5);
			img[pos * 4 + 2] = static_cast<int>(pow(col.z, inv_gamma) * 0xFF + 0.5);
			img[pos * 4 + 3] = static_cast<int>(pow(col.w, inv_gamma) * 0xFF + 0.5);
		}
	}
}

void UMImage::create_b8g8r8_buffer(UMImage::B8G8R8Buffer& img) const 
{
	const double inv_gamma = 1.0 / 2.2;
	img.resize(width() * height() * 3);
	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < width(); ++x)
		{
			int pos = width() * y + x;
			const UMVec4d col = buffer().at(pos);
			img[pos * 3 + 0] = static_cast<int>(pow(col.z, inv_gamma) * 0xFF + 0.5);
			img[pos * 3 + 1] = static_cast<int>(pow(col.y, inv_gamma) * 0xFF + 0.5);
			img[pos * 3 + 2] = static_cast<int>(pow(col.x, inv_gamma) * 0xFF + 0.5);
		}
	}
}

} // burger

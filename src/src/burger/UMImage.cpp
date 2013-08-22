/**
 * @file UMImage.cpp
 * Image
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMImage.h"

namespace burger
{
	
/**
 * init image
 */
bool UMImage::init(int width, int height)
{
	width_ = width;
	height_ = height;
	buffer_.clear();
	buffer_.resize(width * height);
	return true;
}

/**
 * create r8g8b8a8 buffer
 */
void UMImage::create_r8g8b8a8_buffer(UMImage::R8G8B8A8Buffer& img) const 
{
	const double inv_gamma = 1.0 / 2.2;
	img.resize(width() * height() * 4);
	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < width(); ++x)
		{
			int pos = width() * y + x;
			const UMVec4d col = list().at(pos);
			img[pos * 4 + 0] = static_cast<int>(pow(col.x, inv_gamma) * 0xFF + 0.5);
			img[pos * 4 + 1] = static_cast<int>(pow(col.y, inv_gamma) * 0xFF + 0.5);
			img[pos * 4 + 2] = static_cast<int>(pow(col.z, inv_gamma) * 0xFF + 0.5);
			img[pos * 4 + 3] = static_cast<int>(pow(col.w, inv_gamma) * 0xFF + 0.5);
		}
	}
}

/**
 * create r8g8b8 buffer
 */
void UMImage::create_b8g8r8_buffer(UMImage::B8G8R8Buffer& img) const 
{
	const double inv_gamma = 1.0 / 2.2;
	img.resize(width() * height() * 3);
	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < width(); ++x)
		{
			int pos = width() * y + x;
			const UMVec4d col = list().at(pos);
			img[pos * 3 + 0] = static_cast<int>(pow(col.z, inv_gamma) * 0xFF + 0.5);
			img[pos * 3 + 1] = static_cast<int>(pow(col.y, inv_gamma) * 0xFF + 0.5);
			img[pos * 3 + 2] = static_cast<int>(pow(col.x, inv_gamma) * 0xFF + 0.5);
		}
	}
}

/**
 * clear image buffer
 */
void UMImage::clear()
{
	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < width(); ++x)
		{
			int pos = width() * y + x;
			mutable_list().at(pos) = UMVec4d(0);
		}
	}
}

/**
 * create flip image
 */
UMImagePtr UMImage::create_flip_image(bool horizon, bool vertical) const
{
	if (!is_valid()) return UMImagePtr();
	
	UMImagePtr dst(std::make_shared<UMImage>());
	dst->mutable_list().resize(list().size());
	dst->set_width(width());
	dst->set_height(height());

	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < width(); ++x)
		{
			int dstx = x;
			int dsty = y;
			if (horizon) { dstx = width() - x - 1; }
			if (vertical) { dsty = height() - y - 1; }
			
			int src_pos = width() * y + x;
			int dst_pos = width() * dsty + dstx;
			dst->mutable_list().at(dst_pos) = list().at(src_pos);
		}
	}
	return dst;
}

} // burger

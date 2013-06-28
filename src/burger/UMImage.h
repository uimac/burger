/**
 * @file UMImage.h
 * Image
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <vector>
#include "UMMacro.h"
#include "UMVector.h"

namespace burger
{

/**
 * Image
 */
class UMImage
{
	DISALLOW_COPY_AND_ASSIGN(UMImage);
public:
	typedef std::vector<UMVec4d> ImageBuffer;
	typedef std::vector<unsigned char> R8G8B8A8Buffer;
	typedef std::vector<unsigned char> B8G8R8Buffer;
	
	UMImage() : width_(0), height_(0) {}
	
	/**
	 * @param [in] width the pixel width
	 * @param [in] height the pixels height
	 */
	UMImage(int width, int height) : width_(width), height_(height) {}
	
	~UMImage() {}

	/**
	 * get image buffer
	 */
	const ImageBuffer& buffer() const { return buffer_; } 
	
	/**
	 * get image buffer
	 */
	ImageBuffer&  mutable_buffer() { return buffer_; }

	/**
	 * create r8g8b8a8 buffer
	 */
	void create_r8g8b8a8_buffer(R8G8B8A8Buffer& buffer) const;

	/**
	 * create r8g8b8 buffer
	 */
	void create_b8g8r8_buffer(B8G8R8Buffer& buffer) const;

	/**
	 * get width
	 */
	int width() const { return width_; }

	/**
	 * set width
	 * @param [in] width width
	 */
	void set_width(int width) { width_ = width; }

	/**
	 * get height
	 */
	int height() const { return height_; }
	
	/**
	 * set height
	 * @param [in] height height
	 */
	void set_height(int height) { height_ = height; }

	/**
	 * validate image
	 */
	bool is_validate() const { return (width_ * height_ == static_cast<int>(buffer_.size())); }

private:
	int width_;
	int height_;
	ImageBuffer buffer_;
};

} // burger

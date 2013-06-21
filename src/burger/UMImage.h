/**
 * @file UMImage.h
 * Image
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
	typedef std::vector<UMVec3d> ImageBuffer;
	
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
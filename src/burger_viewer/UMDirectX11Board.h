/**
 * @file UMDirectX11Board.h
 * a textured rectangle
 */
#pragma once

#include "UMMacro.h"
#include "UMVector.h"

namespace burger
{

/**
 * board is a textured rectangle
 */
class UMDirectX11Board
{
	DISALLOW_COPY_AND_ASSIGN(UMDirectX11Board);

public:
	
	/**
	 * create board on x-y plane
	 * @param [in] left_top left top position
	 * @param [in] right_bottom right bottom position
	 * @param [in] z z position
	 */
	UMDirectX11Board(
		UMVec2f left_top,
		UMVec2f right_bottom,
		float z);
			
private:
	UMVec3f p1_;
	UMVec2f uv1_;

	UMVec3f p2_;
	UMVec2f uv2_;

	UMVec3f p3_;
	UMVec2f uv3_;

	UMVec3f p4_;
	UMVec2f uv4_;
};

} // burger

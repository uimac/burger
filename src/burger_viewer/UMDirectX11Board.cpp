/**
 * @file UMDirectX11Board.cpp
 */
#include "UMDirectX11Board.h"

namespace burger
{
	
UMDirectX11Board::UMDirectX11Board(
	UMVec2f left_top,
	UMVec2f right_bottom,
	float z)
{
	// culling is CCW
	p1_ = UMVec3f(left_top.x, right_bottom.y, z);
	p2_ = UMVec3f(left_top.x, left_top.y, z);
	p3_ = UMVec3f(right_bottom.x, right_bottom.y, z);
	p4_ = UMVec3f(right_bottom.x, left_top.y, z);

	uv1_ = UMVec2f(0, 1.0f);
	uv2_ = UMVec2f(0, 0);
	uv3_ = UMVec2f(1.0f, 1.0f);
	uv4_ = UMVec2f(1.0f, 0);
}

} // burger

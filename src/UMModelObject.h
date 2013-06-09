/// UMBufferObject.h ///
#pragma once

#include "UMVector.h"

#include <vector>

namespace burger
{

class UMModelObject {
public:
	UMModelObject() {}
	virtual ~UMModelObject() {}

	typedef std::vector<UMVec4d> Buffer4;
	typedef std::vector<UMVec2d> Buffer2;
	typedef std::vector<int> IndexList;

	inline const Buffer4& face_buffer() const { return face_buffer_; }
	inline const Buffer4& vertex_buffer() const { return vertex_buffer_; }
	inline const Buffer4& normal_buffer() const { return normal_buffer_; }
	inline const Buffer4& vertex_color_buffer() const { return vertex_color_buffer_; }
	inline const Buffer2& uv_buffer() const { return uv_buffer_; }
	inline const IndexList& uv_index_buffer() const { return uv_index_buffer_; }

	inline Buffer4& mutable_face_buffer() { return face_buffer_; }
	inline Buffer4& mutable_vertex_buffer() { return vertex_buffer_; }
	inline Buffer4& mutable_normal_buffer() { return normal_buffer_; }
	inline Buffer4& mutable_vertex_color_buffer() { return vertex_color_buffer_; }
	inline Buffer2& mutable_uv_buffer() { return uv_buffer_; }
	inline IndexList& mutable_uv_index_buffer() { return uv_index_buffer_; }

private:
	Buffer4 face_buffer_;
	Buffer4 vertex_buffer_;
	Buffer4 normal_buffer_;
	Buffer4 vertex_color_buffer_;
	Buffer2 uv_buffer_;
	IndexList uv_index_buffer_;
	
};

} //burger
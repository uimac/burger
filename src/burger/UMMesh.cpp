/**
 * @file UMMesh.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMMesh.h"
#include "UMMacro.h"
#include "UMVector.h"
#include "UMTriangle.h"
#include "UMShaderParameter.h"
#include "UMRay.h"

#include <vector>

namespace
{
	using namespace burger;
	bool create_smooth_normals(UMMesh& mesh)
	{
		return false;
	}

	bool create_flat_normals(UMMesh& mesh)
	{
		return false;
	}
}

namespace burger
{
/** 
 * create normals
 * @param [in] is_smooth smooth or flat
 */
bool UMMesh::create_normals(bool is_smooth)
{
	if (face_list().empty()) return false;
	if (vertex_list().empty()) return false;
	
	const int face_size = static_cast<int>(face_list().size());
	const int vertex_size = static_cast<int>(vertex_list().size());
	mutable_normal_list().resize(vertex_size);
	
	for (int i = 0; i < vertex_size; ++i)
	{
		mutable_normal_list().at(i) = UMVec3d(0);
	}

	for (int i = 0; i < face_size; ++i)
	{
		UMVec3i face = face_list().at(i);
		const UMVec3d& v0 = vertex_list().at(face.x);
		const UMVec3d& v1 = vertex_list().at(face.y);
		const UMVec3d& v2 = vertex_list().at(face.z);
		UMVec3d normal = (v0-v1).cross(v1-v2);

		mutable_normal_list().at(face.x) += normal;
		mutable_normal_list().at(face.y) += normal;
		mutable_normal_list().at(face.z) += normal;
	}
	
	for (int i = 0; i < vertex_size; ++i)
	{
		mutable_normal_list().at(i) = normal_list().at(i).normalized();
	}
	return false;
}

/**
 * update AABB
 */
void UMMesh::update_box()
{
	box_.set_minimum(UMVec3d(std::numeric_limits<double>::infinity()));
	box_.set_maximum(UMVec3d(-std::numeric_limits<double>::infinity()));
	
	const int vertex_size = static_cast<int>(vertex_list().size());
	for (int i = 0; i < vertex_size; ++i)
	{
		box_.extend(vertex_list().at(i));
	}
}

/** 
 * get material from vertex index
 */
UMMaterialPtr UMMesh::material_from_face_index(int face_index) const
{
	int pos = 0;
	UMMaterialList::const_iterator it = material_list_.begin();
	for (; it != material_list_.end(); ++it)
	{
		const int polygon_count = (*it)->polygon_count();
		if (face_index >= pos && face_index < (pos+polygon_count)) {
			return *it;
		}
		pos += polygon_count;
	}
	return UMMaterialPtr();
}

/**
 * ray mesh intersection
 */
bool UMMesh::intersects(const UMRay& ray, UMShaderParameter& dst_parameter) const
{
	if (!box_.intersects(ray)) return false;
	double closest_distance = std::numeric_limits<double>::max();

	UMShaderParameter parameter;

	bool result = false;
	const int face_size = static_cast<int>(face_list().size());
	for (int i = 0; i < face_size; ++i)
	{
		const UMVec3i& face = face_list().at(i);
		const UMVec3d& v0 = vertex_list().at(face.x);
		const UMVec3d& v1 = vertex_list().at(face.y);
		const UMVec3d& v2 = vertex_list().at(face.z);
		if (UMTriangle::intersects(v0, v1, v2, ray, parameter))
		{
			if (parameter.distance < closest_distance)
			{
				result = true;
				closest_distance = parameter.distance;
				// normal
				const UMVec3d& n0 = normal_list().at(face.x);
				const UMVec3d& n1 = normal_list().at(face.y);
				const UMVec3d& n2 = normal_list().at(face.z);
				parameter.normal = (
					n0 * parameter.uvw.x + 
					n1 * parameter.uvw.y + 
					n2 * parameter.uvw.z).normalized();

				if (UMMaterialPtr material = material_from_face_index(i))
				{
					const UMVec4d& diffuse = material->diffuse();
					parameter.color.x = diffuse.x;
					parameter.color.y = diffuse.y;
					parameter.color.z = diffuse.z;
					if (!uv_list().empty() && !material->texture_list().empty()) {
						// uv
						const UMVec2d& uv0 = uv_list().at(i * 3 + 0);
						const UMVec2d& uv1 = uv_list().at(i * 3 + 1);
						const UMVec2d& uv2 = uv_list().at(i * 3 + 2);
						UMVec2d uv = UMVec2d(
							uv0 * parameter.uvw.x +
							uv1 * parameter.uvw.y +
							uv2 * parameter.uvw.z);
						uv.x = um_clip(uv.x);
						uv.y = um_clip(uv.y);
						const UMImagePtr texture = material->texture_list().at(0);
						const int x = static_cast<int>(texture->width() * uv.x);
						const int y = static_cast<int>(texture->height() * uv.y);
						const int pixel = y * texture->width() + x;
						const UMVec4d& pixel_color = texture->list().at(pixel);
						parameter.color.x *= pixel_color.x;
						parameter.color.y *= pixel_color.y;
						parameter.color.z *= pixel_color.z;
					}
				}
				dst_parameter = parameter;
			}
		}
	}
	return result;
}

	
/**
 * ray mesh intersection
 */
bool UMMesh::intersects(const UMRay& ray) const
{
	if (!box_.intersects(ray)) return false;
	const int face_size = static_cast<int>(face_list().size());
	for (int i = 0; i < face_size; ++i)
	{
		const UMVec3i& face = face_list().at(i);
		const UMVec3d& v0 = vertex_list().at(face.x);
		const UMVec3d& v1 = vertex_list().at(face.y);
		const UMVec3d& v2 = vertex_list().at(face.z);
		if (UMTriangle::intersects(v0, v1, v2, ray))
		{
			return true;
		}
	}
	return false;
}

} //burger
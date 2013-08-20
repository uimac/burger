/**
 * @file UMTriangle.h
 * a plane
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"
#include "UMPrimitive.h"
#include "UMVector.h"
#include "UMMesh.h"
#include "UMBox.h"

namespace burger
{

class UMTriangle;
typedef std::shared_ptr<UMTriangle> UMTrianglePtr;

class UMRay;
class UMShaderParameter;

/**
 * a triangle
 */
class UMTriangle : public UMPrimitive
{
	DISALLOW_COPY_AND_ASSIGN(UMTriangle);
public:
	UMTriangle() :
		vertex_index_(0),
		face_index_(0),
		normal_(0) {}

	/**
	 * @param [in] index center index
	 * @param [in] normal normal
	 */
	UMTriangle(UMMeshPtr mesh, const UMVec3i& vertex_index, int face_index) :
		mesh_(mesh),
		vertex_index_(vertex_index),
		face_index_(face_index),
		normal_(0)
	{
		 update_box();
	}
	
	~UMTriangle() {}
	
	/**
	 * ray triangle intersection static version
	 * @param [in] v1 vertex 1
	 * @param [in] v2 vertex 2
	 * @param [in] v3 vertex 3
	 * @param [in] ray a ray
	 * @param [in,out] parameter shading parameters
	 * @param [out] barycentric barycentric coordinate value
	 */
	static bool intersects(
		const UMVec3d& v1,
		const UMVec3d& v2,
		const UMVec3d& v3,
		const UMRay& ray, 
		UMShaderParameter& parameter);

	/**
	 * ray triangle intersection static version
	 * @param [in] v1 vertex 1
	 * @param [in] v2 vertex 2
	 * @param [in] v3 vertex 3
	 * @param [in] ray a ray
	 */
	static bool intersects(
		const UMVec3d& v1,
		const UMVec3d& v2,
		const UMVec3d& v3,
		const UMRay& ray);

	/**
	 * get index
	 */
	const UMVec3i& vertex_index() const { return vertex_index_; }
	
	/**
	 * set index
	 * @param [in] index source index
	 */
	void set_vertex_index(const UMVec3i& index) { vertex_index_ = index; }

	/**
	 * get face index
	 */
	int face_index() const { return face_index_; }

	/**
	 * set face index
	 */
	void set_face_index(const int face_index) { face_index_ = face_index; }

	/**
	 * get normal
	 */
	const UMVec3d& normal() const { return normal_; }
	
	/**
	 * set normal
	 * @param [in] normal source normal
	 */
	void set_normal(const UMVec3d& normal) { normal_ = normal; }

	/**
	 * ray triangle intersection
	 * @param [in] ray a ray
	 * @param [in,out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& parameter) const;

	/**
	 * ray triangle intersection
	 * @param [in] ray a ray
	 */
	virtual bool intersects(const UMRay& ray) const;
	
	/**
	 * get box
	 */
	virtual const UMBox& box() const { return box_; }
	
	/**
	 * update AABB
	 */
	virtual void update_box();
	
private:
	UMMeshPtr mesh_;
	
	int face_index_;
	UMVec3i vertex_index_;
	UMVec3d normal_;
	
	UMBox box_;
};

} // burger

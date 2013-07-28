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
		index_(0),
		normal_(0),
		color_(0){}

	/**
	 * @param [in] index center index
	 * @param [in] normal normal
	 */
	UMTriangle(UMMeshPtr mesh, const UMVec3i& index) :
		mesh_(mesh),
		index_(index),
		normal_(0),
		color_(0){}
	
	~UMTriangle() {}

	/**
	 * get index
	 */
	const UMVec3i& index() const { return index_; }
	
	/**
	 * set index
	 * @param [in] index source index
	 */
	void set_index(const UMVec3i& index) { index_ = index; }

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
	 * set color
	 * @param [in] color source color
	 */
	void set_color(const UMVec3d& color) { color_ = color; }

	/**
	 * ray triangle intersection
	 * @param [in] ray a ray
	 * @param [in,out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& parameter) const;
	
	/**
	 * ray triangle intersection static version
	 * @param [in] v1 vertex 1
	 * @param [in] v2 vertex 2
	 * @param [in] v3 vertex 3
	 * @param [in] ray a ray
	 * @param [in,out] parameter shading parameters
	 */
	static bool intersects(
		const UMVec3d& v1,
		const UMVec3d& v2,
		const UMVec3d& v3,
		const UMRay& ray, 
		UMShaderParameter& parameter);

	/**
	 * ray triangle intersection
	 * @param [in] ray a ray
	 */
	virtual bool intersects(const UMRay& ray) const;

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


private:
	UMMeshPtr mesh_;

	UMVec3i index_;
	UMVec3d normal_;
	
	UMVec3d color_;
};

} // burger

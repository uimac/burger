/**
 * @file UMPlane.h
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
#include "UMBox.h"
#include "UMMaterial.h"

namespace burger
{

class UMPlane;
typedef std::shared_ptr<UMPlane> UMPlanePtr;

class UMRay;
class UMShaderParameter;
class UMMesh;
typedef std::shared_ptr<UMMesh> UMMeshPtr;

/**
 * a plane
 */
class UMPlane : public UMPrimitive
{
	DISALLOW_COPY_AND_ASSIGN(UMPlane);
public:
	UMPlane() :
		point_(0),
		normal_(0, 1, 0),
		material_(UMMaterial::default_material()) {}

	/**
	 * @param [in] point center point
	 * @param [in] normal normal
	 */
	UMPlane(const UMVec3d& point, const UMVec3d& normal) :
		point_(point),
		normal_(normal),
		material_(UMMaterial::default_material()) {}
	
	~UMPlane() {}

	/**
	 * get point
	 */
	const UMVec3d& point() const { return point_; }
	
	/**
	 * set point
	 * @param [in] point source point
	 */
	void set_point(const UMVec3d& point) { point_ = point; }

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
	void set_color(const UMVec3d& color) { material_->set_diffuse(UMVec4d(color, 1.0)); }

	/**
	 * ray plane intersection
	 * @param [in] ray a ray
	 * @param [in,out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& parameter) const;
	
	/**
	 * ray plane intersection
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
	virtual void update_box() {}

	/**
	 * convert to plane mesh
	 * @param [in] width width
	 * @param [in] height height
	 * @retval UMMeshPtr converted mesh
	 */
	UMMeshPtr convert_to_mesh(double width, double height) const;
	
private:
	UMVec3d point_;
	UMVec3d normal_;
	
	UMBox box_;
	UMMaterialPtr material_;
};

} // burger

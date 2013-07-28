/**
 * @file UMBox.h
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

namespace burger
{

class UMBox;
typedef std::shared_ptr<UMBox> UMBoxPtr;

class UMRay;
class UMShaderParameter;
class UMMesh;
typedef std::shared_ptr<UMMesh> UMMeshPtr;

/**
 * AABB
 */
class UMBox : public UMPrimitive
{
	DISALLOW_COPY_AND_ASSIGN(UMBox);
public:
	UMBox() :
		min_(0, 0, 0),
		max_(0, 0, 0),
		color_(0){}

	/**
	 * @param [in] min minimum point of this box
	 * @param [in] max minimum point of this box
	 */
	UMBox(const UMVec3d& min, const UMVec3d& max) :
		min_(min),
		max_(max),
		color_(0){}
	
	~UMBox() {}

	/**
	 * get minimum
	 */
	const UMVec3d& minimum() const { return min_; }
	
	/**
	 * set minimum
	 * @param [in] minimum minimum point of this box
	 */
	void set_minimum(const UMVec3d& min) { min_ = min; }
	
	/**
	 * get maximum
	 */
	const UMVec3d& maximum() const { return max_; }

	/**
	 * set maximum
	 * @param [in] maximum maximum point of this box
	 */
	void set_maximum(const UMVec3d& max) { max_ = max; }

	/**
	 * is empty
	 */
	bool is_empty() const { return min_.x >= max_.x && min_.y >= max_.y && min_.z >= max_.z; }
	
	/**
	 * get center
	 */
	UMVec3d center() const { return (min_ + max_) / 2.0; }

	/**
	 * get size
	 */
	UMVec3d size() const { return max_ - min_; }

	/** 
	 * extend box by point
	 */
	void extend(const UMVec3d& p);

	/**
	 * get normal at point
	 */
	UMVec3d normal(const UMVec3d& point) const;

	/**
	 * ray AABB intersection
	 * @param [in] ray a ray
	 * @param [in,out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& parameter) const;
	
	/**
	 * ray AABB intersection
	 * @param [in] ray a ray
	 */
	virtual bool intersects(const UMRay& ray) const;
	
	/**
	 * convert to plane mesh
	 * @retval UMMeshPtr converted mesh
	 */
	UMMeshPtr convert_to_mesh() const;

private:
	UMVec3d max_;
	UMVec3d min_;
	
	UMVec3d color_;
};

} // burger

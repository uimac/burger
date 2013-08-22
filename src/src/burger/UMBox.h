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

#include <limits>
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
		min_( (std::numeric_limits<double>::infinity)() ),
		max_( -(std::numeric_limits<double>::infinity)() ){}
	
	/**
	 * @param [in] v initialize point
	 */
	UMBox(const UMVec3d& v) :
		min_(v),
		max_(v){}

	/**
	 * @param [in] min minimum point of this box
	 * @param [in] max minimum point of this box
	 */
	UMBox(const UMVec3d& min, const UMVec3d& max) :
		min_(min),
		max_(max){}
	
	~UMBox() {}

	/**
	 * initialize
	 */
	void init() {
		min_ = UMVec3d(  (std::numeric_limits<double>::infinity)() );
		max_ = UMVec3d( -(std::numeric_limits<double>::infinity)() );
	}
	
	/**
	 * get
	 */
	UMVec3d& operator [] (int i) { return i == 0 ? min_ : max_; }
	
	/**
	 * get
	 */
	const UMVec3d& operator [] (int i) const { return i == 0 ? min_ : max_; }

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
	UMVec3d center() const { return (min_ + max_) * 0.5; }

	/**
	 * get size
	 */
	UMVec3d size() const { return max_ - min_; }

	/**
	 * get area
	 */
	double area() const { 
		UMVec3d d = max_ - min_;
		return 2.0 * (d.x * d.y + d.x * d.z + d.y * d.z);
	}

	/** 
	 * extend box by point
	 */
	void extend(const UMVec3d& p);

	/**
	 * extend box by box
	 */
	void extend(const UMBox& box);

	/**
	 * is overlap
	 */
	bool is_overlap(const UMBox& box);

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
	 * ray AABB intersection
	 * @param [in] ray a ray
	 * @param [out] tmin_tmax min and max multipliers "t" of (ray_orig + t * ray_direction)
	 */
	bool intersects(const UMRay& ray, UMVec2d& tmin_tmax) const;

	/**
	 * get box
	 */
	virtual const UMBox& box() const { return *this; }
	
	/**
	 * update AABB
	 */
	virtual void update_box() {}

	/**
	 * convert to box mesh
	 * @retval UMMeshPtr converted mesh
	 */
	UMMeshPtr convert_to_mesh() const;

private:
	UMVec3d max_;
	UMVec3d min_;
};

} // burger

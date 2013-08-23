/**
 * @file UMLight.h
 * a light
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <vector>

#include "UMMacro.h"
#include "UMPrimitive.h"
#include "UMVector.h"

namespace burger
{

class UMLight;
typedef std::shared_ptr<UMLight> UMLightPtr;
typedef std::weak_ptr<UMLight> UMLightWeakPtr;
typedef std::vector< UMLightPtr > UMLightList;

/**
 * a light
 */
class UMLight
{
	DISALLOW_COPY_AND_ASSIGN(UMLight);
public:
	UMLight() {}

	/**
	 * @param [in] position light position
	 */
	UMLight(const UMVec3d& position) :
		position_(position),
		color_(13) {}
	
	~UMLight() {}

	/**
	 * get position
	 */
	UMVec3d position() const { return position_; }
	
	/**
	 * set position
	 * @param [in] position source position
	 */
	void set_position(const UMVec3d& position) { position_ = position; }

	/** 
	 * get color
	 */
	UMVec3d color() const { return color_; }

	/**
	 * set color
	 * @param [in] color light color
	 */
	void set_color(const UMVec3d& color) { color_ = color; }
	
	/** 
	 * sample a point
	 * @param [out] intensity light intensity
	 * @param [out] point sampling point
	 * @param [out] direction light direction
	 * @param [in] parameter shader parameter on sample point
	 * @param [in] random_value random value
	 */
	virtual bool sample(UMVec3d& intensity, UMVec3d& point, UMVec3d& direction, const UMShaderParameter& parameter,
		const UMVec2d& random_value) = 0;
	
private:
	UMVec3d position_;
	UMVec3d color_;
};

} // burger

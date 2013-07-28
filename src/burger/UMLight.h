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

#include "UMMacro.h"
#include "UMPrimitive.h"
#include "UMVector.h"

namespace burger
{

class UMLight;
typedef std::shared_ptr<UMLight> UMLightPtr;
typedef std::weak_ptr<UMLight> UMLightWeakPtr;

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
		position_(position){}
	
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

private:
	UMVec3d position_;
};

} // burger

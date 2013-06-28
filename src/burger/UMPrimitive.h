/**
 * @file UMPrimitive.h
 * interface of primitive
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <memory>
#include "UMMacro.h"

namespace burger
{

class UMPrimitive;
typedef std::shared_ptr<UMPrimitive> UMPrimitivePtr;

class UMRay;
class UMShaderParameter;

/**
 * interface of primitive
 */
class UMPrimitive
{
	DISALLOW_COPY_AND_ASSIGN(UMPrimitive);

public:
	UMPrimitive() {}
	~UMPrimitive() {}
	
	/**
	 * ray intersection
	 * @param [in] ray a ray
	 * @param [in,out] param shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& param) const = 0;
	
	/**
	 * ray sphere intersection
	 * @param [in] ray a ray
	 */
	virtual bool intersects(const UMRay& ray) const = 0;
};

} // burger

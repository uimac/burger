/**
 * @file UMLight.cpp
 * a light
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMLight.h"
#include "UMVector.h"
#include "UMRay.h"
#include "UMShaderParameter.h"

#include <cmath>
#include <cfloat>


namespace burger
{

bool UMLight::sample(UMVec3d& intensity, UMVec3d& direction, const UMShaderParameter& parameter)
{
	return false;
}

} // burger

/// UMPrimitive.h ///
#pragma once

#include <memory>

namespace burger
{

class UMPrimitive;
typedef std::shared_ptr<UMPrimitive> UMPrimitivePtr;

class UMRay;
class UMShaderParameter;

class UMPrimitive
{
public:
	UMPrimitive() {}
	~UMPrimitive() {}
	
	/**
	 * ray sphere intersection
	 * @param [in] ray a ray
	 * @param [in|out] parameter shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& param) const = 0;
};

} // burger

/**
 * @file UMBvh.h
 * bounding volume hierarchy
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <memory>
#include <vector>
#include <string>
#include "UMMacro.h"
#include "UMMath.h"
#include "UMPrimitive.h"

namespace burger
{

class UMBvh;
typedef std::shared_ptr<UMBvh> UMBvhPtr;
typedef std::weak_ptr<UMBvh> UMBvhWeakPtr;

class UMScene;
typedef std::shared_ptr<UMScene> UMScenePtr;

class UMBvhNode;
typedef std::shared_ptr<UMBvhNode> UMBvhNodePtr;
typedef std::vector<UMBvhNodePtr> UMBvhNodeList;

/**
 * a bounding volume hierarchy using SAH(surface area heuristic)
 */
class UMBvh : public UMPrimitive
{
	DISALLOW_COPY_AND_ASSIGN(UMBvh);

public:

	static UMBvhPtr create() { 
		UMBvhPtr instance = UMBvhPtr(new UMBvh);
		instance->self_ptr_ = instance;
		return instance;
	}

	~UMBvh() {}
	
	/**
	 * build bvh from scene
	 * @param [in,out] scene scene
	 * @retval success or fail
	 */
	bool build(UMScenePtr scene);
	
	/**
	 * build bvh from scene
	 * @param [in,out] scene scene
	 * @retval success or fail
	 */
	bool build(UMScene& scene);

	/**
	 * (for debug) create box list
	 */
	UMPrimitiveList create_box_list() const;

	/**
	 * ray intersection
	 * @param [in] ray a ray
	 * @param [in,out] param shading parameters
	 */
	virtual bool intersects(const UMRay& ray, UMShaderParameter& param) const;
	
	/**
	 * ray intersection
	 * @param [in] ray a ray
	 */
	virtual bool intersects(const UMRay& ray) const;
	
	/**
	 * get box
	 */
	virtual const UMBox& box() const;
	
	/**
	 * update AABB
	 */
	virtual void update_box() {}

private:
	UMBvh() {}

	UMBvhNodeList node_list_;
	UMPrimitiveList ordered_primitives_;

	UMBvhPtr self_ptr() { return self_ptr_.lock(); }
	UMBvhWeakPtr self_ptr_;
};

} // burger

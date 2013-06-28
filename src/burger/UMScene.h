/**
 * @file UMScene.h
 * 3D scene including many objects.
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
#include "UMVector.h"
#include "UMPrimitive.h"

/// burger
namespace burger
{

class UMScene;
typedef std::shared_ptr<UMScene> UMScenePtr;

/**
 * 3D scene including many objects, lights, cameras, ...
 */
class UMScene
{
	DISALLOW_COPY_AND_ASSIGN(UMScene);

public:
	UMScene() {}
	~UMScene() {}
	
	typedef std::vector<UMPrimitivePtr> Primitives;

	/// sample scene 1 for test
	void create_sample_scene_1();
	
	/**
	 * get primitives
	 */
	const Primitives& primitives() const { return primitives_; }

	/**
	 * get primitives
	 */
	Primitives& mutable_primitives() { return primitives_; }

	/**
	 * get background color
	 */
	UMVec3d background_color() const { return UMVec3d(0.1, 0.1, 0.1); }

private:
	Primitives primitives_;
};

} // burger

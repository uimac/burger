/**
 * @file UMScene.cpp
 * 3D scene including many objects.
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMScene.h"

#include "UMVector.h"
#include "UMPrimitive.h"
#include "UMSphere.h"
#include "UMPlane.h"

namespace burger
{

/// sample scene 1
void UMScene::create_sample_scene_1()
{
	UMSpherePtr sphere1(std::make_shared<UMSphere>(UMVec3d(0, 0, 0), 250.0));
	UMSpherePtr sphere2(std::make_shared<UMSphere>(UMVec3d(500, 0, 200), 200.0));
	UMPlanePtr plane1(std::make_shared<UMPlane>(UMVec3d(0, -250, 0), UMVec3d(0, 1, 0)));
	sphere1->set_color(UMVec3d(1.0, 0.0, 0.0));
	sphere2->set_color(UMVec3d(0.0, 0.0, 1.0));
	plane1->set_color(UMVec3d(0.0, 1.0, 0.0));
	mutable_primitives().push_back(UMPrimitivePtr(sphere1));
	mutable_primitives().push_back(UMPrimitivePtr(sphere2));
	mutable_primitives().push_back(UMPrimitivePtr(plane1));
}

} // burger

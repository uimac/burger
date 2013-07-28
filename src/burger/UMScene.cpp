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
#include "UMMesh.h"
#include "UMTriangle.h"
#include "UMSphere.h"
#include "UMPlane.h"

namespace burger
{

/// sample scene 1
void UMScene::create_sample_scene_1()
{
	UMSpherePtr sphere1(std::make_shared<UMSphere>(UMVec3d(-400, 0, -50), 250.0));
	UMSpherePtr sphere2(std::make_shared<UMSphere>(UMVec3d(500, 0, -200), 200.0));
	UMPlanePtr plane1(std::make_shared<UMPlane>(UMVec3d(0, -250, 0), UMVec3d(0, 1, 0)));
	sphere1->set_color(UMVec3d(1.0, 0.0, 0.0));
	sphere2->set_color(UMVec3d(0.0, 0.0, 1.0));
	plane1->set_color(UMVec3d(0.0, 1.0, 0.0));
	mutable_primitive_list().push_back(UMPrimitivePtr(sphere1));
	mutable_primitive_list().push_back(UMPrimitivePtr(sphere2));
	mutable_primitive_list().push_back(UMPrimitivePtr(plane1));
}

/// sample scene 2
void UMScene::create_sample_scene_2()
{
	UMSpherePtr sphere1(std::make_shared<UMSphere>(UMVec3d(-200, -100, 0), 200));
	UMSpherePtr sphere2(std::make_shared<UMSphere>(UMVec3d(200, -100, 0), 200.0));
	UMSpherePtr sphere3(std::make_shared<UMSphere>(UMVec3d(0, 250, 0), 200));
	UMPlanePtr plane1(std::make_shared<UMPlane>(UMVec3d(0, -250, 0), UMVec3d(0, 1, 0)));

	sphere1->set_color(UMVec3d(0.0, 1.0, 0.0));
	sphere2->set_color(UMVec3d(0.0, 0.0, 1.0));
	sphere3->set_color(UMVec3d(1.0, 0.0, 0.0));

	//sphere1->set_color(UMVec3d(1.0, 1.0, 0.0));
	//sphere2->set_color(UMVec3d(1.0, 0.0, 1.0));
	//sphere3->set_color(UMVec3d(0.0, 1.0, 1.0));
	
	plane1->set_color(UMVec3d(0.5, 1.0, 0.5));
	
	UMMeshPtr mesh( std::make_shared<UMMesh>() );

	// triangle
	mesh->mutable_vertex_list().push_back(UMVec3d( 350, -100, 200));
	mesh->mutable_vertex_list().push_back(UMVec3d( 650, -100, 200));
	mesh->mutable_vertex_list().push_back(UMVec3d( 500, 200, 200));
	UMTrianglePtr triangle(std::make_shared<UMTriangle>(mesh, UMVec3i(0, 1, 2)));
	triangle->set_color(UMVec3d(1.0, 0.5, 0.0));
	
	mutable_primitive_list().push_back(UMPrimitivePtr(triangle));
	mutable_primitive_list().push_back(UMPrimitivePtr(sphere1));
	mutable_primitive_list().push_back(UMPrimitivePtr(sphere2));
	mutable_primitive_list().push_back(UMPrimitivePtr(sphere3));
	mutable_primitive_list().push_back(UMPrimitivePtr(plane1));
}

/** 
 * init scene
 */
void UMScene::init(int width, int height)
{
	width_ = width;
	height_ = height;

	primitive_list_.clear();
	light_list_.clear();
	camera_ = std::make_shared<UMCamera>(width, height);
}

} // burger

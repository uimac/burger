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
#include "UMCamera.h"
#include "UMLight.h"
#include "UMMeshGroup.h"

/// burger
namespace burger
{

class UMScene;
typedef std::shared_ptr<UMScene> UMScenePtr;
typedef std::weak_ptr<UMScene> UMSceneWeakPtr;

class UMBvh;
typedef std::shared_ptr<UMBvh> UMBvhPtr;

/**
 * 3D scene including many objects, lights, cameras, ...
 */
class UMScene
{
	DISALLOW_COPY_AND_ASSIGN(UMScene);

public:
	UMScene() { init(1280, 720); }
	UMScene(int width, int height) { init(width, height); }
	~UMScene() {}

	void create_sample_scene_1();
	void create_sample_scene_2();
	
	/** 
	 * init scene
	 */
	void init(int width, int height);
	
	/**
	 * get width
	 */
	int width() const { return width_; }
	
	/**
	 * get height
	 */
	int height() const { return height_; }

	/**
	 * get camera
	 */
	UMCameraPtr camera() const { return camera_; }

	/**
	 * get camera
	 */
	UMCameraPtr mutable_camera() { return camera_; }

	/**
	 * get primitive list
	 */
	const UMPrimitiveList& primitive_list() const { return primitive_list_; }

	/**
	 * get primitive list
	 */
	UMPrimitiveList& mutable_primitive_list() { return primitive_list_; }
	
	/**
	 * get mesh group list
	 */
	const UMMeshGroupList& mesh_group_list() const { return mesh_group_list_; }
	
	/**
	 * get mesh group list
	 */
	UMMeshGroupList& mutable_mesh_group_list() { return mesh_group_list_; }
	
	/**
	 * get light list
	 */
	const UMLightList& light_list() const { return light_list_; }

	/**
	 * get light list
	 */
	UMLightList& mutable_light_list() { return light_list_; }
	
	/**
	 * get background color
	 */
	UMVec3d background_color() const { return UMVec3d(0.1, 0.1, 0.1); }

	/**
	 * get bvh
	 */
	UMBvhPtr bvh() const { return bvh_; }

	/**
	 * update bvh
	 */
	bool update_bvh();

	/**
	 * connect listener to all event on scene
	 */
	void connect_to_all_events(UMListenerPtr listener);

private:

	int width_;
	int height_;

	UMCameraPtr camera_;
	UMPrimitiveList primitive_list_;
	UMLightList light_list_;

	UMMeshGroupList mesh_group_list_;
	UMBvhPtr bvh_;
};

} // burger

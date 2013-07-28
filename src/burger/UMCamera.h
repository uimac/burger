/**
 * @file UMCamera.h
 * a Camera
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
#include "UMMacro.h"
#include "UMMath.h"

namespace burger
{

class UMCamera;
typedef std::shared_ptr<UMCamera> UMCameraPtr;
typedef std::weak_ptr<UMCamera> UMCameraWeakPtr;

class UMRay;

/**
 * a camera
 */
class UMCamera
{
	DISALLOW_COPY_AND_ASSIGN(UMCamera);

public:
	explicit UMCamera(int width, int height) {
		init(width, height);
	}
	~UMCamera() {}

	/**
	 * initialize camera
	 */
	void init(int width, int height);
	
	/**
	 * refresh camera matrix
	 */
	void refresh() {
		view_projection_matrix_ = view_matrix() * projection_matrix();
	}

	/** 
	 * get view matrix
	 */
	const UMMat44d& view_matrix() const { return view_matrix_; }

	/** 
	 * set view matrix
	 */
	void set_view_matrix(const UMMat44d& mat) { view_matrix_ = mat; }
	
	/** 
	 * get projection matrix
	 */
	const UMMat44d& projection_matrix() const { return projection_matrix_; }

	/** 
	 * set projection matrix
	 */
	void set_projection_matrix(const UMMat44d& mat) { projection_matrix_ = mat; }
	
	/**
	 * get view projection matrix
	 */
	const UMMat44d& view_projection_matrix() const { return view_projection_matrix_; }
	
	/** 
	 * generate a camera ray
	 * @param [out] ray generated ray
	 * @param [in] x imageplane's pixel x
	 * @param [in] x imageplane's pixel y
	 */
	void generate_ray(UMRay& ray, int x, int y) const;

	/** 
	 * get aspect
	 */
	double aspect() const { return aspect_; }

	/**
	 * get vertical fov
	 */
	double fov_y() const { return fov_y_; }

	/**
	 * get horizontal fov
	 */
	double fov_x() const { return um_to_degree(2.0 * atan(tan( fov_y() / 2.0) *aspect())); }

	/**
	 * get near
	 */
	double near_plane() const { return near_; }

	/**
	 * get far
	 */
	double far_plane() const { return far_; }

	/**
	 * get positoin
	 */
	const UMVec3d& position() const { return position_; }

	/**
	 * get target
	 */
	const UMVec3d& target() const { return target_; }

	/**
	 * get up
	 */
	const UMVec3d& up() const { return up_; }

	/**
	 * rotate camera
	 * @param [in] mx x-axis mouse move
	 * @param [in] my y-axis mouse move
	 */
	void rotate(double mx, double my);

	/**
	 * zoom camera
	 * @param [in] mx x-axis mouse move
	 * @param [in] my y-axis mouse move
	 */
	void zoom(double mx, double my);

private:
	UMMat44d view_matrix_;
	UMMat44d projection_matrix_;
	UMMat44d view_projection_matrix_;

	double aspect_;
	double fov_y_;
	double near_;
	double far_;
	UMVec3d position_;
	UMVec3d target_;
	UMVec3d up_;
		
	double theta_;
	double phi_;

	// for performance
	double inverted_width_;
	double inverted_height_;
};

} // burger

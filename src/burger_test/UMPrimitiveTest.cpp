/**
 * @file UMPrimitiveTest.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license.
 *
 */
#include <boost/test/unit_test.hpp>
#include "UMSphere.h"
#include "UMPlane.h"
#include "UMBox.h"
#include "UMMesh.h"
#include "UMTriangle.h"
#include "UMRay.h"

#define TEST_EPSILON 0.0001

BOOST_AUTO_TEST_SUITE(UMMatrixTest)

using namespace burger;

BOOST_AUTO_TEST_CASE(BoxIntersectTest)
{
	UMBox sphere(UMVec3d(-10, -10, -10), UMVec3d(10, 10, 10));

	UMRay ray1(UMVec3d(-5, -5, 100), UMVec3d(0, 0, -1));
	UMRay ray2(UMVec3d(0, 0, 100), UMVec3d(0, 0, -1));
	UMRay ray3(UMVec3d(5, 5, 100), UMVec3d(0, 0, -1));

	BOOST_CHECK_EQUAL(true, sphere.intersects(ray1));
	BOOST_CHECK_EQUAL(true, sphere.intersects(ray2));
	BOOST_CHECK_EQUAL(true, sphere.intersects(ray3));
	
	UMRay ray4(UMVec3d(-9.9, 9.9, 200), UMVec3d(0, 0, -1));
	UMRay ray5(UMVec3d(-10.1, 9.9, 200), UMVec3d(0, 0, -1));
	UMRay ray6(UMVec3d( 9.9, -9.9, 200), UMVec3d(0, 0, -1));
	UMRay ray7(UMVec3d( 9.9, -10.1, 200), UMVec3d(0, 0, -1));

	BOOST_CHECK_EQUAL(true, sphere.intersects(ray4));
	BOOST_CHECK_EQUAL(false, sphere.intersects(ray5));
	BOOST_CHECK_EQUAL(true, sphere.intersects(ray6));
	BOOST_CHECK_EQUAL(false, sphere.intersects(ray7));
}

BOOST_AUTO_TEST_CASE(SphereIntersectTest)
{
	UMSphere sphere(UMVec3d(0), 100);

	UMRay ray1(UMVec3d(0, 0, 200), UMVec3d(0, 0, -1));
	UMRay ray2(UMVec3d(50, 50, 200), UMVec3d(0, 0, -1));
	UMRay ray3(UMVec3d(-50, -50, 200), UMVec3d(0, 0, -1));

	BOOST_CHECK_EQUAL(true, sphere.intersects(ray1));
	BOOST_CHECK_EQUAL(true, sphere.intersects(ray2));
	BOOST_CHECK_EQUAL(true, sphere.intersects(ray3));
	
	UMRay ray4(UMVec3d(-100, 0, 200), UMVec3d(0, 0, -1));
	UMRay ray5(UMVec3d(-101, 0, 200), UMVec3d(0, 0, -1));
	UMRay ray6(UMVec3d( 0, -100, 200), UMVec3d(0, 0, -1));
	UMRay ray7(UMVec3d( 0, -101, 200), UMVec3d(0, 0, -1));

	BOOST_CHECK_EQUAL(true, sphere.intersects(ray4));
	BOOST_CHECK_EQUAL(false, sphere.intersects(ray5));
	BOOST_CHECK_EQUAL(true, sphere.intersects(ray6));
	BOOST_CHECK_EQUAL(false, sphere.intersects(ray7));
}


BOOST_AUTO_TEST_CASE(TriangleIntersectTest)
{
	UMMeshPtr mesh( std::make_shared<UMMesh>() );

	// triangle
	mesh->mutable_vertex_list().push_back(UMVec3d(-100, -100, 0));
	mesh->mutable_vertex_list().push_back(UMVec3d( 100, -100, 0));
	mesh->mutable_vertex_list().push_back(UMVec3d(   0,  100, 0));
	UMTriangle triangle(mesh, UMVec3i(0, 1, 2), 0);

	UMRay ray1(UMVec3d(0, 0, 200), UMVec3d(0, 0, -1));
	UMRay ray2(UMVec3d(-50, -50, 200), UMVec3d(0, 0, -1));
	UMRay ray3(UMVec3d( 50, -50, 200), UMVec3d(0, 0, -1));
	
	BOOST_CHECK_EQUAL(true, triangle.intersects(ray1));
	BOOST_CHECK_EQUAL(true, triangle.intersects(ray2));
	BOOST_CHECK_EQUAL(true, triangle.intersects(ray3));
	
	UMRay ray4(UMVec3d(-99, -99, 200), UMVec3d(0, 0, -1));
	UMRay ray5(UMVec3d(-101, -100, 200), UMVec3d(0, 0, -1));
	UMRay ray6(UMVec3d( 0, 99, 200), UMVec3d(0, 0, -1));
	UMRay ray7(UMVec3d( 0, 101, 200), UMVec3d(0, 0, -1));
	
	BOOST_CHECK_EQUAL(true, triangle.intersects(ray4));
	BOOST_CHECK_EQUAL(false, triangle.intersects(ray5));
	BOOST_CHECK_EQUAL(true, triangle.intersects(ray6));
	BOOST_CHECK_EQUAL(false, triangle.intersects(ray7));
}

BOOST_AUTO_TEST_SUITE_END()

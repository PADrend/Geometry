/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2013 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "TriangleTest.h"
#include "Triangle.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Matrix3x3.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TriangleTest);

void TriangleTest::testBarycentricCoordinates() {
	const Geometry::Vec3d a(0.0, 0.0, 0.0);
	const Geometry::Vec3d b(30.0, 0.0, 0.0);
	const Geometry::Vec3d c(0.0, 30.0, 0.0);
	const Geometry::Triangle<Geometry::Vec3d> triangle(a, b, c);

	// Check the vertices.
	CPPUNIT_ASSERT_EQUAL(Geometry::Vec3d(1.0, 0.0, 0.0), triangle.calcBarycentricCoordinates(a));
	CPPUNIT_ASSERT_EQUAL(Geometry::Vec3d(0.0, 1.0, 0.0), triangle.calcBarycentricCoordinates(b));
	CPPUNIT_ASSERT_EQUAL(Geometry::Vec3d(0.0, 0.0, 1.0), triangle.calcBarycentricCoordinates(c));

	// Check the middle of edges.
	CPPUNIT_ASSERT_EQUAL(Geometry::Vec3d(0.5, 0.5, 0.0),
						 triangle.calcBarycentricCoordinates(Geometry::Vec3d(15.0, 0.0, 0.0)));
	CPPUNIT_ASSERT_EQUAL(Geometry::Vec3d(0.0, 0.5, 0.5),
						 triangle.calcBarycentricCoordinates(Geometry::Vec3d(15.0, 15.0, 0.0)));
	CPPUNIT_ASSERT_EQUAL(Geometry::Vec3d(0.5, 0.0, 0.5),
						 triangle.calcBarycentricCoordinates(Geometry::Vec3d(0.0, 15.0, 0.0)));

	// Check the centroid.
	CPPUNIT_ASSERT_EQUAL(Geometry::Vec3d(1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0),
						 triangle.calcBarycentricCoordinates(Geometry::Vec3d(10.0, 10.0, 0.0)));

	CPPUNIT_ASSERT_EQUAL(Geometry::Vec3d(0.8, 0.1, 0.1),
						 triangle.calcBarycentricCoordinates(Geometry::Vec3d(3.0, 3.0, 0.0)));
}

void TriangleTest::testTriangleClosestPoint() {
	//              normal of ca
	//                          / p
	//                         /  x ....... extension of ab
	//                        /.....
	//                -------X
	//         -------       a-----
	//  -------                    -----
	// X--------------------------------X
	// b                                c
	// p is nearest to ca, and not to a.
	const Geometry::Vec2d a(16.0, 2.0);
	const Geometry::Vec2d b(0.0, 0.0);
	const Geometry::Vec2d c(22.0, 0.0);
	const Geometry::Triangle<Geometry::Vec2d> triangle(a, b, c);

	const Geometry::Vec2d p(20.0, 3.0);

	const double delta = 1.0e-6;
	Geometry::Vec3d barycentric;
	const Geometry::Vec2d closest = triangle.closestPoint(p, barycentric);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, Geometry::Vec2d(19.3, 0.9).distanceSquared(closest), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.45, barycentric.getX(), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, barycentric.getY(), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.55, barycentric.getZ(), delta);
}

void TriangleTest::testTriangleDistance() {
	/* c
	   x
	   |\
	   | \
	   |  \
	   |   \
	   x----x
	   a    b */
	const Geometry::Vec2d a(0.0, 0.0);
	const Geometry::Vec2d b(30.0, 0.0);
	const Geometry::Vec2d c(0.0, 30.0);
	const Geometry::Triangle<Geometry::Vec2d> triangle(a, b, c);

	const double delta = 1.0e-6;

	// Check the vertices.
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, triangle.distanceSquared(a), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, triangle.distanceSquared(b), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, triangle.distanceSquared(c), delta);

	// Check the middle of edges.
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, triangle.distanceSquared(Geometry::Vec2d(15.0, 0.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, triangle.distanceSquared(Geometry::Vec2d(15.0, 15.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, triangle.distanceSquared(Geometry::Vec2d(0.0, 15.0)), delta);

	// Check the centroid.
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, triangle.distanceSquared(Geometry::Vec2d(10.0, 10.0)), delta);

	// Test points left of the triangle.
	CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, triangle.distanceSquared(Geometry::Vec2d(-10.0, -10.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, triangle.distanceSquared(Geometry::Vec2d(-10.0, 0.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, triangle.distanceSquared(Geometry::Vec2d(-10.0, 30.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, triangle.distanceSquared(Geometry::Vec2d(-10.0, 40.0)), delta);

	// Test points below the triangle.
	CPPUNIT_ASSERT_DOUBLES_EQUAL(800.0, triangle.distanceSquared(Geometry::Vec2d(-20.0, -20.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0, triangle.distanceSquared(Geometry::Vec2d(0.0, -20.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0, triangle.distanceSquared(Geometry::Vec2d(30.0, -20.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(800.0, triangle.distanceSquared(Geometry::Vec2d(50.0, -20.0)), delta);

	// Test points top right of the triangle.
	CPPUNIT_ASSERT_DOUBLES_EQUAL(900.0, triangle.distanceSquared(Geometry::Vec2d(60.0, 0.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1800.0, triangle.distanceSquared(Geometry::Vec2d(45.0, 45.0)), delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(900.0, triangle.distanceSquared(Geometry::Vec2d(0.0, 60.0)), delta);
}

void TriangleTest::testTriangleArea() {
	const double delta = 9.0e-5; // use a larger delta here; the calculations are not extremely accurate

	CPPUNIT_ASSERT_DOUBLES_EQUAL(50.0, Geometry::Triangle<Geometry::Vec3d>(Geometry::Vec3d(0.0, 0.0, 0.0),
																		   Geometry::Vec3d(10.0, 0.0, 0.0),
																		   Geometry::Vec3d(-5.0, 10.0, 0.0)).calcArea(),
								 delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(50.0, Geometry::Triangle<Geometry::Vec3d>(Geometry::Vec3d(0.0, 0.0, 0.0),
																		   Geometry::Vec3d(10.0, 0.0, 0.0),
																		   Geometry::Vec3d(5.0, 10.0, 0.0)).calcArea(),
								 delta);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, Geometry::Triangle<Geometry::Vec3d>(Geometry::Vec3d(0.0, 0.0, 0.0),
																		  Geometry::Vec3d(0.0, 0.0, 0.0),
																		  Geometry::Vec3d(0.0, 0.0, 0.0)).calcArea(),
								 delta);

	const Geometry::Vec3d a(1.0, 1.0, 1.0);
	const Geometry::Vec3d b(11.0, 1.0, 1.0);
	const Geometry::Vec3d c(-6.0, 21.0, 1.0);

	// rotate a triangle (should keep the area invariant)
	for (int i = 0; i < 1000; ++i) {
		Geometry::Matrix3x3d r = Geometry::Matrix3x3d::createRotation(
				Geometry::Angle::rad(static_cast<float>(i) * 0.01), Geometry::Vec3d(1.0, 0.0, 0.0));
		r.normOrthoLize();
		r = Geometry::Matrix3x3d::createRotation(Geometry::Angle::rad(static_cast<float>(i) * 0.017),
												 Geometry::Vec3d(0.0, 1.0, 0.0)) * r;
		r.normOrthoLize();
		r = Geometry::Matrix3x3d::createRotation(Geometry::Angle::rad(static_cast<float>(i) * 0.027),
												 Geometry::Vec3d(0.0, 0.0, 1.0)) * r;
		r.normOrthoLize();

		const Geometry::Vec3d a2(r * a);
		const Geometry::Vec3d b2(r * b);
		const Geometry::Vec3d c2(r * c);
		const Geometry::Triangle<Geometry::Vec3d> triangle(a2, b2, c2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, triangle.calcArea(), delta);
	}
}

/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2013 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2011 Claudius Jähn <claudius@uni-paderborn.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "PlaneTest.h"
#include "Line.h"
#include "Plane.h"
#include <sstream>
#include <cmath>
CPPUNIT_TEST_SUITE_REGISTRATION(PlaneTest);

void PlaneTest::setUp() {
	// simple plane p1 (y-up)
	p1 = Geometry::Plane(Geometry::Vec3(0, 1, 0), 17);

	// contructor with three points
	p1b = Geometry::Plane(Geometry::Vec3(10, 17, 0), Geometry::Vec3(0, 17, -10), Geometry::Vec3(0, 17, 0));
	p1c = Geometry::Plane(Geometry::Vec3(10, 16, 0), Geometry::Vec3(0, 17, -10), Geometry::Vec3(0, 17, 0));

	// contructor with position and normal
	p1d = Geometry::Plane(Geometry::Vec3(-13, 17, 78), Geometry::Vec3(0, 1, 0));

	// rotated plane p2
	/*      ^y
			|        /
			|       /
			|      /
			|     /\
			|    /  O
			----------------->x
			*/
	const float d = sqrtf(2.0);

	p2 = Geometry::Plane(Geometry::Vec3(-1, 1, 0).normalize(), d);

	// contructor with three points
	p2b = Geometry::Plane(Geometry::Vec3(2, 4, 20), Geometry::Vec3(2, 4, -20), Geometry::Vec3(-3, -1, 20));

	// contructor with position and normal
	p2c = Geometry::Plane(Geometry::Vec3(2, 4, 78), Geometry::Vec3(-1, 1, 0).normalize());
}

void PlaneTest::testConstruction() {
	CPPUNIT_ASSERT(p1 == p1b);
	CPPUNIT_ASSERT(!(p1 == p1c));
	CPPUNIT_ASSERT(p1 == p1d);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(p2b.getOffset(), p2.getOffset(), 0.001);
	CPPUNIT_ASSERT((p2.getNormal() - p2b.getNormal()).length() < 0.001);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(p2c.getOffset(), p2.getOffset(), 0.001);
	CPPUNIT_ASSERT((p2.getNormal() - p2c.getNormal()).length() < 0.001);
}

void PlaneTest::testDistance() {
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.planeTest(Geometry::Vec3(1, 2, 3)), -15.0, 0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.planeTest(Geometry::Vec3(1, 17, 3)), 0.0, 0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.planeTest(Geometry::Vec3(18, 19, 20)), 2.0, 0.001);

	const float d = sqrtf(2.0);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(p2.planeTest(Geometry::Vec3(2, 1, 17)) + 1.5 * d, 0.0, 0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p2.planeTest(Geometry::Vec3(-3, -1, 0)) - 0.0, 0.0, 0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p2.planeTest(Geometry::Vec3(-1, 3, -17)) - d, 0.0, 0.001);
}

void PlaneTest::testIntersection() {
	// intersection with segment
	Geometry::Vec3 s;
	CPPUNIT_ASSERT(p1.getIntersection(Geometry::Segment3(Geometry::Vec3(18, 0, 20), Geometry::Vec3(18, 17, 20)), s)
				   == true);
	CPPUNIT_ASSERT(s == Geometry::Vec3(18, 17, 20));

	// segment too short; no intersection
	CPPUNIT_ASSERT(p1.getIntersection(Geometry::Segment3(Geometry::Vec3(18, 0, 20), Geometry::Vec3(18, 16, 20)), s)
				   == false);

	// parallel line; no intersection
	CPPUNIT_ASSERT(p1.getIntersection(Geometry::Line3(Geometry::Vec3(1, 2, 3), Geometry::Vec3(17, 0, 3)), s) == false);

	// intersection with segment
	CPPUNIT_ASSERT(p2.getIntersection(Geometry::Segment3(Geometry::Vec3(3, 2, 20), Geometry::Vec3(1, 6, 20)), s)
				   == true);
	CPPUNIT_ASSERT(s == Geometry::Vec3(2, 4, 20));
}

void PlaneTest::testProjection() {
	// projection
	CPPUNIT_ASSERT(p1.getProjection(Geometry::Vec3(18, 19, 20)) == Geometry::Vec3(18, 17, 20));

	CPPUNIT_ASSERT((p2.getProjection(Geometry::Vec3(-1, 3, 20)) - Geometry::Vec3(0, 2, 20)).length() < 0.001);
}

void PlaneTest::testMisc() {
	// Compare to functions in vec3
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p2.planeTest(Geometry::Vec3(2, 1, 17)),
								 Geometry::Vec3(Geometry::Vec3(2, 1, 17)).planeTest(p2.getNormal(), p2.getOffset()),
								 0.001);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(
			p2.planeTest(Geometry::Vec3(2, 1, 17)),
			Geometry::Vec3(Geometry::Vec3(2, 1, 17)).planeTest(p2.getNormal() * p2.getOffset(), p2.getNormal()), 0.001);
}

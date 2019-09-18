/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2013 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2011 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Line.h"
#include "Plane.h"
#include <sstream>
#include <cmath>
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))
#define REQUIRE_DOUBLES_EQUAL(a,b,e) REQUIRE((((a) <= (b) + e) && ((b) <= (a) + e)))

static Geometry::Plane p1;
static Geometry::Plane p1b;
static Geometry::Plane p1c;
static Geometry::Plane p1d;
static Geometry::Plane p2;
static Geometry::Plane p2b;
static Geometry::Plane p2c;
	
static void setUp() {
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

TEST_CASE("PlaneTest_testConstruction", "[PlaneTest]") {
	setUp();
	REQUIRE(p1 == p1b);
	REQUIRE(!(p1 == p1c));
	REQUIRE(p1 == p1d);

	REQUIRE_DOUBLES_EQUAL(p2b.getOffset(), p2.getOffset(), 0.001);
	REQUIRE((p2.getNormal() - p2b.getNormal()).length() < 0.001);

	REQUIRE_DOUBLES_EQUAL(p2c.getOffset(), p2.getOffset(), 0.001);
	REQUIRE((p2.getNormal() - p2c.getNormal()).length() < 0.001);
}

TEST_CASE("PlaneTest_testDistance", "[PlaneTest]") {
	setUp();
	REQUIRE_DOUBLES_EQUAL(p1.planeTest(Geometry::Vec3(1, 2, 3)), -15.0, 0.001);
	REQUIRE_DOUBLES_EQUAL(p1.planeTest(Geometry::Vec3(1, 17, 3)), 0.0, 0.001);
	REQUIRE_DOUBLES_EQUAL(p1.planeTest(Geometry::Vec3(18, 19, 20)), 2.0, 0.001);

	const float d = sqrtf(2.0);

	REQUIRE_DOUBLES_EQUAL(p2.planeTest(Geometry::Vec3(2, 1, 17)) + 1.5 * d, 0.0, 0.001);
	REQUIRE_DOUBLES_EQUAL(p2.planeTest(Geometry::Vec3(-3, -1, 0)) - 0.0, 0.0, 0.001);
	REQUIRE_DOUBLES_EQUAL(p2.planeTest(Geometry::Vec3(-1, 3, -17)) - d, 0.0, 0.001);
}

TEST_CASE("PlaneTest_testIntersection", "[PlaneTest]") {
	setUp();
	// intersection with segment
	Geometry::Vec3 s;
	REQUIRE(p1.getIntersection(Geometry::Segment3(Geometry::Vec3(18, 0, 20), Geometry::Vec3(18, 17, 20)), s)
				   == true);
	REQUIRE(s == Geometry::Vec3(18, 17, 20));

	// segment too short; no intersection
	REQUIRE(p1.getIntersection(Geometry::Segment3(Geometry::Vec3(18, 0, 20), Geometry::Vec3(18, 16, 20)), s)
				   == false);

	// parallel line; no intersection
	REQUIRE(p1.getIntersection(Geometry::Line3(Geometry::Vec3(1, 2, 3), Geometry::Vec3(17, 0, 3)), s) == false);

	// intersection with segment
	REQUIRE(p2.getIntersection(Geometry::Segment3(Geometry::Vec3(3, 2, 20), Geometry::Vec3(1, 6, 20)), s)
				   == true);
	REQUIRE(s == Geometry::Vec3(2, 4, 20));
}

TEST_CASE("PlaneTest_testProjection", "[PlaneTest]") {
	setUp();
	// projection
	REQUIRE(p1.getProjection(Geometry::Vec3(18, 19, 20)) == Geometry::Vec3(18, 17, 20));

	REQUIRE((p2.getProjection(Geometry::Vec3(-1, 3, 20)) - Geometry::Vec3(0, 2, 20)).length() < 0.001);
}

TEST_CASE("PlaneTest_testMisc", "[PlaneTest]") {
	setUp();
	// Compare to functions in vec3
	REQUIRE_DOUBLES_EQUAL(p2.planeTest(Geometry::Vec3(2, 1, 17)),
								 Geometry::Vec3(Geometry::Vec3(2, 1, 17)).planeTest(p2.getNormal(), p2.getOffset()),
								 0.001);

	REQUIRE_DOUBLES_EQUAL(
			p2.planeTest(Geometry::Vec3(2, 1, 17)),
			Geometry::Vec3(Geometry::Vec3(2, 1, 17)).planeTest(p2.getNormal() * p2.getOffset(), p2.getNormal()), 0.001);
}

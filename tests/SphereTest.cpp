/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Sphere.h"
#include <cmath>
#include <sstream>
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))
#define REQUIRE_DOUBLES_EQUAL(a,b,e) REQUIRE((((a) <= (b) + e) && ((b) <= (a) + e)))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#endif


using namespace Geometry;

TEST_CASE("SphereTest_testConstructors", "[SphereTest]") {
	REQUIRE_EQUAL(Sphere_i(Vec3i(0, 0, 0), 1), Sphere_i());
	const Sphere_i s1(Vec3i(1, 2, 3), 5);
	REQUIRE_EQUAL(s1, Sphere_i(s1));
}

TEST_CASE("SphereTest_testGetters", "[SphereTest]") {
	const Sphere_i s1;
	const Sphere_i s2(Vec3i(1, 2, 3), 5);
	const Sphere_i s3(Vec3i(-6, -7, -8), 25);

	REQUIRE(Sphere_i(Vec3i(1, 2, 3), 5) == s2);
	REQUIRE(!(Sphere_i(Vec3i(1, 2, 3), -5) == s2));
	REQUIRE(!(Sphere_i(Vec3i(1, 2, 3), 6) == s2));
	REQUIRE(!(Sphere_i(Vec3i(-1, 2, 3), 5) == s2));
	REQUIRE(!(Sphere_i(Vec3i(1, -2, 3), 5) == s2));
	REQUIRE(!(Sphere_i(Vec3i(1, 2, -3), 5) == s2));
	REQUIRE(!(Sphere_i(Vec3i(-1, -2, -3), -5) == s2));

	REQUIRE_EQUAL(Vec3i(0, 0, 0), s1.getCenter());
	REQUIRE_EQUAL(Vec3i(1, 2, 3), s2.getCenter());
	REQUIRE_EQUAL(Vec3i(-6, -7, -8), s3.getCenter());

	REQUIRE_EQUAL(1, s1.getRadius());
	REQUIRE_EQUAL(5, s2.getRadius());
	REQUIRE_EQUAL(25, s3.getRadius());

	REQUIRE_DOUBLES_EQUAL(-1, Sphere_d().distance(Vec3d(0, 0, 0)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(0, Sphere_d().distance(Vec3d(1, 0, 0)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(0, Sphere_d().distance(Vec3d(0, 1, 0)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(0, Sphere_d().distance(Vec3d(0, 0, 1)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(1, Sphere_d().distance(Vec3d(2, 0, 0)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(1, Sphere_d().distance(Vec3d(0, 2, 0)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(1, Sphere_d().distance(Vec3d(0, 0, 2)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(-5, Sphere_d(Vec3d(1, 2, 3), 5).distance(Vec3d(1, 2, 3)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(-4, Sphere_d(Vec3d(1, 2, 3), 5).distance(Vec3d(2, 2, 3)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(-3, Sphere_d(Vec3d(1, 2, 3), 5).distance(Vec3d(3, 2, 3)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(-2, Sphere_d(Vec3d(1, 2, 3), 5).distance(Vec3d(4, 2, 3)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(-1, Sphere_d(Vec3d(1, 2, 3), 5).distance(Vec3d(5, 2, 3)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(0, Sphere_d(Vec3d(1, 2, 3), 5).distance(Vec3d(6, 2, 3)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(1, Sphere_d(Vec3d(1, 2, 3), 5).distance(Vec3d(7, 2, 3)), 1.0e-16);
	REQUIRE_DOUBLES_EQUAL(2, Sphere_d(Vec3d(1, 2, 3), 5).distance(Vec3d(8, 2, 3)), 1.0e-16);

	REQUIRE(!s1.isOutside(Vec3i(0, 0, 0)));
	REQUIRE(!s1.isOutside(Vec3i(1, 0, 0)));
	REQUIRE(!s1.isOutside(Vec3i(0, 1, 0)));
	REQUIRE(!s1.isOutside(Vec3i(0, 0, 1)));
	REQUIRE(s1.isOutside(Vec3i(2, 0, 0)));
	REQUIRE(s1.isOutside(Vec3i(0, 2, 0)));
	REQUIRE(s1.isOutside(Vec3i(0, 0, 2)));
}

TEST_CASE("SphereTest_testSetters", "[SphereTest]") {
	Sphere_i s1;
	const Sphere_i s2(Vec3i(1, 2, 3), 5);
	const Sphere_i s3(Vec3i(-6, -7, -8), 25);
	s1 = s2;
	REQUIRE_EQUAL(s2, s1);
	s1 = s3;
	REQUIRE_EQUAL(s3, s1);
	s1.setCenter(Vec3i(-90, -80, -70));
	REQUIRE_EQUAL(Vec3i(-90, -80, -70), s1.getCenter());
	s1.setRadius(-30);
	REQUIRE_EQUAL(-30, s1.getRadius());
}

static void assertSpheresEqual(const Sphere_d & s1, const Sphere_d & s2, double epsilon) {
	REQUIRE_DOUBLES_EQUAL(s1.getCenter().getX(), s2.getCenter().getX(), epsilon);
	REQUIRE_DOUBLES_EQUAL(s1.getCenter().getY(), s2.getCenter().getY(), epsilon);
	REQUIRE_DOUBLES_EQUAL(s1.getCenter().getZ(), s2.getCenter().getZ(), epsilon);
	REQUIRE_DOUBLES_EQUAL(s1.getRadius(), s2.getRadius(), epsilon);
}

TEST_CASE("SphereTest_testMisc", "[SphereTest]") {
	Sphere_d s1(Vec3d(1, 0, 0), 0);

	const Vec3d pos[5] = {Vec3d(3, 0, 0), Vec3d(2, 3, 0), Vec3d(2, 5, 0), Vec3d(2, 2, 7), Vec3d(2, 2, 9)};

	for (unsigned int i = 0; i < 5; ++i) {
		for (unsigned int j = i; j < 5; ++j) {
			REQUIRE(s1.isOutside(pos[j]));
		}
		s1.include(pos[i]);
		for (unsigned int j = 0; j <= i; ++j) {
			REQUIRE(!s1.isOutside(pos[j]));
		}
	}

	REQUIRE_EQUAL(Sphere_d(Vec3d(2, 2, 3), 6), s1);

	const Sphere_d s1Copy(s1);
	REQUIRE_EQUAL(s1, s1Copy);
	s1.include(Vec3d(2, 2, 2));
	REQUIRE_EQUAL(s1, s1Copy);
	s1.include(Vec3d(8, 2, 3));
	REQUIRE_EQUAL(s1, s1Copy);

	{
		const Sphere_d sValid1(Vec3d(1, 2, 3), 6);
		const Sphere_d sValid2(Vec3d(9, 8, 7), 4);
		const Sphere_d sInvalid1(Vec3d(5, 6, 7), -1);
		const Sphere_d sInvalid2(Vec3d(1, 2, 3), -10);
		// Include of an invalid sphere
		REQUIRE_EQUAL(sValid1, Sphere_d(sValid1).include(sInvalid1));
		REQUIRE_EQUAL(sValid1, Sphere_d(sValid1).include(sInvalid2));
		REQUIRE_EQUAL(sValid2, Sphere_d(sValid2).include(sInvalid1));
		REQUIRE_EQUAL(sValid2, Sphere_d(sValid2).include(sInvalid2));
		// Include into an invalid sphere
		REQUIRE_EQUAL(sValid1, Sphere_d(sInvalid1).include(sValid1));
		REQUIRE_EQUAL(sValid2, Sphere_d(sInvalid1).include(sValid2));
		REQUIRE_EQUAL(sValid1, Sphere_d(sInvalid2).include(sValid1));
		REQUIRE_EQUAL(sValid2, Sphere_d(sInvalid2).include(sValid2));
		// Include of an already included sphere
		REQUIRE_EQUAL(sValid1, Sphere_d(sValid1).include(Sphere_d(sValid1.getCenter(), 5)));
		REQUIRE_EQUAL(sValid1, Sphere_d(sValid1).include(Sphere_d(Vec3d(2, 2, 2), 1)));
		REQUIRE_EQUAL(sValid2, Sphere_d(sValid2).include(Sphere_d(sValid2.getCenter(), 3)));
		REQUIRE_EQUAL(sValid2, Sphere_d(sValid2).include(Sphere_d(Vec3d(8, 8, 8), 1)));
		// Include of an including sphere
		REQUIRE_EQUAL(sValid1, Sphere_d(sValid1.getCenter(), 5).include(sValid1));
		REQUIRE_EQUAL(sValid1, Sphere_d(Vec3d(2, 2, 2), 1).include(sValid1));
		REQUIRE_EQUAL(sValid2, Sphere_d(sValid2.getCenter(), 3).include(sValid2));
		REQUIRE_EQUAL(sValid2, Sphere_d(Vec3d(8, 8, 8), 1).include(sValid2));
		// Include of outside sphere
		REQUIRE_EQUAL(Sphere_d(Vec3d(2, 0, 0), 6),
							 Sphere_d(Vec3d(0, 0, 0), 4).include(Sphere_d(Vec3d(4, 0, 0), 4)));
		REQUIRE_EQUAL(Sphere_d(Vec3d(4, 0, 0), 8),
							 Sphere_d(Vec3d(0, 0, 0), 4).include(Sphere_d(Vec3d(8, 0, 0), 4)));
		REQUIRE_EQUAL(Sphere_d(Vec3d(5, 0, 0), 10),
							 Sphere_d(Vec3d(0, 0, 0), 5).include(Sphere_d(Vec3d(10, 0, 0), 5)));

		assertSpheresEqual(Sphere_d(Vec3d(9, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 1).include(Sphere_d(Vec3d(10, 0, 0), 9)), 1.0e-6);
		assertSpheresEqual(Sphere_d(Vec3d(8, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 2).include(Sphere_d(Vec3d(10, 0, 0), 8)), 1.0e-6);
		assertSpheresEqual(Sphere_d(Vec3d(7, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 3).include(Sphere_d(Vec3d(10, 0, 0), 7)), 1.0e-6);
		assertSpheresEqual(Sphere_d(Vec3d(6, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 4).include(Sphere_d(Vec3d(10, 0, 0), 6)), 1.0e-6);
		assertSpheresEqual(Sphere_d(Vec3d(5, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 5).include(Sphere_d(Vec3d(10, 0, 0), 5)), 1.0e-6);
		assertSpheresEqual(Sphere_d(Vec3d(4, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 6).include(Sphere_d(Vec3d(10, 0, 0), 4)), 1.0e-6);
		assertSpheresEqual(Sphere_d(Vec3d(3, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 7).include(Sphere_d(Vec3d(10, 0, 0), 3)), 1.0e-6);
		assertSpheresEqual(Sphere_d(Vec3d(2, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 8).include(Sphere_d(Vec3d(10, 0, 0), 2)), 1.0e-6);
		assertSpheresEqual(Sphere_d(Vec3d(1, 0, 0), 10),
						   Sphere_d(Vec3d(0, 0, 0), 9).include(Sphere_d(Vec3d(10, 0, 0), 1)), 1.0e-6);
	}
}

TEST_CASE("SphereTest_testCoordinates", "[SphereTest]") {
	const double dist = 1.0e-10;

	REQUIRE(Vec3d(0.0, 1.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(0.0, 0.0)) < dist);
	REQUIRE(Vec3d(0.0, 1.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(0.0, M_PI_2))
				   < dist);
	REQUIRE(Vec3d(0.0, 1.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(0.0, M_PI)) < dist);
	REQUIRE(Vec3d(0.0, 1.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(0.0, 3.0 * M_PI_2))
				   < dist);

	REQUIRE(Vec3d(0.0, -1.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(M_PI, 0.0))
				   < dist);
	REQUIRE(Vec3d(0.0, -1.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(M_PI, M_PI_2))
				   < dist);
	REQUIRE(Vec3d(0.0, -1.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(M_PI, M_PI))
				   < dist);
	REQUIRE(Vec3d(0.0, -1.0, 0.0)
						   .distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(M_PI, 3.0 * M_PI_2)) < dist);

	REQUIRE(Vec3d(1.0, 0.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(M_PI_2, 0.0))
				   < dist);
	REQUIRE(Vec3d(0.0, 0.0, 1.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(M_PI_2, M_PI_2))
				   < dist);
	REQUIRE(Vec3d(-1.0, 0.0, 0.0).distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(M_PI_2, M_PI))
				   < dist);
	REQUIRE(Vec3d(0.0, 0.0, -1.0)
						   .distanceSquared(Sphere_d::calcCartesianCoordinateUnitSphere(M_PI_2, 3.0 * M_PI_2)) < dist);

	{
		const Sphere_d sphere(Vec3d(5.0, 5.0, 5.0), 2.0);

		REQUIRE(Vec3d(5.0, 7.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(0.0, 0.0)) < dist);
		REQUIRE(Vec3d(5.0, 7.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(0.0, M_PI_2)) < dist);
		REQUIRE(Vec3d(5.0, 7.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(0.0, M_PI)) < dist);
		REQUIRE(Vec3d(5.0, 7.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(0.0, 3.0 * M_PI_2)) < dist);

		REQUIRE(Vec3d(5.0, 3.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(M_PI, 0.0)) < dist);
		REQUIRE(Vec3d(5.0, 3.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(M_PI, M_PI_2)) < dist);
		REQUIRE(Vec3d(5.0, 3.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(M_PI, M_PI)) < dist);
		REQUIRE(Vec3d(5.0, 3.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(M_PI, 3.0 * M_PI_2)) < dist);

		REQUIRE(Vec3d(7.0, 5.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(M_PI_2, 0.0)) < dist);
		REQUIRE(Vec3d(5.0, 5.0, 7.0).distanceSquared(sphere.calcCartesianCoordinate(M_PI_2, M_PI_2)) < dist);
		REQUIRE(Vec3d(3.0, 5.0, 5.0).distanceSquared(sphere.calcCartesianCoordinate(M_PI_2, M_PI)) < dist);
		REQUIRE(Vec3d(5.0, 5.0, 3.0).distanceSquared(sphere.calcCartesianCoordinate(M_PI_2, 3.0 * M_PI_2))
					   < dist);
	}
}

TEST_CASE("SphereTest_testSerialization", "[SphereTest]") {
	const Sphere_i s1;
	const Sphere_i s2(Vec3i(1, 2, 3), 5);
	const Sphere_i s3(Vec3i(-6, -7, -8), 25);

	std::stringstream stream;
	stream << s1 << ' ' << s2 << ' ' << s3;

	Sphere_i s1Copy;
	stream >> s1Copy;
	Sphere_i s2Copy;
	stream >> s2Copy;
	Sphere_i s3Copy;
	stream >> s3Copy;

	REQUIRE_EQUAL(s1, s1Copy);
	REQUIRE_EQUAL(s2, s2Copy);
	REQUIRE_EQUAL(s3, s3Copy);
}

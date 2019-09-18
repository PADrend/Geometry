/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Sphere.h"
#include "Vec2.h"
#include "Vec3.h"
#include "VecHelper.h"
#include <cmath>
#include <limits>
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))
#define REQUIRE_DOUBLES_EQUAL(a,b,e) REQUIRE((((a) <= (b) + e) && ((b) <= (a) + e)))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
static const double oneDegree = M_PI / 180.0;
static const double epsilon = 16.0 * std::numeric_limits<double>::epsilon();

TEST_CASE("VecHelperTest_testVecHelper2D", "[VecHelperTest]") {
	using vec2_t = Geometry::_Vec2<double>;

	for (double angle = 0.0; angle < 2.0 * M_PI; angle += oneDegree) {
		const vec2_t vector(std::cos(angle), std::sin(angle));
		REQUIRE_DOUBLES_EQUAL(1.0, vector.length(), epsilon);
		const auto orthoVector = Geometry::Helper::createOrthogonal(vector);
		REQUIRE_DOUBLES_EQUAL(0.0, vector.dot(orthoVector), epsilon);
		REQUIRE_DOUBLES_EQUAL(1.0, orthoVector.length(), epsilon);

		const auto scaledVector = vector * 17.0;
		REQUIRE_DOUBLES_EQUAL(17.0, scaledVector.length(), epsilon);
		const auto scaledOrthoVector = Geometry::Helper::createOrthogonal(scaledVector);
		REQUIRE_DOUBLES_EQUAL(0.0, scaledVector.dot(scaledOrthoVector), epsilon);
		REQUIRE_DOUBLES_EQUAL(17.0, scaledOrthoVector.length(), epsilon);
	}
}

TEST_CASE("VecHelperTest_testVecHelper3D", "[VecHelperTest]") {
	using sphere_t = Geometry::_Sphere<double>;

	for (double inclination = 0.0; inclination <= M_PI; inclination += oneDegree) {
		for (double azimuth = 0.0; azimuth < 2.0 * M_PI; azimuth += oneDegree) {
			const auto vector = sphere_t::calcCartesianCoordinateUnitSphere(inclination, azimuth);
			REQUIRE_DOUBLES_EQUAL(1.0, vector.length(), epsilon);
			const auto orthoVector = Geometry::Helper::createOrthogonal(vector);
			REQUIRE_DOUBLES_EQUAL(0.0, vector.dot(orthoVector), epsilon);
			REQUIRE_DOUBLES_EQUAL(1.0, orthoVector.length(), epsilon);
		}
	}
}

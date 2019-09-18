/*
	This file is part of the Geometry library.
	Copyright (C) 2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Interpolation.h"
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))
#define REQUIRE_DOUBLES_EQUAL(a,b,e) REQUIRE((((a) <= (b) + e) && ((b) <= (a) + e)))

TEST_CASE("InterpolationTest_testClamp", "[InterpolationTest]") {
	using namespace Geometry::Interpolation;

	const int lower = 4;
	const int upper = 17;
	for (int i = -50; i <= 50; ++i) {
		if (i < lower) {
			REQUIRE_EQUAL(lower, clamp(lower, i, upper));
		} else if (i > upper) {
			REQUIRE_EQUAL(upper, clamp(lower, i, upper));
		} else {
			REQUIRE_EQUAL(i, clamp(lower, i, upper));
		}
	}
}

TEST_CASE("InterpolationTest_testLinear", "[InterpolationTest]") {
	using namespace Geometry::Interpolation;

	const double epsilon = 1.0e-9;

	{
		const double p0 = 0.0;
		const double p1 = 1.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				REQUIRE_DOUBLES_EQUAL(p0, linear(p0, p1, t), epsilon);
			} else if (t > 1.0) {
				REQUIRE_DOUBLES_EQUAL(p1, linear(p0, p1, t), epsilon);
			} else {
				REQUIRE_DOUBLES_EQUAL(t, linear(p0, p1, t), epsilon);
			}
		}
	}

	{
		const double p0 = -17.0;
		const double p1 = 25.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				REQUIRE_DOUBLES_EQUAL(p0, linear(p0, p1, t), epsilon);
			} else if (t > 1.0) {
				REQUIRE_DOUBLES_EQUAL(p1, linear(p0, p1, t), epsilon);
			} else {
				REQUIRE_DOUBLES_EQUAL((1.0 - t) * p0 + t * p1, linear(p0, p1, t), epsilon);
			}
		}
	}
}

TEST_CASE("InterpolationTest_testQuadraticBezier", "[InterpolationTest]") {
	using namespace Geometry::Interpolation;

	const double epsilon = 1.0e-9;

	{
		const double p0 = 0.0;
		const double p1 = 0.5;
		const double p2 = 1.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				REQUIRE_DOUBLES_EQUAL(p0, quadraticBezier(p0, p1, p2, t), epsilon);
			} else if (t > 1.0) {
				REQUIRE_DOUBLES_EQUAL(p2, quadraticBezier(p0, p1, p2, t), epsilon);
			} else {
				REQUIRE_DOUBLES_EQUAL(t, quadraticBezier(p0, p1, p2, t), epsilon);
			}
		}
	}

	{
		const double p0 = -17.0;
		const double p1 = 5.0;
		const double p2 = 25.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				REQUIRE_DOUBLES_EQUAL(p0, quadraticBezier(p0, p1, p2, t), epsilon);
			} else if (t > 1.0) {
				REQUIRE_DOUBLES_EQUAL(p2, quadraticBezier(p0, p1, p2, t), epsilon);
			} else {
				const double v = clamp(0.0, t, 1.0);
				const double w = 1.0 - v;
				REQUIRE_DOUBLES_EQUAL(w * w * p0 + 2.0 * w * v * p1 + v * v * p2, quadraticBezier(p0, p1, p2, t),
											 epsilon);
			}
		}
	}
}

TEST_CASE("InterpolationTest_testCubicBezier", "[InterpolationTest]") {
	using namespace Geometry::Interpolation;

	const double epsilon = 1.0e-9;

	{
		const double p0 = 0.0;
		const double p1 = 1.0 / 3.0;
		const double p2 = 2.0 / 3.0;
		const double p3 = 1.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				REQUIRE_DOUBLES_EQUAL(p0, cubicBezier(p0, p1, p2, p3, t), epsilon);
			} else if (t > 1.0) {
				REQUIRE_DOUBLES_EQUAL(p3, cubicBezier(p0, p1, p2, p3, t), epsilon);
			} else {
				REQUIRE_DOUBLES_EQUAL(t, cubicBezier(p0, p1, p2, p3, t), epsilon);
			}
		}
	}

	{
		const double p0 = -17.0;
		const double p1 = -2.0;
		const double p2 = 5.0;
		const double p3 = 25.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				REQUIRE_DOUBLES_EQUAL(p0, cubicBezier(p0, p1, p2, p3, t), epsilon);
			} else if (t > 1.0) {
				REQUIRE_DOUBLES_EQUAL(p3, cubicBezier(p0, p1, p2, p3, t), epsilon);
			} else {
				const double v = clamp(0.0, t, 1.0);
				const double w = 1.0 - v;
				REQUIRE_DOUBLES_EQUAL(w * w * w * p0 + 3.0 * w * w * v * p1 + 3.0 * w * v * v * p2
													 + v * v * v * p3,
											 cubicBezier(p0, p1, p2, p3, t), epsilon);
			}
		}
	}
}

/*
	This file is part of the Geometry library.
	Copyright (C) 2012 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "InterpolationTest.h"
#include "Interpolation.h"
CPPUNIT_TEST_SUITE_REGISTRATION(InterpolationTest);

void InterpolationTest::testClamp() {
	using namespace Geometry::Interpolation;

	const int lower = 4;
	const int upper = 17;
	for (int i = -50; i <= 50; ++i) {
		if (i < lower) {
			CPPUNIT_ASSERT_EQUAL(lower, clamp(lower, i, upper));
		} else if (i > upper) {
			CPPUNIT_ASSERT_EQUAL(upper, clamp(lower, i, upper));
		} else {
			CPPUNIT_ASSERT_EQUAL(i, clamp(lower, i, upper));
		}
	}
}

void InterpolationTest::testLinear() {
	using namespace Geometry::Interpolation;

	const double epsilon = 1.0e-9;

	{
		const double p0 = 0.0;
		const double p1 = 1.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p0, linear(p0, p1, t), epsilon);
			} else if (t > 1.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p1, linear(p0, p1, t), epsilon);
			} else {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(t, linear(p0, p1, t), epsilon);
			}
		}
	}

	{
		const double p0 = -17.0;
		const double p1 = 25.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p0, linear(p0, p1, t), epsilon);
			} else if (t > 1.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p1, linear(p0, p1, t), epsilon);
			} else {
				CPPUNIT_ASSERT_DOUBLES_EQUAL((1.0 - t) * p0 + t * p1, linear(p0, p1, t), epsilon);
			}
		}
	}
}

void InterpolationTest::testQuadraticBezier() {
	using namespace Geometry::Interpolation;

	const double epsilon = 1.0e-9;

	{
		const double p0 = 0.0;
		const double p1 = 0.5;
		const double p2 = 1.0;
		for (int i = -10; i <= 20; ++i) {
			const double t = 0.1 * i;
			if (t < 0.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p0, quadraticBezier(p0, p1, p2, t), epsilon);
			} else if (t > 1.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p2, quadraticBezier(p0, p1, p2, t), epsilon);
			} else {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(t, quadraticBezier(p0, p1, p2, t), epsilon);
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
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p0, quadraticBezier(p0, p1, p2, t), epsilon);
			} else if (t > 1.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p2, quadraticBezier(p0, p1, p2, t), epsilon);
			} else {
				const double v = clamp(0.0, t, 1.0);
				const double w = 1.0 - v;
				CPPUNIT_ASSERT_DOUBLES_EQUAL(w * w * p0 + 2.0 * w * v * p1 + v * v * p2, quadraticBezier(p0, p1, p2, t),
											 epsilon);
			}
		}
	}
}

void InterpolationTest::testCubicBezier() {
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
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p0, cubicBezier(p0, p1, p2, p3, t), epsilon);
			} else if (t > 1.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p3, cubicBezier(p0, p1, p2, p3, t), epsilon);
			} else {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(t, cubicBezier(p0, p1, p2, p3, t), epsilon);
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
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p0, cubicBezier(p0, p1, p2, p3, t), epsilon);
			} else if (t > 1.0) {
				CPPUNIT_ASSERT_DOUBLES_EQUAL(p3, cubicBezier(p0, p1, p2, p3, t), epsilon);
			} else {
				const double v = clamp(0.0, t, 1.0);
				const double w = 1.0 - v;
				CPPUNIT_ASSERT_DOUBLES_EQUAL(w * w * w * p0 + 3.0 * w * w * v * p1 + 3.0 * w * v * v * p2
													 + v * v * v * p3,
											 cubicBezier(p0, p1, p2, p3, t), epsilon);
			}
		}
	}
}

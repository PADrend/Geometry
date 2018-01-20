/*
	This file is part of the Geometry library.
	Copyright (C) 2012 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef INTERPOLATIONTEST_H_
#define INTERPOLATIONTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class InterpolationTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(InterpolationTest);
	CPPUNIT_TEST(testClamp);
	CPPUNIT_TEST(testLinear);
	CPPUNIT_TEST(testQuadraticBezier);
	CPPUNIT_TEST(testCubicBezier);
	CPPUNIT_TEST_SUITE_END();

public:
	void testClamp();
	void testLinear();
	void testQuadraticBezier();
	void testCubicBezier();
};

#endif /* INTERPOLATIONTEST_H_ */

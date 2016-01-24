/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2012 Claudius Jähn <claudius@uni-paderborn.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef TRIANGLETEST_H_
#define TRIANGLETEST_H_

#include <cppunit/extensions/HelperMacros.h>

class TriangleTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TriangleTest);
	CPPUNIT_TEST(testBarycentricCoordinates);
	CPPUNIT_TEST(testTriangleClosestPoint);
	CPPUNIT_TEST(testTriangleDistance);
	CPPUNIT_TEST(testTriangleArea);
	CPPUNIT_TEST_SUITE_END();

public:
	void testBarycentricCoordinates();
	void testTriangleClosestPoint();
	void testTriangleDistance();
	void testTriangleArea();
};

#endif /* TRIANGLETEST_H_ */

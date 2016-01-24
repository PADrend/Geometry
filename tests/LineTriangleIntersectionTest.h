/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef LINETRIANGLEINTERSECTIONTEST_H_
#define LINETRIANGLEINTERSECTIONTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class LineTriangleIntersectionTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(LineTriangleIntersectionTest);
	CPPUNIT_TEST(testLineTriangleIntersection);
	CPPUNIT_TEST_SUITE_END();

public:
	void testLineTriangleIntersection();
};

#endif /* LINETRIANGLEINTERSECTIONTEST_H_ */

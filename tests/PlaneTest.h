/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2011 Claudius Jähn <claudius@uni-paderborn.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef PLANETEST_H_
#define PLANETEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include "Plane.h"

class PlaneTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(PlaneTest);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testDistance);
	CPPUNIT_TEST(testIntersection);
	CPPUNIT_TEST(testProjection);
	CPPUNIT_TEST(testMisc);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void testConstruction();
	void testDistance();
	void testIntersection();
	void testProjection();
	void testMisc();

private:
	Geometry::Plane p1;
	Geometry::Plane p1b;
	Geometry::Plane p1c;
	Geometry::Plane p1d;
	Geometry::Plane p2;
	Geometry::Plane p2b;
	Geometry::Plane p2c;
};

#endif /* PLANETEST_H_ */

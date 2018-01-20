/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef SPHERETEST_H_
#define SPHERETEST_H_

#include <cppunit/extensions/HelperMacros.h>

class SphereTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SphereTest);
	CPPUNIT_TEST(testConstructors);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testSetters);
	CPPUNIT_TEST(testCoordinates);
	CPPUNIT_TEST(testMisc);
	CPPUNIT_TEST(testSerialization);
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructors();
	void testGetters();
	void testSetters();
	void testCoordinates();
	void testMisc();
	void testSerialization();
};

#endif /* SPHERETEST_H_ */

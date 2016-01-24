/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef RECTTEST_H_
#define RECTTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class RectTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(RectTest);
	CPPUNIT_TEST(testConstructors);
	CPPUNIT_TEST(testOperators);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testSetters);
	CPPUNIT_TEST(testSerialization);
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructors();
	void testOperators();
	void testGetters();
	void testSetters();
	void testSerialization();
};

#endif /* RECTTEST_H_ */

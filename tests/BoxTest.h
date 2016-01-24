/*
	This file is part of the Geometry library.
	Copyright (C) 2010-2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef BOXTEST_H_
#define BOXTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class BoxTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(BoxTest);
	CPPUNIT_TEST(testOperators);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testSetters);
	CPPUNIT_TEST(testMisc);
	CPPUNIT_TEST(testSerialization);
	CPPUNIT_TEST_SUITE_END();

public:
	void testOperators();
	void testGetters();
	void testSetters();
	void testMisc();
	void testSerialization();
};

#endif /* BOXTEST_H_ */

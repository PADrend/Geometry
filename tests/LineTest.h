/*
	This file is part of the Geometry library.
	Copyright (C) 2010-2011 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef LINETEST_H_
#define LINETEST_H_

#include <cppunit/extensions/HelperMacros.h>

class LineTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(LineTest);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testSetters);
	CPPUNIT_TEST(testMisc);
	CPPUNIT_TEST(testBounds);
	CPPUNIT_TEST_SUITE_END();

public:
	void testGetters();
	void testSetters();
	void testMisc();
	void testBounds();
};

#endif /* LINETEST_H_ */

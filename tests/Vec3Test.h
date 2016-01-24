/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef VEC3TEST_H_
#define VEC3TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class Vec3Test : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(Vec3Test);
	CPPUNIT_TEST(testOperators);
	CPPUNIT_TEST(testFunctions);
	CPPUNIT_TEST(testSerialization);
	CPPUNIT_TEST_SUITE_END();

public:
	void testOperators();
	void testFunctions();
	void testSerialization();
};

#endif /* VEC3TEST_H_ */

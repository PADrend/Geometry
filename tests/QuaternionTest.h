/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef QUATERNIONTEST_H_
#define QUATERNIONTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class QuaternionTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(QuaternionTest);
	CPPUNIT_TEST(testConstructors);
	CPPUNIT_TEST(testOperators);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testSetters);
	CPPUNIT_TEST(testConjugate);
	CPPUNIT_TEST(testNorm);
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructors();
	void testOperators();
	void testGetters();
	void testSetters();
	void testConjugate();
	void testNorm();
};

#endif /* QUATERNIONTEST_H_ */

/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef MATRIX4X4TEST_H_
#define MATRIX4X4TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class Matrix4x4Test : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(Matrix4x4Test);
	CPPUNIT_TEST(testConstructors);
	CPPUNIT_TEST(testOperators);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testSetters);
	CPPUNIT_TEST(testConversion);
	CPPUNIT_TEST(testSerialization);
	CPPUNIT_TEST(testVectorTransformation);
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructors();
	void testOperators();
	void testGetters();
	void testSetters();
	void testConversion();
	void testSerialization();
	void testVectorTransformation();
};

#endif /* MATRIX4X4TEST_H_ */

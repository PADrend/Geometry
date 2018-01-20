/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef VECHELPERTEST_H_
#define VECHELPERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class VecHelperTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(VecHelperTest);
	CPPUNIT_TEST(testVecHelper2D);
	CPPUNIT_TEST(testVecHelper3D);
	CPPUNIT_TEST_SUITE_END();

public:
	void testVecHelper2D();
	void testVecHelper3D();
};

#endif /* VECHELPERTEST_H_ */

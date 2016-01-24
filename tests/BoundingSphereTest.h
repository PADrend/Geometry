/*
	This file is part of the Geometry library.
	Copyright (C) 2012 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef BOUNDINGSPHERETEST_H_
#define BOUNDINGSPHERETEST_H_

#include <cppunit/extensions/HelperMacros.h>

class BoundingSphereTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(BoundingSphereTest);
	CPPUNIT_TEST(testRandomPoints);
	CPPUNIT_TEST(testRandomSpheres);
	CPPUNIT_TEST_SUITE_END();

public:
	void testRandomPoints();
	void testRandomSpheres();
};

#endif /* BOUNDINGSPHERETEST_H_ */

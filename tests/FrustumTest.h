/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef FRUSTUMTEST_H_
#define FRUSTUMTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class FrustumTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FrustumTest);
	CPPUNIT_TEST(testFrustumTest);
	CPPUNIT_TEST_SUITE_END();

public:
	void testFrustumTest();
};

#endif /* FRUSTUMTEST_H_ */

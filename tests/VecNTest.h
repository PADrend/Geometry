/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2011 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef VECNTEST_H_
#define VECNTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class VecNTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(VecNTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void test();
};

#endif /* VECNTEST_H_ */

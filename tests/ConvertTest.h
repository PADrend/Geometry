/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef CONVERTTEST_H_
#define CONVERTTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class ConvertTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ConvertTest);
	CPPUNIT_TEST(testNormalConversion);
	CPPUNIT_TEST_SUITE_END();

public:
	void testNormalConversion();
};

#endif /* CONVERTTEST_H_ */

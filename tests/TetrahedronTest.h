/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2012 Claudius Jähn <claudius@uni-paderborn.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef TETRAHEDRONTEST_H_
#define TETRAHEDRONTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class TetrahedronTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TetrahedronTest);
	CPPUNIT_TEST(testTetrahedronBarycentricCoordinates);
	CPPUNIT_TEST(testTetrahedronDistance);
	CPPUNIT_TEST(testTetrahedronVolume);
	CPPUNIT_TEST_SUITE_END();

public:
	void testTetrahedronBarycentricCoordinates();
	void testTetrahedronDistance();
	void testTetrahedronVolume();
};

#endif /* TETRAHEDRONTEST_H_ */

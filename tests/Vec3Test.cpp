/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2012 Claudius Jähn <claudius@uni-paderborn.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Vec3Test.h"
#include "Vec3.h"
#include <limits>
#include <sstream>
#include <stdint.h>
CPPUNIT_TEST_SUITE_REGISTRATION(Vec3Test);

void Vec3Test::testOperators() {
	const Geometry::Vec3d a(1.0, 2.0, 3.0);
	const Geometry::Vec3d b(3.0, 2.0, 1.0);

	Geometry::Vec3d c;

	const double epsilon = 1.0e-9;

	c = a + b;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, c.x(), epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, c.y(), epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, c.z(), epsilon);

	c = a - b;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0, c.x(), epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, c.y(), epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, c.z(), epsilon);

	c = a.cross(b);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, c.x(), epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, c.y(), epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, c.z(), epsilon);
}

void Vec3Test::testFunctions() {
	const Geometry::Vec3d a(1.0, 2.0, 3.0);
	const Geometry::Vec3d b(3.0, 2.0, 1.0);

	Geometry::Vec3d c;

	const double epsilon = 1.0e-6;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, a.dot(b), epsilon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(std::sqrt(14.0), a.length(), epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(std::sqrt(14.0), b.length(), epsilon);

	CPPUNIT_ASSERT(Geometry::Vec3d(12.34567, 0.0, 0.0).normalize().equals(Geometry::Vec3d(1.0, 0.0, 0.0), epsilon));
	CPPUNIT_ASSERT(Geometry::Vec3d(0.0, 2.34567, 0.0).normalize().equals(Geometry::Vec3d(0.0, 1.0, 0.0), epsilon));
	CPPUNIT_ASSERT(Geometry::Vec3d(0.0, 0.0, 345.6789).normalize().equals(Geometry::Vec3d(0.0, 0.0, 1.0), epsilon));
}

void Vec3Test::testSerialization() {
	{
		std::stringstream stream;

		using IntVec3 = Geometry::_Vec3<int32_t>;
		for (int32_t x = -10; x <= 10; ++x) {
			for (int32_t y = -10; y <= 10; ++y) {
				for (int32_t z = -10; z <= 10; ++z) {
					stream << IntVec3(x, y, z) << ' ';
				}
			}
		}

		for (int32_t x = -10; x <= 10; ++x) {
			for (int32_t y = -10; y <= 10; ++y) {
				for (int32_t z = -10; z <= 10; ++z) {
					IntVec3 vec;
					stream >> vec;
					CPPUNIT_ASSERT_EQUAL(vec.getX(), x);
					CPPUNIT_ASSERT_EQUAL(vec.getY(), y);
					CPPUNIT_ASSERT_EQUAL(vec.getZ(), z);
				}
			}
		}
	}
	{
		std::stringstream stream;

		using FloatVec3 = Geometry::_Vec3<float>;
		for (float x = -1.0f; x <= 1.0f; x += 0.1f) {
			for (float y = -1.0f; y <= 1.0f; y += 0.1f) {
				for (float z = -1.0f; z <= 1.0f; z += 0.1f) {
					stream << FloatVec3(x, y, z) << ' ';
				}
			}
		}

		for (float x = -1.0f; x <= 1.0f; x += 0.1f) {
			for (float y = -1.0f; y <= 1.0f; y += 0.1f) {
				for (float z = -1.0f; z <= 1.0f; z += 0.1f) {
					FloatVec3 vec;
					stream >> vec;
					CPPUNIT_ASSERT_DOUBLES_EQUAL(vec.getX(), x, 2.0f * std::numeric_limits<float>::epsilon());
					CPPUNIT_ASSERT_DOUBLES_EQUAL(vec.getY(), y, 2.0f * std::numeric_limits<float>::epsilon());
					CPPUNIT_ASSERT_DOUBLES_EQUAL(vec.getZ(), z, 2.0f * std::numeric_limits<float>::epsilon());
				}
			}
		}
	}
}

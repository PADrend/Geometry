/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "LineTriangleIntersectionTest.h"
#include "Line.h"
#include "LineTriangleIntersection.h"
#include "Triangle.h"
#include <limits>
CPPUNIT_TEST_SUITE_REGISTRATION(LineTriangleIntersectionTest);

void LineTriangleIntersectionTest::testLineTriangleIntersection() {
	using vec_t = Geometry::_Vec3<double>;
	using line_t = Geometry::_Line<vec_t>;
	using triangle_t = Geometry::Triangle<vec_t>;

	const triangle_t triangle(vec_t(0.0, 0.0, 0.0), vec_t(0.0, 1.0, 0.0), vec_t(0.0, 0.0, 1.0));

	double t, u, v;
	const auto epsilon = std::numeric_limits<double>::epsilon();
	const auto sentinel = std::numeric_limits<double>::max();
	const vec_t dirX(1.0, 0.0, 0.0);
	{
		// Line inside triangle
		const line_t line(vec_t(0.0, 0.2, 0.2), vec_t(0.0, 1.0, 1.0));
		CPPUNIT_ASSERT_EQUAL(false, Geometry::Intersection::getLineTriangleIntersection(line, triangle, t, u, v));
	}
	for (double x = -3.0; x <= 3.0; x += 0.1) {
		for (double y = -3.0; y <= 3.0; y += 0.1) {
			for (double z = -3.0; z <= 3.0; z += 0.1) {
				t = sentinel;
				u = sentinel;
				v = sentinel;
				const line_t line(vec_t(x, y, z), dirX);
				const bool intersection = (y >= 0.0 && z >= 0.0 && (y + z) <= 1.0);
				CPPUNIT_ASSERT_EQUAL(intersection,
									 Geometry::Intersection::getLineTriangleIntersection(line, triangle, t, u, v));
				if (intersection) {
					CPPUNIT_ASSERT_DOUBLES_EQUAL(-x, t, epsilon);
					CPPUNIT_ASSERT_DOUBLES_EQUAL(y, u, epsilon);
					CPPUNIT_ASSERT_DOUBLES_EQUAL(z, v, epsilon);
					CPPUNIT_ASSERT_EQUAL(line.getPoint(t), triangle.calcPoint(u, v));
				} else {
					// Make sure that the values were not changed
					CPPUNIT_ASSERT_DOUBLES_EQUAL(sentinel, t, epsilon);
					CPPUNIT_ASSERT_DOUBLES_EQUAL(sentinel, u, epsilon);
					CPPUNIT_ASSERT_DOUBLES_EQUAL(sentinel, v, epsilon);
				}
			}
		}
	}

	/*
	 *           (29, 19)
	 *              *
	 *             / ` - _ t2
	 * y      t4 /  t1    ` - _
	 * ^       /     ____,-----* (42, 6)
	 * |      *-----´       t3
	 * | (17, 5)
	 * |
	 * o---> x
	 */
	const triangle_t triangle2(vec_t(17, 5, 8), vec_t(42, 6, 8), vec_t(29, 19, 8));
	const vec_t t1(26.3, 9.5, 0.0);
	const vec_t t2(35.3, 13.0, 0.0);
	const vec_t t3(37.0, 5.3, 0.0);
	const vec_t t4(21.3, 11.8, 0.0);
	const vec_t dirZ(0.0, 0.0, 1.0);
	{
		const line_t line(t1, dirZ);
		CPPUNIT_ASSERT_EQUAL(true, Geometry::Intersection::getLineTriangleIntersection(line, triangle2, t, u, v));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, t, epsilon);
		CPPUNIT_ASSERT_EQUAL(line.getPoint(t), triangle2.calcPoint(u, v));
	}
	CPPUNIT_ASSERT_EQUAL(false,
						 Geometry::Intersection::getLineTriangleIntersection(line_t(t2, dirZ), triangle2, t, u, v));
	CPPUNIT_ASSERT_EQUAL(false,
						 Geometry::Intersection::getLineTriangleIntersection(line_t(t3, dirZ), triangle2, t, u, v));
	CPPUNIT_ASSERT_EQUAL(false,
						 Geometry::Intersection::getLineTriangleIntersection(line_t(t4, dirZ), triangle2, t, u, v));
}

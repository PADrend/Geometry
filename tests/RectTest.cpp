/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "RectTest.h"
#include "Rect.h"
#include <sstream>
#include <stdint.h>
CPPUNIT_TEST_SUITE_REGISTRATION(RectTest);

using IntRect = Geometry::_Rect<int32_t>;

void RectTest::testConstructors() {
	CPPUNIT_ASSERT(IntRect() == IntRect(0, 0, 0, 0));
	const IntRect source(5, 6, 10, 20);
	CPPUNIT_ASSERT(IntRect(source) == IntRect(5, 6, 10, 20));
	CPPUNIT_ASSERT(source == IntRect(Geometry::_Rect<float>(5.0f, 6.0f, 10.0f, 20.0f)));
	CPPUNIT_ASSERT(source == IntRect(Geometry::_Rect<double>(5.0, 6.0, 10.0, 20.0)));
}

void RectTest::testOperators() {
	CPPUNIT_ASSERT(IntRect() == IntRect());
	CPPUNIT_ASSERT(!(IntRect(1, 0, 0, 0) == IntRect(0, 0, 0, 0)));
	CPPUNIT_ASSERT(!(IntRect(0, 1, 0, 0) == IntRect(0, 0, 0, 0)));
	CPPUNIT_ASSERT(!(IntRect(0, 0, 1, 0) == IntRect(0, 0, 0, 0)));
	CPPUNIT_ASSERT(!(IntRect(0, 0, 0, 1) == IntRect(0, 0, 0, 0)));
	CPPUNIT_ASSERT(!(IntRect(0, 0, 0, 0) == IntRect(1, 0, 0, 0)));
	CPPUNIT_ASSERT(!(IntRect(0, 0, 0, 0) == IntRect(0, 1, 0, 0)));
	CPPUNIT_ASSERT(!(IntRect(0, 0, 0, 0) == IntRect(0, 0, 1, 0)));
	CPPUNIT_ASSERT(!(IntRect(0, 0, 0, 0) == IntRect(0, 0, 0, 1)));

	CPPUNIT_ASSERT(!(IntRect() != IntRect()));
	CPPUNIT_ASSERT(IntRect(1, 0, 0, 0) != IntRect(0, 0, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 1, 0, 0) != IntRect(0, 0, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 1, 0) != IntRect(0, 0, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 1) != IntRect(0, 0, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) != IntRect(1, 0, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) != IntRect(0, 1, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) != IntRect(0, 0, 1, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) != IntRect(0, 0, 0, 1));

	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) + Geometry::_Vec2<int32_t>(5, 5) == IntRect(5, 5, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) + Geometry::_Vec2<int32_t>(-5, 5) == IntRect(-5, 5, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) + Geometry::_Vec2<int32_t>(5, -5) == IntRect(5, -5, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) + Geometry::_Vec2<int32_t>(-5, -5) == IntRect(-5, -5, 0, 0));

	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) - Geometry::_Vec2<int32_t>(5, 5) == IntRect(-5, -5, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) - Geometry::_Vec2<int32_t>(-5, 5) == IntRect(5, -5, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) - Geometry::_Vec2<int32_t>(5, -5) == IntRect(-5, 5, 0, 0));
	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0) - Geometry::_Vec2<int32_t>(-5, -5) == IntRect(5, 5, 0, 0));
}

void RectTest::testGetters() {
	const IntRect rectA(5, 6, 10, 20);
	const IntRect rectB(2, -3, 7, 2);
	const IntRect rectC(-20, -10, 2, 5);

	CPPUNIT_ASSERT_EQUAL(rectA.getX(), 5);
	CPPUNIT_ASSERT_EQUAL(rectB.getX(), 2);
	CPPUNIT_ASSERT_EQUAL(rectC.getX(), -20);
	CPPUNIT_ASSERT_EQUAL(rectA.getX(), rectA.getMinX());
	CPPUNIT_ASSERT_EQUAL(rectB.getX(), rectB.getMinX());
	CPPUNIT_ASSERT_EQUAL(rectC.getX(), rectC.getMinX());
	CPPUNIT_ASSERT_EQUAL(rectA.getMaxX(), 15);
	CPPUNIT_ASSERT_EQUAL(rectB.getMaxX(), 9);
	CPPUNIT_ASSERT_EQUAL(rectC.getMaxX(), -18);
	CPPUNIT_ASSERT_EQUAL(rectA.getY(), 6);
	CPPUNIT_ASSERT_EQUAL(rectB.getY(), -3);
	CPPUNIT_ASSERT_EQUAL(rectC.getY(), -10);
	CPPUNIT_ASSERT_EQUAL(rectA.getY(), rectA.getMinY());
	CPPUNIT_ASSERT_EQUAL(rectB.getY(), rectB.getMinY());
	CPPUNIT_ASSERT_EQUAL(rectC.getY(), rectC.getMinY());
	CPPUNIT_ASSERT_EQUAL(rectA.getMaxY(), 26);
	CPPUNIT_ASSERT_EQUAL(rectB.getMaxY(), -1);
	CPPUNIT_ASSERT_EQUAL(rectC.getMaxY(), -5);
	CPPUNIT_ASSERT_EQUAL(rectA.getWidth(), 10);
	CPPUNIT_ASSERT_EQUAL(rectB.getWidth(), 7);
	CPPUNIT_ASSERT_EQUAL(rectC.getWidth(), 2);
	CPPUNIT_ASSERT_EQUAL(rectA.getHeight(), 20);
	CPPUNIT_ASSERT_EQUAL(rectB.getHeight(), 2);
	CPPUNIT_ASSERT_EQUAL(rectC.getHeight(), 5);
	CPPUNIT_ASSERT_EQUAL(rectA.getPosition(), Geometry::_Vec2<int32_t>(5, 6));
	CPPUNIT_ASSERT_EQUAL(rectB.getPosition(), Geometry::_Vec2<int32_t>(2, -3));
	CPPUNIT_ASSERT_EQUAL(rectC.getPosition(), Geometry::_Vec2<int32_t>(-20, -10));
	CPPUNIT_ASSERT_EQUAL(rectA.getCenter(), Geometry::_Vec2<int32_t>(10, 16));
	CPPUNIT_ASSERT_EQUAL(rectB.getCenter(), Geometry::_Vec2<int32_t>(5, -2));
	CPPUNIT_ASSERT_EQUAL(rectC.getCenter(), Geometry::_Vec2<int32_t>(-19, -7));

	CPPUNIT_ASSERT(rectA.contains(rectA.getMinX(), rectA.getMinY()));
	CPPUNIT_ASSERT(rectA.contains(rectA.getMaxX(), rectA.getMinY()));
	CPPUNIT_ASSERT(rectA.contains(rectA.getMinX(), rectA.getMaxY()));
	CPPUNIT_ASSERT(rectA.contains(rectA.getMaxX(), rectA.getMaxY()));
	CPPUNIT_ASSERT(rectB.contains(rectB.getMinX(), rectB.getMinY()));
	CPPUNIT_ASSERT(rectB.contains(rectB.getMaxX(), rectB.getMinY()));
	CPPUNIT_ASSERT(rectB.contains(rectB.getMinX(), rectB.getMaxY()));
	CPPUNIT_ASSERT(rectB.contains(rectB.getMaxX(), rectB.getMaxY()));
	CPPUNIT_ASSERT(rectC.contains(rectC.getMinX(), rectC.getMinY()));
	CPPUNIT_ASSERT(rectC.contains(rectC.getMaxX(), rectC.getMinY()));
	CPPUNIT_ASSERT(rectC.contains(rectC.getMinX(), rectC.getMaxY()));
	CPPUNIT_ASSERT(rectC.contains(rectC.getMaxX(), rectC.getMaxY()));

	CPPUNIT_ASSERT(!rectA.contains(rectA.getMinX() - 1, rectA.getMinY()));
	CPPUNIT_ASSERT(!rectA.contains(rectA.getMinX(), rectA.getMinY() - 1));
	CPPUNIT_ASSERT(!rectA.contains(rectA.getMaxX() + 1, rectA.getMinY()));
	CPPUNIT_ASSERT(!rectA.contains(rectA.getMaxX(), rectA.getMinY() - 1));
	CPPUNIT_ASSERT(!rectA.contains(rectA.getMinX() - 1, rectA.getMaxY()));
	CPPUNIT_ASSERT(!rectA.contains(rectA.getMinX(), rectA.getMaxY() + 1));
	CPPUNIT_ASSERT(!rectA.contains(rectA.getMaxX() + 1, rectA.getMaxY()));
	CPPUNIT_ASSERT(!rectA.contains(rectA.getMaxX(), rectA.getMaxY() + 1));

	CPPUNIT_ASSERT(rectA.contains(rectA));
	CPPUNIT_ASSERT(rectB.contains(rectB));
	CPPUNIT_ASSERT(rectC.contains(rectC));

	CPPUNIT_ASSERT(rectA.contains(IntRect(10, 16, 1, 1)));
	CPPUNIT_ASSERT(rectB.contains(IntRect(5, -2, 1, 1)));
	CPPUNIT_ASSERT(rectC.contains(IntRect(-19, -7, 1, 1)));

	CPPUNIT_ASSERT(!rectA.contains(IntRect(rectA.getMinX() - 1, rectA.getMinY(), rectA.getWidth(), rectA.getHeight())));
	CPPUNIT_ASSERT(!rectA.contains(IntRect(rectA.getMinX(), rectA.getMinY() - 1, rectA.getWidth(), rectA.getHeight())));
	CPPUNIT_ASSERT(!rectA.contains(IntRect(rectA.getMinX(), rectA.getMinY(), rectA.getWidth() + 1, rectA.getHeight())));
	CPPUNIT_ASSERT(!rectA.contains(IntRect(rectA.getMinX(), rectA.getMinY(), rectA.getWidth(), rectA.getHeight() + 1)));

	CPPUNIT_ASSERT(rectA.contains(rectA.getPosition()));
	CPPUNIT_ASSERT(rectB.contains(rectB.getPosition()));
	CPPUNIT_ASSERT(rectC.contains(rectC.getPosition()));

	CPPUNIT_ASSERT(rectA.contains(rectA.getCenter()));
	CPPUNIT_ASSERT(rectB.contains(rectB.getCenter()));
	CPPUNIT_ASSERT(rectC.contains(rectC.getCenter()));

	CPPUNIT_ASSERT(!rectA.isInvalid());
	CPPUNIT_ASSERT(!rectB.isInvalid());
	CPPUNIT_ASSERT(!rectC.isInvalid());

	CPPUNIT_ASSERT(!IntRect(0, 0, 0, 0).isInvalid());
	CPPUNIT_ASSERT(IntRect(0, 0, 0, -1).isInvalid());
	CPPUNIT_ASSERT(IntRect(0, 0, -1, 0).isInvalid());
	CPPUNIT_ASSERT(IntRect(0, 0, -1, -1).isInvalid());

	CPPUNIT_ASSERT(rectA.isValid());
	CPPUNIT_ASSERT(rectB.isValid());
	CPPUNIT_ASSERT(rectC.isValid());

	CPPUNIT_ASSERT(IntRect(0, 0, 0, 0).isValid());
	CPPUNIT_ASSERT(!IntRect(0, 0, 0, -1).isValid());
	CPPUNIT_ASSERT(!IntRect(0, 0, -1, 0).isValid());
	CPPUNIT_ASSERT(!IntRect(0, 0, -1, -1).isValid());

	CPPUNIT_ASSERT(IntRect().intersects(IntRect()));
	CPPUNIT_ASSERT(IntRect(0, 0, 10, 10).intersects(IntRect(5, 5, 1, 1)));
	CPPUNIT_ASSERT(IntRect(0, 0, 10, 10).intersects(IntRect(10, 10, 1, 1)));
	CPPUNIT_ASSERT(IntRect(0, 0, 10, 10).intersects(IntRect(-1, -1, 1, 1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(5, 5, -1, -1)));
	CPPUNIT_ASSERT(!IntRect(5, 5, -1, -1).intersects(IntRect(0, 0, 10, 10)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(5, 5, -1, -1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(-2, -2, 1, 1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(11, -2, 1, 1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(-2, 11, 1, 1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(11, 11, 1, 1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(5, -2, 1, 1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(5, 11, 1, 1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(-2, 5, 1, 1)));
	CPPUNIT_ASSERT(!IntRect(0, 0, 10, 10).intersects(IntRect(11, 5, 1, 1)));
}

void RectTest::testSetters() {
	IntRect rect(0, 0, 3, 4);
	rect.setPosition(5, 6);
	CPPUNIT_ASSERT(rect == IntRect(5, 6, 3, 4));
	rect.setPosition(Geometry::_Vec2<int32_t>(7, 8));
	CPPUNIT_ASSERT(rect == IntRect(7, 8, 3, 4));
	rect.setX(9);
	CPPUNIT_ASSERT(rect == IntRect(9, 8, 3, 4));
	rect.setY(10);
	CPPUNIT_ASSERT(rect == IntRect(9, 10, 3, 4));
	rect.setWidth(1);
	CPPUNIT_ASSERT(rect == IntRect(9, 10, 1, 4));
	rect.setHeight(2);
	CPPUNIT_ASSERT(rect == IntRect(9, 10, 1, 2));

	CPPUNIT_ASSERT(rect.isValid());
	rect.invalidate();
	CPPUNIT_ASSERT(rect.isInvalid());

	rect.setSize(11, 12);
	CPPUNIT_ASSERT(rect == IntRect(9, 10, 11, 12));

	rect.moveRel(-1, -3);
	CPPUNIT_ASSERT(rect == IntRect(8, 7, 11, 12));

	rect.moveRel(Geometry::_Vec2<int32_t>(-1, -3));
	CPPUNIT_ASSERT(rect == IntRect(7, 4, 11, 12));

	rect += Geometry::_Vec2<int32_t>(1, 3);
	CPPUNIT_ASSERT(rect == IntRect(8, 7, 11, 12));

	rect -= Geometry::_Vec2<int32_t>(1, 3);
	CPPUNIT_ASSERT(rect == IntRect(7, 4, 11, 12));

	rect.changeSize(5, 6);
	CPPUNIT_ASSERT(rect == IntRect(7, 4, 16, 18));

	rect.changeSize(Geometry::_Vec2<int32_t>(-5, -3));
	CPPUNIT_ASSERT(rect == IntRect(7, 4, 11, 15));

	rect.changeSizeCentered(2, 4);
	CPPUNIT_ASSERT(rect == IntRect(6, 2, 13, 19));

	rect.changeSizeCentered(Geometry::_Vec2<int32_t>(6, 8));
	CPPUNIT_ASSERT(rect == IntRect(3, -2, 19, 27));

	CPPUNIT_ASSERT(IntRect(17, 25, -5, 0).include(2, 3) == IntRect(2, 3, 0, 0));
	CPPUNIT_ASSERT(IntRect(17, 25, 3, 4).include(17, 25) == IntRect(17, 25, 3, 4));
	CPPUNIT_ASSERT(IntRect(17, 25, 3, 4).include(16, 25) == IntRect(16, 25, 4, 4));
	CPPUNIT_ASSERT(IntRect(17, 25, 3, 4).include(21, 25) == IntRect(17, 25, 4, 4));
	CPPUNIT_ASSERT(IntRect(17, 25, 3, 4).include(17, 24) == IntRect(17, 24, 3, 5));
	CPPUNIT_ASSERT(IntRect(17, 25, 3, 4).include(17, 30) == IntRect(17, 25, 3, 5));
	CPPUNIT_ASSERT(IntRect(17, 25, 3, 4).include(Geometry::_Vec2<int32_t>(-1, -1)) == IntRect(-1, -1, 21, 30));

	CPPUNIT_ASSERT(IntRect(17, 25, -5, 0).include(IntRect(17, 25, 3, 4)) == IntRect(17, 25, 3, 4));
	CPPUNIT_ASSERT(IntRect(17, 25, 3, 4).include(IntRect(17, 25, -5, 0)) == IntRect(17, 25, 3, 4));
	CPPUNIT_ASSERT(IntRect(17, 25, 3, 4).include(IntRect(16, 24, 4, 5)) == IntRect(16, 24, 4, 5));

	CPPUNIT_ASSERT(IntRect(0, 0, 10, 10).clipBy(IntRect(20, 20, 1, 1)).isInvalid());
	CPPUNIT_ASSERT(IntRect(0, 0, 10, 10).clipBy(IntRect(1, 0, 10, 10)) == IntRect(1, 0, 9, 10));
	CPPUNIT_ASSERT(IntRect(0, 0, 10, 10).clipBy(IntRect(0, 0, 9, 10)) == IntRect(0, 0, 9, 10));
	CPPUNIT_ASSERT(IntRect(0, 0, 10, 10).clipBy(IntRect(0, 1, 10, 10)) == IntRect(0, 1, 10, 9));
	CPPUNIT_ASSERT(IntRect(0, 0, 10, 10).clipBy(IntRect(0, 0, 10, 9)) == IntRect(0, 0, 10, 9));
}

void RectTest::testSerialization() {
	std::stringstream stream;
	for (int_fast32_t x = -10; x <= 10; ++x) {
		for (int_fast32_t y = -10; y <= 10; ++y) {
			for (int_fast32_t width = 0; width <= 10; ++width) {
				for (int_fast32_t height = 0; height <= 10; ++height) {
					stream << IntRect(x, y, width, height) << ' ';
				}
			}
		}
	}
	for (int_fast32_t x = -10; x <= 10; ++x) {
		for (int_fast32_t y = -10; y <= 10; ++y) {
			for (int_fast32_t width = 0; width <= 10; ++width) {
				for (int_fast32_t height = 0; height <= 10; ++height) {
					IntRect rect;
					stream >> rect;
					CPPUNIT_ASSERT(rect == IntRect(x, y, width, height));
				}
			}
		}
	}
}

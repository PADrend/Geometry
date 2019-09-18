/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Rect.h"
#include <sstream>
#include <stdint.h>
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))
#define REQUIRE_DOUBLES_EQUAL(a,b,e) REQUIRE((((a) <= (b) + e) && ((b) <= (a) + e)))

using IntRect = Geometry::_Rect<int32_t>;

TEST_CASE("RectTest_testConstructors", "[RectTest]") {
	REQUIRE(IntRect() == IntRect(0, 0, 0, 0));
	const IntRect source(5, 6, 10, 20);
	REQUIRE(IntRect(source) == IntRect(5, 6, 10, 20));
	REQUIRE(source == IntRect(Geometry::_Rect<float>(5.0f, 6.0f, 10.0f, 20.0f)));
	REQUIRE(source == IntRect(Geometry::_Rect<double>(5.0, 6.0, 10.0, 20.0)));
}

TEST_CASE("RectTest_testOperators", "[RectTest]") {
	REQUIRE(IntRect() == IntRect());
	REQUIRE(!(IntRect(1, 0, 0, 0) == IntRect(0, 0, 0, 0)));
	REQUIRE(!(IntRect(0, 1, 0, 0) == IntRect(0, 0, 0, 0)));
	REQUIRE(!(IntRect(0, 0, 1, 0) == IntRect(0, 0, 0, 0)));
	REQUIRE(!(IntRect(0, 0, 0, 1) == IntRect(0, 0, 0, 0)));
	REQUIRE(!(IntRect(0, 0, 0, 0) == IntRect(1, 0, 0, 0)));
	REQUIRE(!(IntRect(0, 0, 0, 0) == IntRect(0, 1, 0, 0)));
	REQUIRE(!(IntRect(0, 0, 0, 0) == IntRect(0, 0, 1, 0)));
	REQUIRE(!(IntRect(0, 0, 0, 0) == IntRect(0, 0, 0, 1)));

	REQUIRE(!(IntRect() != IntRect()));
	REQUIRE(IntRect(1, 0, 0, 0) != IntRect(0, 0, 0, 0));
	REQUIRE(IntRect(0, 1, 0, 0) != IntRect(0, 0, 0, 0));
	REQUIRE(IntRect(0, 0, 1, 0) != IntRect(0, 0, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 1) != IntRect(0, 0, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) != IntRect(1, 0, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) != IntRect(0, 1, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) != IntRect(0, 0, 1, 0));
	REQUIRE(IntRect(0, 0, 0, 0) != IntRect(0, 0, 0, 1));

	REQUIRE(IntRect(0, 0, 0, 0) + Geometry::_Vec2<int32_t>(5, 5) == IntRect(5, 5, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) + Geometry::_Vec2<int32_t>(-5, 5) == IntRect(-5, 5, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) + Geometry::_Vec2<int32_t>(5, -5) == IntRect(5, -5, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) + Geometry::_Vec2<int32_t>(-5, -5) == IntRect(-5, -5, 0, 0));

	REQUIRE(IntRect(0, 0, 0, 0) - Geometry::_Vec2<int32_t>(5, 5) == IntRect(-5, -5, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) - Geometry::_Vec2<int32_t>(-5, 5) == IntRect(5, -5, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) - Geometry::_Vec2<int32_t>(5, -5) == IntRect(-5, 5, 0, 0));
	REQUIRE(IntRect(0, 0, 0, 0) - Geometry::_Vec2<int32_t>(-5, -5) == IntRect(5, 5, 0, 0));
}

TEST_CASE("RectTest_testGetters", "[RectTest]") {
	const IntRect rectA(5, 6, 10, 20);
	const IntRect rectB(2, -3, 7, 2);
	const IntRect rectC(-20, -10, 2, 5);

	REQUIRE_EQUAL(rectA.getX(), 5);
	REQUIRE_EQUAL(rectB.getX(), 2);
	REQUIRE_EQUAL(rectC.getX(), -20);
	REQUIRE_EQUAL(rectA.getX(), rectA.getMinX());
	REQUIRE_EQUAL(rectB.getX(), rectB.getMinX());
	REQUIRE_EQUAL(rectC.getX(), rectC.getMinX());
	REQUIRE_EQUAL(rectA.getMaxX(), 15);
	REQUIRE_EQUAL(rectB.getMaxX(), 9);
	REQUIRE_EQUAL(rectC.getMaxX(), -18);
	REQUIRE_EQUAL(rectA.getY(), 6);
	REQUIRE_EQUAL(rectB.getY(), -3);
	REQUIRE_EQUAL(rectC.getY(), -10);
	REQUIRE_EQUAL(rectA.getY(), rectA.getMinY());
	REQUIRE_EQUAL(rectB.getY(), rectB.getMinY());
	REQUIRE_EQUAL(rectC.getY(), rectC.getMinY());
	REQUIRE_EQUAL(rectA.getMaxY(), 26);
	REQUIRE_EQUAL(rectB.getMaxY(), -1);
	REQUIRE_EQUAL(rectC.getMaxY(), -5);
	REQUIRE_EQUAL(rectA.getWidth(), 10);
	REQUIRE_EQUAL(rectB.getWidth(), 7);
	REQUIRE_EQUAL(rectC.getWidth(), 2);
	REQUIRE_EQUAL(rectA.getHeight(), 20);
	REQUIRE_EQUAL(rectB.getHeight(), 2);
	REQUIRE_EQUAL(rectC.getHeight(), 5);
	REQUIRE_EQUAL(rectA.getPosition(), Geometry::_Vec2<int32_t>(5, 6));
	REQUIRE_EQUAL(rectB.getPosition(), Geometry::_Vec2<int32_t>(2, -3));
	REQUIRE_EQUAL(rectC.getPosition(), Geometry::_Vec2<int32_t>(-20, -10));
	REQUIRE_EQUAL(rectA.getCenter(), Geometry::_Vec2<int32_t>(10, 16));
	REQUIRE_EQUAL(rectB.getCenter(), Geometry::_Vec2<int32_t>(5, -2));
	REQUIRE_EQUAL(rectC.getCenter(), Geometry::_Vec2<int32_t>(-19, -7));

	REQUIRE(rectA.contains(rectA.getMinX(), rectA.getMinY()));
	REQUIRE(rectA.contains(rectA.getMaxX(), rectA.getMinY()));
	REQUIRE(rectA.contains(rectA.getMinX(), rectA.getMaxY()));
	REQUIRE(rectA.contains(rectA.getMaxX(), rectA.getMaxY()));
	REQUIRE(rectB.contains(rectB.getMinX(), rectB.getMinY()));
	REQUIRE(rectB.contains(rectB.getMaxX(), rectB.getMinY()));
	REQUIRE(rectB.contains(rectB.getMinX(), rectB.getMaxY()));
	REQUIRE(rectB.contains(rectB.getMaxX(), rectB.getMaxY()));
	REQUIRE(rectC.contains(rectC.getMinX(), rectC.getMinY()));
	REQUIRE(rectC.contains(rectC.getMaxX(), rectC.getMinY()));
	REQUIRE(rectC.contains(rectC.getMinX(), rectC.getMaxY()));
	REQUIRE(rectC.contains(rectC.getMaxX(), rectC.getMaxY()));

	REQUIRE(!rectA.contains(rectA.getMinX() - 1, rectA.getMinY()));
	REQUIRE(!rectA.contains(rectA.getMinX(), rectA.getMinY() - 1));
	REQUIRE(!rectA.contains(rectA.getMaxX() + 1, rectA.getMinY()));
	REQUIRE(!rectA.contains(rectA.getMaxX(), rectA.getMinY() - 1));
	REQUIRE(!rectA.contains(rectA.getMinX() - 1, rectA.getMaxY()));
	REQUIRE(!rectA.contains(rectA.getMinX(), rectA.getMaxY() + 1));
	REQUIRE(!rectA.contains(rectA.getMaxX() + 1, rectA.getMaxY()));
	REQUIRE(!rectA.contains(rectA.getMaxX(), rectA.getMaxY() + 1));

	REQUIRE(rectA.contains(rectA));
	REQUIRE(rectB.contains(rectB));
	REQUIRE(rectC.contains(rectC));

	REQUIRE(rectA.contains(IntRect(10, 16, 1, 1)));
	REQUIRE(rectB.contains(IntRect(5, -2, 1, 1)));
	REQUIRE(rectC.contains(IntRect(-19, -7, 1, 1)));

	REQUIRE(!rectA.contains(IntRect(rectA.getMinX() - 1, rectA.getMinY(), rectA.getWidth(), rectA.getHeight())));
	REQUIRE(!rectA.contains(IntRect(rectA.getMinX(), rectA.getMinY() - 1, rectA.getWidth(), rectA.getHeight())));
	REQUIRE(!rectA.contains(IntRect(rectA.getMinX(), rectA.getMinY(), rectA.getWidth() + 1, rectA.getHeight())));
	REQUIRE(!rectA.contains(IntRect(rectA.getMinX(), rectA.getMinY(), rectA.getWidth(), rectA.getHeight() + 1)));

	REQUIRE(rectA.contains(rectA.getPosition()));
	REQUIRE(rectB.contains(rectB.getPosition()));
	REQUIRE(rectC.contains(rectC.getPosition()));

	REQUIRE(rectA.contains(rectA.getCenter()));
	REQUIRE(rectB.contains(rectB.getCenter()));
	REQUIRE(rectC.contains(rectC.getCenter()));

	REQUIRE(!rectA.isInvalid());
	REQUIRE(!rectB.isInvalid());
	REQUIRE(!rectC.isInvalid());

	REQUIRE(!IntRect(0, 0, 0, 0).isInvalid());
	REQUIRE(IntRect(0, 0, 0, -1).isInvalid());
	REQUIRE(IntRect(0, 0, -1, 0).isInvalid());
	REQUIRE(IntRect(0, 0, -1, -1).isInvalid());

	REQUIRE(rectA.isValid());
	REQUIRE(rectB.isValid());
	REQUIRE(rectC.isValid());

	REQUIRE(IntRect(0, 0, 0, 0).isValid());
	REQUIRE(!IntRect(0, 0, 0, -1).isValid());
	REQUIRE(!IntRect(0, 0, -1, 0).isValid());
	REQUIRE(!IntRect(0, 0, -1, -1).isValid());

	REQUIRE(IntRect().intersects(IntRect()));
	REQUIRE(IntRect(0, 0, 10, 10).intersects(IntRect(5, 5, 1, 1)));
	REQUIRE(IntRect(0, 0, 10, 10).intersects(IntRect(10, 10, 1, 1)));
	REQUIRE(IntRect(0, 0, 10, 10).intersects(IntRect(-1, -1, 1, 1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(5, 5, -1, -1)));
	REQUIRE(!IntRect(5, 5, -1, -1).intersects(IntRect(0, 0, 10, 10)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(5, 5, -1, -1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(-2, -2, 1, 1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(11, -2, 1, 1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(-2, 11, 1, 1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(11, 11, 1, 1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(5, -2, 1, 1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(5, 11, 1, 1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(-2, 5, 1, 1)));
	REQUIRE(!IntRect(0, 0, 10, 10).intersects(IntRect(11, 5, 1, 1)));
}

TEST_CASE("RectTest_testSetters", "[RectTest]") {
	IntRect rect(0, 0, 3, 4);
	rect.setPosition(5, 6);
	REQUIRE(rect == IntRect(5, 6, 3, 4));
	rect.setPosition(Geometry::_Vec2<int32_t>(7, 8));
	REQUIRE(rect == IntRect(7, 8, 3, 4));
	rect.setX(9);
	REQUIRE(rect == IntRect(9, 8, 3, 4));
	rect.setY(10);
	REQUIRE(rect == IntRect(9, 10, 3, 4));
	rect.setWidth(1);
	REQUIRE(rect == IntRect(9, 10, 1, 4));
	rect.setHeight(2);
	REQUIRE(rect == IntRect(9, 10, 1, 2));

	REQUIRE(rect.isValid());
	rect.invalidate();
	REQUIRE(rect.isInvalid());

	rect.setSize(11, 12);
	REQUIRE(rect == IntRect(9, 10, 11, 12));

	rect.moveRel(-1, -3);
	REQUIRE(rect == IntRect(8, 7, 11, 12));

	rect.moveRel(Geometry::_Vec2<int32_t>(-1, -3));
	REQUIRE(rect == IntRect(7, 4, 11, 12));

	rect += Geometry::_Vec2<int32_t>(1, 3);
	REQUIRE(rect == IntRect(8, 7, 11, 12));

	rect -= Geometry::_Vec2<int32_t>(1, 3);
	REQUIRE(rect == IntRect(7, 4, 11, 12));

	rect.changeSize(5, 6);
	REQUIRE(rect == IntRect(7, 4, 16, 18));

	rect.changeSize(Geometry::_Vec2<int32_t>(-5, -3));
	REQUIRE(rect == IntRect(7, 4, 11, 15));

	rect.changeSizeCentered(2, 4);
	REQUIRE(rect == IntRect(6, 2, 13, 19));

	rect.changeSizeCentered(Geometry::_Vec2<int32_t>(6, 8));
	REQUIRE(rect == IntRect(3, -2, 19, 27));

	REQUIRE(IntRect(17, 25, -5, 0).include(2, 3) == IntRect(2, 3, 0, 0));
	REQUIRE(IntRect(17, 25, 3, 4).include(17, 25) == IntRect(17, 25, 3, 4));
	REQUIRE(IntRect(17, 25, 3, 4).include(16, 25) == IntRect(16, 25, 4, 4));
	REQUIRE(IntRect(17, 25, 3, 4).include(21, 25) == IntRect(17, 25, 4, 4));
	REQUIRE(IntRect(17, 25, 3, 4).include(17, 24) == IntRect(17, 24, 3, 5));
	REQUIRE(IntRect(17, 25, 3, 4).include(17, 30) == IntRect(17, 25, 3, 5));
	REQUIRE(IntRect(17, 25, 3, 4).include(Geometry::_Vec2<int32_t>(-1, -1)) == IntRect(-1, -1, 21, 30));

	REQUIRE(IntRect(17, 25, -5, 0).include(IntRect(17, 25, 3, 4)) == IntRect(17, 25, 3, 4));
	REQUIRE(IntRect(17, 25, 3, 4).include(IntRect(17, 25, -5, 0)) == IntRect(17, 25, 3, 4));
	REQUIRE(IntRect(17, 25, 3, 4).include(IntRect(16, 24, 4, 5)) == IntRect(16, 24, 4, 5));

	REQUIRE(IntRect(0, 0, 10, 10).clipBy(IntRect(20, 20, 1, 1)).isInvalid());
	REQUIRE(IntRect(0, 0, 10, 10).clipBy(IntRect(1, 0, 10, 10)) == IntRect(1, 0, 9, 10));
	REQUIRE(IntRect(0, 0, 10, 10).clipBy(IntRect(0, 0, 9, 10)) == IntRect(0, 0, 9, 10));
	REQUIRE(IntRect(0, 0, 10, 10).clipBy(IntRect(0, 1, 10, 10)) == IntRect(0, 1, 10, 9));
	REQUIRE(IntRect(0, 0, 10, 10).clipBy(IntRect(0, 0, 10, 9)) == IntRect(0, 0, 10, 9));
}

TEST_CASE("RectTest_testSerialization", "[RectTest]") {
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
					REQUIRE(rect == IntRect(x, y, width, height));
				}
			}
		}
	}
}

/*
	This file is part of the Geometry library.
	Copyright (C) 2010-2011 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2011 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Line.h"
#include "Tools.h"
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))

TEST_CASE("LineTest_testGetters", "[LineTest]") {
	Geometry::Line3f lineX(Geometry::Vec3f(0.0f, 0.0f, 0.0f), Geometry::Vec3f(1.0f, 0.0f, 0.0f));
	Geometry::Line3f lineY(Geometry::Vec3f(0.0f, 3.0f, 0.0f), Geometry::Vec3f(0.0f, -1.0f, 0.0f));

	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineX.getOrigin());
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 3.0f, 0.0f), lineY.getOrigin());

	REQUIRE_EQUAL(Geometry::Vec3f(1.0f, 0.0f, 0.0f), lineX.getDirection());
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, -1.0f, 0.0f), lineY.getDirection());

	REQUIRE_EQUAL(lineX.getOrigin(), lineX.getPoint(0.0f));
	REQUIRE_EQUAL(lineX.getOrigin() + lineX.getDirection(), lineX.getPoint(1.0f));
	REQUIRE_EQUAL(lineX.getOrigin() - lineX.getDirection(), lineX.getPoint(-1.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(100.0f, 0.0f, 0.0f), lineX.getPoint(100.0f));

	REQUIRE_EQUAL(lineY.getOrigin(), lineY.getPoint(0.0f));
	REQUIRE_EQUAL(lineY.getOrigin() + lineY.getDirection() * 2.0f, lineY.getPoint(2.0f));
	REQUIRE_EQUAL(lineY.getOrigin() - lineY.getDirection() * 2.0f, lineY.getPoint(-2.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, -4.0f, 0.0f), lineY.getPoint(7.0f));

	REQUIRE_EQUAL(0.0f, lineX.distance(Geometry::Vec3f(0.0f, 0.0f, 0.0f)));
	REQUIRE_EQUAL(0.0f, lineX.distance(Geometry::Vec3f(1000.0f, 0.0f, 0.0f)));
	REQUIRE_EQUAL(0.0f, lineX.distance(Geometry::Vec3f(-1000.0f, 0.0f, 0.0f)));

	REQUIRE_EQUAL(0.0f, lineY.distance(Geometry::Vec3f(0.0f, 0.0f, 0.0f)));
	REQUIRE_EQUAL(0.0f, lineY.distance(Geometry::Vec3f(0.0f, 1000.0f, 0.0f)));
	REQUIRE_EQUAL(0.0f, lineY.distance(Geometry::Vec3f(0.0f, -1000.0f, 0.0f)));

	REQUIRE_EQUAL(1.0f, lineX.distance(Geometry::Vec3f(0.0f, 1.0f, 0.0f)));
	REQUIRE_EQUAL(1.0f, lineX.distance(Geometry::Vec3f(0.0f, 0.0f, 1.0f)));
	REQUIRE_EQUAL(1.0f, lineX.distance(Geometry::Vec3f(0.0f, -1.0f, 0.0f)));
	REQUIRE_EQUAL(1.0f, lineX.distance(Geometry::Vec3f(0.0f, 0.0f, -1.0f)));

	REQUIRE_EQUAL(1.0f, lineY.distance(Geometry::Vec3f(1.0f, 0.0f, 0.0f)));
	REQUIRE_EQUAL(1.0f, lineY.distance(Geometry::Vec3f(0.0f, 0.0f, 1.0f)));
	REQUIRE_EQUAL(1.0f, lineY.distance(Geometry::Vec3f(-1.0f, 0.0f, 0.0f)));
	REQUIRE_EQUAL(1.0f, lineY.distance(Geometry::Vec3f(0.0f, 0.0f, -1.0f)));

	REQUIRE_EQUAL(std::sqrt(2.0f), lineX.distance(Geometry::Vec3f(0.0f, 1.0f, -1.0f)));
	REQUIRE_EQUAL(lineX.distance(Geometry::Vec3f(0.0f, -100.0f, 100.0f)), 100.0f * std::sqrt(2.0f));

	REQUIRE_EQUAL(std::sqrt(2.0f), lineY.distance(Geometry::Vec3f(1.0f, 0.0f, -1.0f)));
	REQUIRE_EQUAL(lineY.distance(Geometry::Vec3f(-100.0f, 0.0f, 100.0f)), 100.0f * std::sqrt(2.0f));

	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineX.getClosestPoint(Geometry::Vec3f(0.0f, 0.0f, 0.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(1000.0f, 0.0f, 0.0f),
						 lineX.getClosestPoint(Geometry::Vec3f(1000.0f, 0.0f, 0.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(-1000.0f, 0.0f, 0.0f),
						 lineX.getClosestPoint(Geometry::Vec3f(-1000.0f, 0.0f, 0.0f)));

	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineY.getClosestPoint(Geometry::Vec3f(0.0f, 0.0f, 0.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 1000.0f, 0.0f),
						 lineY.getClosestPoint(Geometry::Vec3f(0.0f, 1000.0f, 0.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, -1000.0f, 0.0f),
						 lineY.getClosestPoint(Geometry::Vec3f(0.0f, -1000.0f, 0.0f)));

	REQUIRE_EQUAL(Geometry::Vec3f(5.0f, 0.0f, 0.0f), lineX.getClosestPoint(Geometry::Vec3f(5.0f, 1.0f, 0.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(5.0f, 0.0f, 0.0f), lineX.getClosestPoint(Geometry::Vec3f(5.0f, 100.0f, 0.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(-5.0f, 0.0f, 0.0f),
						 lineX.getClosestPoint(Geometry::Vec3f(-5.0f, 100.0f, 0.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(-5.0f, 0.0f, 0.0f),
						 lineX.getClosestPoint(Geometry::Vec3f(-5.0f, -100.0f, 0.0f)));

	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 5.0f, 0.0f), lineY.getClosestPoint(Geometry::Vec3f(0.0f, 5.0f, 1.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 5.0f, 0.0f), lineY.getClosestPoint(Geometry::Vec3f(0.0f, 5.0f, 100.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, -5.0f, 0.0f),
						 lineY.getClosestPoint(Geometry::Vec3f(0.0f, -5.0f, 100.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, -5.0f, 0.0f),
						 lineY.getClosestPoint(Geometry::Vec3f(0.0f, -5.0f, -100.0f)));

	REQUIRE_EQUAL(Geometry::Vec3f(10.0f, 0.0f, 0.0f), lineX.getClosestPoint(Geometry::Vec3f(10.0f, 1.0f, 1.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(10.0f, 0.0f, 0.0f),
						 lineX.getClosestPoint(Geometry::Vec3f(10.0f, 1.0f, -1.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(10.0f, 0.0f, 0.0f),
						 lineX.getClosestPoint(Geometry::Vec3f(10.0f, -1.0f, 1.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(10.0f, 0.0f, 0.0f),
						 lineX.getClosestPoint(Geometry::Vec3f(10.0f, -1.0f, -1.0f)));

	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 10.0f, 0.0f), lineY.getClosestPoint(Geometry::Vec3f(1.0f, 10.0f, 1.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 10.0f, 0.0f),
						 lineY.getClosestPoint(Geometry::Vec3f(1.0f, 10.0f, -1.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 10.0f, 0.0f),
						 lineY.getClosestPoint(Geometry::Vec3f(-1.0f, 10.0f, 1.0f)));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 10.0f, 0.0f),
						 lineY.getClosestPoint(Geometry::Vec3f(-1.0f, 10.0f, -1.0f)));
}

TEST_CASE("LineTest_testSetters", "[LineTest]") {
	Geometry::Line3f lineX(Geometry::Vec3f(0.0f, 0.0f, 0.0f), Geometry::Vec3f(1.0f, 0.0f, 0.0f));
	Geometry::Line3f lineY(Geometry::Vec3f(0.0f, 3.0f, 0.0f), Geometry::Vec3f(0.0f, -1.0f, 0.0f));

	lineY.setOrigin(Geometry::Vec3f(0.0f, 100.0f, 0.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 100.0f, 0.0f), lineY.getPoint(0.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineY.getPoint(100.0f));

	lineY.setDirection(Geometry::Vec3f(0.0f, 1.0f, 0.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 100.0f, 0.0f), lineY.getPoint(0.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineY.getPoint(-100.0f));

	lineY.setOrigin(Geometry::Vec3f(0.0f, 100.0f, 5.0f));
	REQUIRE_EQUAL(5.0f, lineY.distance(Geometry::Vec3f(0.0f, 0.0f, 0.0f)));
}

TEST_CASE("LineTest_testMisc", "[LineTest]") {
	Geometry::Line3f lineX(Geometry::Vec3f(0.0f, 0.0f, 0.0f), Geometry::Vec3f(1.0f, 0.0f, 0.0f));
	Geometry::Line3f lineY(Geometry::Vec3f(0.0f, 3.0f, 0.0f), Geometry::Vec3f(0.0f, -1.0f, 0.0f));

	float s;
	float t;

	lineY.setDirection(Geometry::Vec3f(0.0f, 1.0f, 0.0f));

	lineY.setOrigin(Geometry::Vec3f(0.0f, 100.0f, 5.0f));
	Geometry::getClosestPoints(lineX, lineY, s, t);
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineX.getPoint(s));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 5.0f), lineY.getPoint(t));

	lineY.setOrigin(Geometry::Vec3f(5.0f, 100.0f, 5.0f));
	Geometry::getClosestPoints(lineX, lineY, s, t);
	REQUIRE_EQUAL(Geometry::Vec3f(5.0f, 0.0f, 0.0f), lineX.getPoint(s));
	REQUIRE_EQUAL(Geometry::Vec3f(5.0f, 0.0f, 5.0f), lineY.getPoint(t));

	lineY.setDirection(Geometry::Vec3f(1.0f, 1.0f, 1.0f).normalize());
	lineY.setOrigin(Geometry::Vec3f(0.0f, 0.0f, 0.0f));
	Geometry::getClosestPoints(lineX, lineY, s, t);
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineX.getPoint(s));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineY.getPoint(t));

	lineY.setDirection(Geometry::Vec3f(0.0f, 1.0f, -1.0f).normalize());
	lineY.setOrigin(Geometry::Vec3f(0.0f, 0.0f, -100.0f));
	Geometry::getClosestPoints(lineX, lineY, s, t);
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineX.getPoint(s));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, -50.0f, -50.0f), lineY.getPoint(t));

	// Make sure lineX was never changed.
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), lineX.getOrigin());
	REQUIRE_EQUAL(Geometry::Vec3f(1.0f, 0.0f, 0.0f), lineX.getDirection());
}

TEST_CASE("LineTest_testBounds", "[LineTest]") {
	Geometry::Ray3f ray(Geometry::Vec3f(1.0f, 1.0f, 1.0f), Geometry::Vec3f(-1.0f, -1.0f, -1.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(1.0f, 1.0f, 1.0f), ray.getPoint(-1.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(1.0f, 1.0f, 1.0f), ray.getPoint(0.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), ray.getPoint(1.0f));

	Geometry::Segment3f segmentA(Geometry::Vec3f(0.0f, 0.0f, 0.0f), Geometry::Vec3f(0.0f, 10.0f, 0.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), segmentA.getPoint(-1.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 0.0f, 0.0f), segmentA.getPoint(0.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 1.0f, 0.0f), segmentA.getPoint(1.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 9.0f, 0.0f), segmentA.getPoint(9.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 10.0f, 0.0f), segmentA.getPoint(10.0f));
	REQUIRE_EQUAL(Geometry::Vec3f(0.0f, 10.0f, 0.0f), segmentA.getPoint(11.0f));
}

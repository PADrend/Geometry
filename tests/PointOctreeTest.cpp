/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Point.h"
#include "PointOctree.h"
#include <cstdint>
#include <deque>
#include <random>
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))
#define REQUIRE_DOUBLES_EQUAL(a,b,e) REQUIRE((((a) <= (b) + e) && ((b) <= (a) + e)))

struct CharPoint : public Geometry::Point<Geometry::Vec3f> {
	char data;

	CharPoint(const Geometry::Vec3f & pos, char character) : Geometry::Point<Geometry::Vec3f>(pos), data(character) {
	}
};

TEST_CASE("PointOctreeTest_test", "[PointOctreeTest]") {
	using namespace Geometry;

	{
		PointOctree<CharPoint> octree(Box(Vec3f(0.0f, 0.0f, 0.0f), 2.0f), 0.2f, 1);

		octree.insert(CharPoint(Vec3f(0.15f, 0.5f, 0.5f), 'a'));
		octree.insert(CharPoint(Vec3f(0.1f, 0.5f, 0.5f), 'b'));
		octree.insert(CharPoint(Vec3f(-0.1f, 0.5f, 0.5f), 'c'));
		octree.insert(CharPoint(Vec3f(-0.15f, 0.5f, 0.5f), 'd'));

		octree.insert(CharPoint(Vec3f(-0.6f, -0.6f, -0.6f), 'e'));
		octree.insert(CharPoint(Vec3f(-0.4f, -0.6f, -0.6f), 'f'));
		octree.insert(CharPoint(Vec3f(-0.6f, -0.4f, -0.6f), 'g'));
		octree.insert(CharPoint(Vec3f(-0.4f, -0.4f, -0.6f), 'h'));
		octree.insert(CharPoint(Vec3f(-0.6f, -0.6f, -0.4f), 'i'));
		octree.insert(CharPoint(Vec3f(-0.4f, -0.6f, -0.4f), 'j'));
		octree.insert(CharPoint(Vec3f(-0.6f, -0.4f, -0.4f), 'k'));
		octree.insert(CharPoint(Vec3f(-0.4f, -0.4f, -0.4f), 'l'));

		{
			std::deque<CharPoint> points;
			octree.collectPointsWithinSphere(Sphere_f(Vec3f(0.0f, 0.5f, 0.5f), 0.125f), points);
			uint_fast32_t foundB = 0;
			uint_fast32_t foundC = 0;
			for (auto point : points) {
				if (point.data == 'b') {
					++foundB;
				}
				if (point.data == 'c') {
					++foundC;
				}
			}
			REQUIRE_EQUAL(static_cast<uint_fast32_t>(1), foundB);
			REQUIRE_EQUAL(static_cast<uint_fast32_t>(1), foundC);
		}
		{
			const float distance = std::sqrt(0.03f);
			{
				std::deque<CharPoint> points;
				octree.collectPointsWithinSphere(
						Sphere_f(Vec3f(-0.5f, -0.5f, -0.5f), distance - std::numeric_limits<float>::epsilon()), points);
				REQUIRE(points.empty());
			}
			{
				std::deque<CharPoint> points;
				octree.collectPointsWithinSphere(
						Sphere_f(Vec3f(-0.5f, -0.5f, -0.5f), distance + std::numeric_limits<float>::epsilon()), points);
				REQUIRE_EQUAL(static_cast<std::size_t>(8), points.size());
			}
		}
		{
			CharPoint p(Vec3f(), 'X');
			octree.insert(p);
			std::deque<CharPoint> points;
			octree.collectPointsWithinSphere(Sphere_f({}, std::numeric_limits<float>::epsilon()), points);
			REQUIRE_EQUAL(static_cast<std::size_t>(1), points.size());
			points.clear();
			REQUIRE(octree.remove(p));
			octree.collectPointsWithinSphere(Sphere_f({}, std::numeric_limits<float>::epsilon()), points);
			REQUIRE(points.empty());
		}
		{
			std::deque<CharPoint> points;
			std::deque<CharPoint> point;
			octree.collectPoints(points);
			for(uint32_t i=0; i<points.size(); ++i) {
				Sphere_f s(points[i].getPosition(), std::numeric_limits<float>::epsilon());
				point.clear(); octree.collectPointsWithinSphere(s, point); REQUIRE(!point.empty());
				REQUIRE(octree.remove(points[i]));
				point.clear(); octree.collectPointsWithinSphere(s, point); REQUIRE(point.empty());
				for(uint32_t j=0; j<=i; ++j) {
					REQUIRE(!octree.remove(points[i]));
				}
			}
			REQUIRE(octree.empty());
		}
	}

	{
		PointOctree<CharPoint> octree(Box(-1.0f, 3.0f, -2.0f, 2.0f, -3.0f, 1.0f), 0.1f, 10);

		const Vec3f sphereCenter(0.0f, -1.0f, -2.0f);
		const Sphere_f sphere(sphereCenter, 0.8f);

		std::default_random_engine engine;
		std::uniform_real_distribution<float> xDist(sphereCenter.getX() - 1.0f, sphereCenter.getX() + 1.0f);
		std::uniform_real_distribution<float> yDist(sphereCenter.getY() - 1.0f, sphereCenter.getY() + 1.0f);
		std::uniform_real_distribution<float> zDist(sphereCenter.getZ() - 1.0f, sphereCenter.getZ() + 1.0f);

		// Generate random points.
		for (uint_fast32_t i = 0; i < 1000000; ++i) {
			const float x = xDist(engine);
			const float y = yDist(engine);
			const float z = zDist(engine);
			const Vec3f point(x, y, z);
			// Check if point is inside ('i') or outside ('o') of sphere
			if (sphere.isOutside(point)) {
				REQUIRE(octree.insert(CharPoint(point, 'o')));
			} else {
				REQUIRE(octree.insert(CharPoint(point, 'i')));
			}
		}

		std::deque<CharPoint> points;
		octree.collectPointsWithinSphere(sphere, points);
		for (auto point : points) {
			REQUIRE_EQUAL('i', point.data);
		}
	}
}

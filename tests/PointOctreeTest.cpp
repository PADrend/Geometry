/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2012 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "PointOctreeTest.h"
#include "Point.h"
#include "PointOctree.h"
#include <cstdint>
#include <deque>
#include <random>
CPPUNIT_TEST_SUITE_REGISTRATION(PointOctreeTest);

struct CharPoint : public Geometry::Point<Geometry::Vec3f> {
	char data;

	CharPoint(const Geometry::Vec3f & pos, char character) : Geometry::Point<Geometry::Vec3f>(pos), data(character) {
	}
};

void PointOctreeTest::test() {
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
			CPPUNIT_ASSERT_EQUAL(static_cast<uint_fast32_t>(1), foundB);
			CPPUNIT_ASSERT_EQUAL(static_cast<uint_fast32_t>(1), foundC);
		}
		{
			const float distance = std::sqrt(0.03f);
			{
				std::deque<CharPoint> points;
				octree.collectPointsWithinSphere(
						Sphere_f(Vec3f(-0.5f, -0.5f, -0.5f), distance - std::numeric_limits<float>::epsilon()), points);
				CPPUNIT_ASSERT(points.empty());
			}
			{
				std::deque<CharPoint> points;
				octree.collectPointsWithinSphere(
						Sphere_f(Vec3f(-0.5f, -0.5f, -0.5f), distance + std::numeric_limits<float>::epsilon()), points);
				CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(8), points.size());
			}
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
				CPPUNIT_ASSERT(octree.insert(CharPoint(point, 'o')));
			} else {
				CPPUNIT_ASSERT(octree.insert(CharPoint(point, 'i')));
			}
		}

		std::deque<CharPoint> points;
		octree.collectPointsWithinSphere(sphere, points);
		for (auto point : points) {
			CPPUNIT_ASSERT_EQUAL('i', point.data);
		}
	}
}

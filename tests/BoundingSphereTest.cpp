/*
	This file is part of the Geometry library.
	Copyright (C) 2012-2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "BoundingSphereTest.h"
#include "BoundingSphere.h"
#include "Box.h"
#include "Sphere.h"
#include "Vec3.h"
#include <random>

CPPUNIT_TEST_SUITE_REGISTRATION(BoundingSphereTest);

using namespace Geometry;

void BoundingSphereTest::testRandomPoints() {
	const float coordinateRange = 1000.0f;
	std::uniform_real_distribution<float> coordinateDist(-coordinateRange, coordinateRange);
	const unsigned int count = 100000;
	const float maxRadius = std::sqrt(3.0f) * coordinateRange;
	const float epsilon = (1.0f / coordinateRange);

	std::vector<Vec3f> points;
	points.reserve(count);

	for (unsigned int seed = 0; seed < 10; ++seed) {
		std::default_random_engine engine(seed);

		points.clear();
		for (unsigned int i = 0; i < count; ++i) {
			points.emplace_back(coordinateDist(engine), coordinateDist(engine), coordinateDist(engine));
		}

		{
			Box boundingBox;
			boundingBox.invalidate();
			for (const auto & point : points) {
				boundingBox.include(point);
			}

			const Sphere_f boundingSphere(boundingBox.getCenter(), boundingBox.getBoundingSphereRadius());
			CPPUNIT_ASSERT(boundingSphere.getRadius() < maxRadius);
			for (const auto & point : points) {
				CPPUNIT_ASSERT(boundingSphere.distance(point) < epsilon);
			}
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeMiniball(points);
			CPPUNIT_ASSERT(boundingSphere.getRadius() < maxRadius);
			for (const auto & point : points) {
				CPPUNIT_ASSERT(boundingSphere.distance(point) < epsilon);
			}
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeEPOS6(points);
			CPPUNIT_ASSERT(boundingSphere.getRadius() < 1.2f * maxRadius);
			for (const auto & point : points) {
				CPPUNIT_ASSERT(boundingSphere.distance(point) < epsilon);
			}
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeEPOS14(points);
			CPPUNIT_ASSERT(boundingSphere.getRadius() < maxRadius);
			for (const auto & point : points) {
				CPPUNIT_ASSERT(boundingSphere.distance(point) < epsilon);
			}
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeEPOS26(points);
			CPPUNIT_ASSERT(boundingSphere.getRadius() < maxRadius);
			for (const auto & point : points) {
				CPPUNIT_ASSERT(boundingSphere.distance(point) < epsilon);
			}
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeEPOS98(points);
			CPPUNIT_ASSERT(boundingSphere.getRadius() < maxRadius);
			for (const auto & point : points) {
				CPPUNIT_ASSERT(boundingSphere.distance(point) < epsilon);
			}
		}
	}
}

void BoundingSphereTest::testRandomSpheres() {
	const float coordinateRange = 1000.0f;
	std::uniform_real_distribution<float> coordinateDist(-coordinateRange, coordinateRange);
	std::uniform_real_distribution<float> radiusDist(1.0f, 100.0f);

	const float PI = 3.14159265358979323846f;
	std::uniform_real_distribution<float> inclinationDist(0.0f, PI);
	std::uniform_real_distribution<float> azimuthDist(0.0f, 2.0f * PI);

	// Use enough points to make sure that the sphere gets points nearly everywhere on its surface
	const unsigned int count = 100;
	const float epsilon = 1.0e-3;

	std::vector<Vec3f> points;
	points.reserve(count);

	for (unsigned int seed = 0; seed < 10000; ++seed) {
		std::default_random_engine engine(seed);

		const Sphere_f randomSphere(Vec3f(coordinateDist(engine), coordinateDist(engine), coordinateDist(engine)),
									radiusDist(engine));

		points.clear();
		for (unsigned int i = 0; i < count; ++i) {
			// Generate points on the sphere surface
			points.emplace_back(randomSphere.calcCartesianCoordinate(inclinationDist(engine), azimuthDist(engine)));
		}

		{
			const Sphere_f boundingSphere = BoundingSphere::computeMiniball(points);
			CPPUNIT_ASSERT(randomSphere.getCenter().distance(boundingSphere.getCenter()) < epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(randomSphere.getRadius(), boundingSphere.getRadius(), epsilon);
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeEPOS6(points);
			CPPUNIT_ASSERT(randomSphere.getCenter().distance(boundingSphere.getCenter()) < epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(randomSphere.getRadius(), boundingSphere.getRadius(), epsilon);
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeEPOS14(points);
			CPPUNIT_ASSERT(randomSphere.getCenter().distance(boundingSphere.getCenter()) < epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(randomSphere.getRadius(), boundingSphere.getRadius(), epsilon);
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeEPOS26(points);
			CPPUNIT_ASSERT(randomSphere.getCenter().distance(boundingSphere.getCenter()) < epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(randomSphere.getRadius(), boundingSphere.getRadius(), epsilon);
		}
		{
			const Sphere_f boundingSphere = BoundingSphere::computeEPOS98(points);
			CPPUNIT_ASSERT(randomSphere.getCenter().distance(boundingSphere.getCenter()) < epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(randomSphere.getRadius(), boundingSphere.getRadius(), epsilon);
		}
	}
}

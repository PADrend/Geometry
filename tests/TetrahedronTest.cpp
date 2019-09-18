/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Tetrahedron.h"
#include "Matrix3x3.h"
#include "Vec3.h"
#include "Vec4.h"
#include <iostream>
#include <random>
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))
#define REQUIRE_DOUBLES_EQUAL(a,b,e) REQUIRE((((a) <= (b) + e) && ((b) <= (a) + e)))

static Geometry::Vec3d getRandomPoint(std::default_random_engine & engine,
									  std::uniform_real_distribution<float> & dist) {
	return Geometry::Vec3d(dist(engine), dist(engine), dist(engine));
}

TEST_CASE("TetrahedronTest_testTetrahedronBarycentricCoordinates", "[TetrahedronTest]") {
	const Geometry::Vec3d a(0.0, 0.0, 0.0);
	const Geometry::Vec3d b(30.0, 0.0, 0.0);
	const Geometry::Vec3d c(0.0, 30.0, 0.0);
	const Geometry::Vec3d d(0.0, 0.0, 30.0);
	const Geometry::Tetrahedron<double> tetrahedron(a, b, c, d);

	// Check the vertices.
	REQUIRE_EQUAL(Geometry::Vec4d(1.0, 0.0, 0.0, 0.0), tetrahedron.calcBarycentricCoordinates(a).second);
	REQUIRE_EQUAL(Geometry::Vec4d(0.0, 1.0, 0.0, 0.0), tetrahedron.calcBarycentricCoordinates(b).second);
	REQUIRE_EQUAL(Geometry::Vec4d(0.0, 0.0, 1.0, 0.0), tetrahedron.calcBarycentricCoordinates(c).second);
	REQUIRE_EQUAL(Geometry::Vec4d(0.0, 0.0, 0.0, 1.0), tetrahedron.calcBarycentricCoordinates(d).second);

	// Check the middle of edges.
	REQUIRE_EQUAL(Geometry::Vec4d(0.5, 0.5, 0.0, 0.0),
						 tetrahedron.calcBarycentricCoordinates(Geometry::Vec3d(15.0, 0.0, 0.0)).second);
	REQUIRE_EQUAL(Geometry::Vec4d(0.0, 0.5, 0.5, 0.0),
						 tetrahedron.calcBarycentricCoordinates(Geometry::Vec3d(15.0, 15.0, 0.0)).second);
	REQUIRE_EQUAL(Geometry::Vec4d(0.5, 0.0, 0.5, 0.0),
						 tetrahedron.calcBarycentricCoordinates(Geometry::Vec3d(0.0, 15.0, 0.0)).second);
	REQUIRE_EQUAL(Geometry::Vec4d(0.5, 0.0, 0.0, 0.5),
						 tetrahedron.calcBarycentricCoordinates(Geometry::Vec3d(0.0, 0.0, 15.0)).second);

	std::default_random_engine engine;
	std::uniform_real_distribution<float> dist(-10, 10.0);

	const double delta = 1.0e-3; // pretty inaccurate....

	for (int i = 0; i < 1000; ++i) {
		const Geometry::Tetrahedron<double> t(getRandomPoint(engine, dist), getRandomPoint(engine, dist),
											  getRandomPoint(engine, dist), getRandomPoint(engine, dist));
		if (t.calcVolume() < 10)
			continue;

		for (int j = 0; j < 100; ++j) {
			const Geometry::Vec3d point = getRandomPoint(engine, dist);
			const auto & coordinates = t.calcBarycentricCoordinates(point);
			if (!coordinates.first) { // degenerated
				break;
			}
			REQUIRE_DOUBLES_EQUAL(0.0, t.calcPointFromBarycentricCoordinates(coordinates.second).distance(point),
										 delta);
		}
	}
}

TEST_CASE("TetrahedronTest_testTetrahedronDistance", "[TetrahedronTest]") {
	const Geometry::Vec3d a(0.0, 0.0, 0.0);
	const Geometry::Vec3d b(0.0, 30.0, 0.0);
	const Geometry::Vec3d c(0.0, 0.0, -30.0);
	const Geometry::Vec3d d(30.0, 0.0, 0.0);
	const Geometry::Tetrahedron<double> tetrahedron(a, b, c, d);

	const double delta = 1.0e-6;

	// Check the vertices.
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared(a), delta);
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared(b), delta);
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared(c), delta);
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared(d), delta);

	// Check the middle of edges.
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared((a + b) * 0.5), delta);
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared((a + c) * 0.5), delta);
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared((a + d) * 0.5), delta);
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared((b + c) * 0.5), delta);
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared((b + d) * 0.5), delta);
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared((c + d) * 0.5), delta);

	// Check the centroid.
	REQUIRE_DOUBLES_EQUAL(0.0, tetrahedron.distanceSquared((a + b + c + d) * 0.25), delta);

	// Test points left of the tetrahedron.
	REQUIRE_DOUBLES_EQUAL(200.0, tetrahedron.distanceSquared(Geometry::Vec3d(-10.0, -10.0, 0.0)), delta);
	REQUIRE_DOUBLES_EQUAL(100.0, tetrahedron.distanceSquared(Geometry::Vec3d(-10.0, 0.0, 0.0)), delta);
	REQUIRE_DOUBLES_EQUAL(100.0, tetrahedron.distanceSquared(Geometry::Vec3d(-10.0, 30.0, 0.0)), delta);
	REQUIRE_DOUBLES_EQUAL(200.0, tetrahedron.distanceSquared(Geometry::Vec3d(-10.0, 40.0, 0.0)), delta);

	// Test points below the tetrahedron.
	REQUIRE_DOUBLES_EQUAL(800.0, tetrahedron.distanceSquared(Geometry::Vec3d(-20.0, -20.0, 0.0)), delta);
	REQUIRE_DOUBLES_EQUAL(400.0, tetrahedron.distanceSquared(Geometry::Vec3d(0.0, -20.0, 0.0)), delta);
	REQUIRE_DOUBLES_EQUAL(400.0, tetrahedron.distanceSquared(Geometry::Vec3d(30.0, -20.0, 0.0)), delta);
	REQUIRE_DOUBLES_EQUAL(800.0, tetrahedron.distanceSquared(Geometry::Vec3d(50.0, -20.0, 0.0)), delta);

	// Test points top right of the tetrahedron.
	REQUIRE_DOUBLES_EQUAL(900.0, tetrahedron.distanceSquared(Geometry::Vec3d(60.0, 0.0, 0.0)), delta);
	REQUIRE_DOUBLES_EQUAL(1800.0, tetrahedron.distanceSquared(Geometry::Vec3d(45.0, 45.0, 0.0)), delta);
	REQUIRE_DOUBLES_EQUAL(900.0, tetrahedron.distanceSquared(Geometry::Vec3d(0.0, 60.0, 0.0)), delta);
}

/**
 * Returns the volume of the given tetrahedron as alternative implementation.
 * \see http://en.wikipedia.org/wiki/Heron%27s_formula
 * @return The volume.
 */
static double calcTetrahedronVolume(const Geometry::Tetrahedron<double> & t) {
	const double U = (t.getVertexC() - t.getVertexD()).length<double>();
	const double V = (t.getVertexB() - t.getVertexC()).length<double>();
	const double W = (t.getVertexD() - t.getVertexB()).length<double>();
	const double u = (t.getVertexB() - t.getVertexA()).length<double>();
	const double v = (t.getVertexA() - t.getVertexD()).length<double>();
	const double w = (t.getVertexC() - t.getVertexA()).length<double>();
	const double X = (w - U + v) * (U + v + w);
	const double x = (U - v + w) * (v - w + U);
	const double Y = (u - V + w) * (V + w + u);
	const double y = (V - w + u) * (w - u + V);
	const double Z = (v - W + u) * (W + u + v);
	const double z = (W - u + v) * (u - v + W);
	const double a = sqrt(x * Y * Z);
	const double b = sqrt(y * Z * X);
	const double c = sqrt(z * X * Y);
	const double d = sqrt(x * y * z);
	return sqrtf((-a + b + c + d) * (a - b + c + d) * (a + b - c + d) * (a + b + c - d)) / (192.0 * u * v * w);
}

TEST_CASE("TetrahedronTest_testTetrahedronVolume", "[TetrahedronTest]") {
	const double delta = 0.2;

	{
		const Geometry::Vec3d a(0.0, 0.0, 0.0);
		const Geometry::Vec3d b(0.0, 30.0, 0.0);
		const Geometry::Vec3d c(0.0, 0.0, -30.0);
		for (double f = -100; f < 100; f += 1.73) {
			const Geometry::Tetrahedron<double> tetrahedron(
					(Geometry::Tetrahedron<double>(a, b, c, Geometry::Vec3d(f, 0.0, 0.0))));
			REQUIRE_DOUBLES_EQUAL(tetrahedron.calcVolume(), calcTetrahedronVolume(tetrahedron), delta);
		}
	}

	{
		const Geometry::Vec3d a(0.0, 0.0, 0.0);
		const Geometry::Vec3d b(0.0, 30.0, 0.0);
		const Geometry::Vec3d c(0.0, 0.0, -30.0);
		const Geometry::Vec3d d(30.0, 0.0, 0.0);
		const Geometry::Tetrahedron<double> tetrahedron(a, b, c, d);

		const float referenceVolume = tetrahedron.calcVolume();

		for (double f = -100; f < 100; f += 1.73) {
			Geometry::Matrix3x3d r = Geometry::Matrix3x3d::createRotation(Geometry::Angle::rad(f * 0.01),
																		  Geometry::Vec3d(1.0, 0.0, 0.0));
			r.normOrthoLize();
			r = Geometry::Matrix3x3d::createRotation(Geometry::Angle::rad(f * 0.017), Geometry::Vec3d(0.0, 1.0, 0.0))
					* r;
			r.normOrthoLize();
			r = Geometry::Matrix3x3d::createRotation(Geometry::Angle::rad(f * 0.027), Geometry::Vec3d(0.0, 0.0, 1.0))
					* r;
			r.normOrthoLize();

			const Geometry::Vec3d a2(r * a);
			const Geometry::Vec3d b2(r * b);
			const Geometry::Vec3d c2(r * c);
			const Geometry::Vec3d d2(r * d);

			REQUIRE_DOUBLES_EQUAL(referenceVolume, Geometry::Tetrahedron<double>(a2, b2, c2, d2).calcVolume(),
										 delta);
		}
	}
}

/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "RayBoxIntersectionTest.h"
#include "Box.h"
#include "Line.h"
#include "RayBoxIntersection.h"
#include <cstdint>
#include <vector>
CPPUNIT_TEST_SUITE_REGISTRATION(RayBoxIntersectionTest);

static const int32_t halfSideLength = 16;

using box_t = Geometry::_Box<double>;
using dim1_t = std::vector<box_t>;
using dim2_t = std::vector<dim1_t>;
using dim3_t = std::vector<dim2_t>;
using vec3_t = Geometry::_Vec3<double>;
using ray_t = Geometry::_Ray<vec3_t>;

struct BoolVerifyer {
	bool operator()(int32_t x, int32_t y, int32_t z, const ray_t & ray) const {
		const auto & ori = ray.getOrigin();
		const auto & dir = ray.getDirection();
		if (dir.getX() > 0) {
			return x >= ori.getX() && y == ori.getY() && z == ori.getZ();
		} else if (dir.getX() < 0) {
			return x <= ori.getX() && y == ori.getY() && z == ori.getZ();
		} else if (dir.getY() > 0) {
			return x == ori.getX() && y >= ori.getY() && z == ori.getZ();
		} else if (dir.getY() < 0) {
			return x == ori.getX() && y <= ori.getY() && z == ori.getZ();
		} else if (dir.getZ() > 0) {
			return x == ori.getX() && y == ori.getY() && z >= ori.getZ();
		} else if (dir.getZ() < 0) {
			return x == ori.getX() && y == ori.getY() && z <= ori.getZ();
		}
		return false;
	}
};

struct DistanceVerifyer {
	double operator()(const box_t & box, const ray_t & ray) const {
		const auto & ori = ray.getOrigin();
		const auto & dir = ray.getDirection();
		if (dir.getX() > 0) {
			return box.getMinX() - ori.getX();
		} else if (dir.getX() < 0) {
			return ori.getX() - box.getMaxX();
		} else if (dir.getY() > 0) {
			return box.getMinY() - ori.getY();
		} else if (dir.getY() < 0) {
			return ori.getY() - box.getMaxY();
		} else if (dir.getZ() > 0) {
			return box.getMinZ() - ori.getZ();
		} else if (dir.getZ() < 0) {
			return ori.getZ() - box.getMaxZ();
		}
		return false;
	}
};

static void testSingleRay(const ray_t & ray, const dim3_t & boxCube) {
	Geometry::Intersection::Slope<double> slope(ray);
	for (int_fast32_t z = -halfSideLength; z < halfSideLength; ++z) {
		for (int_fast32_t y = -halfSideLength; y < halfSideLength; ++y) {
			for (int_fast32_t x = -halfSideLength; x < halfSideLength; ++x) {
				const auto & currentBox = boxCube[halfSideLength + z][halfSideLength + y][halfSideLength + x];

				CPPUNIT_ASSERT_EQUAL(BoolVerifyer()(x, y, z, ray), slope.isRayIntersectingBox(currentBox));

				double intersection;
				const bool result = slope.getRayBoxIntersection(currentBox, intersection);
				CPPUNIT_ASSERT_EQUAL(BoolVerifyer()(x, y, z, ray), result);
				if (result) {
					CPPUNIT_ASSERT_DOUBLES_EQUAL(DistanceVerifyer()(currentBox, ray), intersection, 1.0e-12);
				}
			}
		}
	}
}

void RayBoxIntersectionTest::testRayBoxIntersection() {
	dim3_t boxCube;
	boxCube.reserve(2 * halfSideLength);
	for (int_fast32_t z = -halfSideLength; z < halfSideLength; ++z) {
		dim2_t boxPlane;
		boxPlane.reserve(2 * halfSideLength);
		for (int_fast32_t y = -halfSideLength; y < halfSideLength; ++y) {
			dim1_t boxRow;
			boxRow.reserve(2 * halfSideLength);
			for (int_fast32_t x = -halfSideLength; x < halfSideLength; ++x) {
				boxRow.emplace_back(vec3_t(x, y, z), 1.0);
			}
			boxPlane.emplace_back(boxRow);
		}
		boxCube.emplace_back(boxPlane);
	}

	{ // Direction: Positive X
		const vec3_t dirPosX(1.0, 0.0, 0.0);
		testSingleRay(ray_t(vec3_t(-halfSideLength / 2, 0.0, 0.0), dirPosX), boxCube);
		testSingleRay(ray_t(vec3_t(0.0, 0.0, 0.0), dirPosX), boxCube);
		testSingleRay(ray_t(vec3_t(halfSideLength / 2, 0.0, 0.0), dirPosX), boxCube);
	}
	{ // Direction: Negative X
		const vec3_t dirNegX(-1.0, 0.0, 0.0);
		testSingleRay(ray_t(vec3_t(-halfSideLength / 2, 0.0, 0.0), dirNegX), boxCube);
		testSingleRay(ray_t(vec3_t(0.0, 0.0, 0.0), dirNegX), boxCube);
		testSingleRay(ray_t(vec3_t(halfSideLength / 2, 0.0, 0.0), dirNegX), boxCube);
	}
	{ // Direction: Positive Y
		const vec3_t dirPosY(0.0, 1.0, 0.0);
		testSingleRay(ray_t(vec3_t(-halfSideLength / 2, 0.0, 0.0), dirPosY), boxCube);
		testSingleRay(ray_t(vec3_t(0.0, 0.0, 0.0), dirPosY), boxCube);
		testSingleRay(ray_t(vec3_t(halfSideLength / 2, 0.0, 0.0), dirPosY), boxCube);
	}
	{ // Direction: Negative Y
		const vec3_t dirNegY(0.0, -1.0, 0.0);
		testSingleRay(ray_t(vec3_t(-halfSideLength / 2, 0.0, 0.0), dirNegY), boxCube);
		testSingleRay(ray_t(vec3_t(0.0, 0.0, 0.0), dirNegY), boxCube);
		testSingleRay(ray_t(vec3_t(halfSideLength / 2, 0.0, 0.0), dirNegY), boxCube);
	}
	{ // Direction: Positive Z
		const vec3_t dirPosZ(0.0, 0.0, 1.0);
		testSingleRay(ray_t(vec3_t(-halfSideLength / 2, 0.0, 0.0), dirPosZ), boxCube);
		testSingleRay(ray_t(vec3_t(0.0, 0.0, 0.0), dirPosZ), boxCube);
		testSingleRay(ray_t(vec3_t(halfSideLength / 2, 0.0, 0.0), dirPosZ), boxCube);
	}
	{ // Direction: Negative Z
		const vec3_t dirNegZ(0.0, 0.0, -1.0);
		testSingleRay(ray_t(vec3_t(-halfSideLength / 2, 0.0, 0.0), dirNegZ), boxCube);
		testSingleRay(ray_t(vec3_t(0.0, 0.0, 0.0), dirNegZ), boxCube);
		testSingleRay(ray_t(vec3_t(halfSideLength / 2, 0.0, 0.0), dirNegZ), boxCube);
	}
}

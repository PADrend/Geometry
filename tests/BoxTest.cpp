/*
	This file is part of the Geometry library.
	Copyright (C) 2010-2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "BoxTest.h"
#include "Box.h"
#include "BoxHelper.h"
#include "BoxIntersection.h"
#include "Definitions.h"
#include "Matrix4x4.h"
#include "Triangle.h"
#include <sstream>
CPPUNIT_TEST_SUITE_REGISTRATION(BoxTest);

void BoxTest::testOperators() {
	const Geometry::Box b1(-1.0f, 1.0f, -2.0f, 2.0f, -3.0f, 3.0f);
	const Geometry::Box b2;

	CPPUNIT_ASSERT(b1 == b1);
	CPPUNIT_ASSERT(b2 == b2);
	CPPUNIT_ASSERT(b1 != Geometry::Box());
	CPPUNIT_ASSERT(Geometry::Box() != b1);
	CPPUNIT_ASSERT(b2 != b1);
	CPPUNIT_ASSERT(b1 != b2);
	CPPUNIT_ASSERT(Geometry::Box() == Geometry::Box());
	CPPUNIT_ASSERT(Geometry::Box(b1) == Geometry::Box(b1));
	CPPUNIT_ASSERT(Geometry::Box() == Geometry::Box(-0.0f, 0.0f, -0.0f, 0.0f, -0.0f, 0.0f));
	CPPUNIT_ASSERT(Geometry::Box(Geometry::Vec3(0.5f, 0.5f, 0.5f), 1.0f)
				   == Geometry::Box(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	CPPUNIT_ASSERT(Geometry::Box(Geometry::Vec3(0.0f, 0.0f, 0.0f), 2.0f, 4.0f, 6.0f) == b1);
	CPPUNIT_ASSERT(Geometry::Box(Geometry::Vec3(-1.0f, -2.0f, -3.0f), Geometry::Vec3(1.0f, 2.0f, 3.0f)) == b1);
}

void BoxTest::testGetters() {
	const Geometry::Box b1(-1.0f, 1.0f, -2.0f, 2.0f, -3.0f, 3.0f);
	const Geometry::Box b2;

	CPPUNIT_ASSERT(b1.getMaxX() == 1.0f);
	CPPUNIT_ASSERT(b1.getMax(Geometry::dimension_t::X) == 1.0f);
	CPPUNIT_ASSERT(b1.getMaxY() == 2.0f);
	CPPUNIT_ASSERT(b1.getMax(Geometry::dimension_t::Y) == 2.0f);
	CPPUNIT_ASSERT(b1.getMaxZ() == 3.0f);
	CPPUNIT_ASSERT(b1.getMax(Geometry::dimension_t::Z) == 3.0f);

	CPPUNIT_ASSERT(b1.getMinX() == -1.0f);
	CPPUNIT_ASSERT(b1.getMin(Geometry::dimension_t::X) == -1.0f);
	CPPUNIT_ASSERT(b1.getMinY() == -2.0f);
	CPPUNIT_ASSERT(b1.getMin(Geometry::dimension_t::Y) == -2.0f);
	CPPUNIT_ASSERT(b1.getMinZ() == -3.0f);
	CPPUNIT_ASSERT(b1.getMin(Geometry::dimension_t::Z) == -3.0f);

	CPPUNIT_ASSERT(b1.getExtentMax() == 6.0f);
	CPPUNIT_ASSERT(b1.getExtentMin() == 2.0f);

	CPPUNIT_ASSERT(b1.getExtentX() == 2.0f);
	CPPUNIT_ASSERT(b1.getExtent(Geometry::dimension_t::X) == 2.0f);
	CPPUNIT_ASSERT(b1.getExtentY() == 4.0f);
	CPPUNIT_ASSERT(b1.getExtent(Geometry::dimension_t::Y) == 4.0f);
	CPPUNIT_ASSERT(b1.getExtentZ() == 6.0f);
	CPPUNIT_ASSERT(b1.getExtent(Geometry::dimension_t::Z) == 6.0f);

	CPPUNIT_ASSERT(b1.getVolume() == 48.0f);
	CPPUNIT_ASSERT(b1.getSurfaceArea() == 88.0f);

	CPPUNIT_ASSERT(b1.getCenter() == Geometry::Vec3(0.0f, 0.0f, 0.0f));
	CPPUNIT_ASSERT(b1.getBoundingSphereRadius() == 0.5f * std::sqrt(56.0f));
}

void BoxTest::testSetters() {
	const Geometry::Box b1(-1.0f, 1.0f, -2.0f, 2.0f, -3.0f, 3.0f);
	Geometry::Box b2;

	b2.setMinX(-1.0f);
	b2.setMaxX(1.0f);
	b2.setMinY(-2.0f);
	b2.setMaxY(2.0f);
	b2.setMinZ(-3.0f);
	b2.setMaxZ(3.0f);
	CPPUNIT_ASSERT(b2 == b1);

	b2.setMin(Geometry::dimension_t::X, -1.0f);
	b2.setMax(Geometry::dimension_t::X, 1.0f);
	b2.setMin(Geometry::dimension_t::Y, -2.0f);
	b2.setMax(Geometry::dimension_t::Y, 2.0f);
	b2.setMin(Geometry::dimension_t::Z, -3.0f);
	b2.setMax(Geometry::dimension_t::Z, 3.0f);
	CPPUNIT_ASSERT(b2 == b1);

	b2.set(-1.0f, 1.0f, -2.0f, 2.0f, -3.0f, 3.0f);
	CPPUNIT_ASSERT(b2 == b1);

	b2 = Geometry::Box();
	CPPUNIT_ASSERT(b2 == Geometry::Box());

	b2 = b1;
	CPPUNIT_ASSERT(b2 == b1);

	b2.set(1.0f, 2.0f, 3.0f);
	b2.include(-1.0, -2.0f, -3.0f);
	CPPUNIT_ASSERT(b2 == b1);

	b2.set(1.0f, 2.0f, 3.0f);
	b2.include(Geometry::Vec3(-1.0, -2.0f, -3.0f));
	CPPUNIT_ASSERT(b2 == b1);

	b2 = Geometry::Box();
	b2.include(b1);
	CPPUNIT_ASSERT(b2 == b1);

	b2 = Geometry::Box();
	b2.setCenter(Geometry::Vec3(0.0f, 0.0f, 0.0f));
	b2.include(-1.0, -2.0f, -3.0f);
	b2.include(1.0, 2.0f, 3.0f);
	CPPUNIT_ASSERT(b2 == b1);

	b2 = Geometry::Box();
	b2.setCenter(Geometry::Vec3(0.5f, 0.5f, 0.5f));
	b2.setExtentX(1.0f);
	b2.setExtentY(1.0f);
	b2.setExtentZ(1.0f);
	CPPUNIT_ASSERT(b2 == Geometry::Box(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f));

	b2 = Geometry::Box();
	b2.setCenter(Geometry::Vec3(0.5f, 0.5f, 0.5f));
	b2.setExtent(1.0f);
	CPPUNIT_ASSERT(b2 == Geometry::Box(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f));

	b2.resizeAbs(2.0f);
	CPPUNIT_ASSERT(b2 == Geometry::Box(-2.0f, 3.0f, -2.0f, 3.0f, -2.0f, 3.0f));

	b2.resizeRel(1.5f);
	CPPUNIT_ASSERT(b2 == Geometry::Box(-3.25f, 4.25f, -3.25f, 4.25f, -3.25f, 4.25f));

	b2.translate(Geometry::Vec3(3.25f, -4.25f, 0.0f));
	CPPUNIT_ASSERT(b2 == Geometry::Box(0.0f, 7.5f, -7.5f, 0.0f, -3.25f, 4.25f));

	Geometry::Matrix4x4 mat;

	mat.setIdentity();
	b2 = Geometry::Helper::getTransformedBox(b2, mat);
	CPPUNIT_ASSERT(b2 == Geometry::Box(0.0f, 7.5f, -7.5f, 0.0f, -3.25f, 4.25f));

	mat.scale(2.0f);
	mat.translate(-3.25f, 4.25f, 0.0f);
	b2 = Geometry::Helper::getTransformedBox(b2, mat);
	CPPUNIT_ASSERT(b2 == Geometry::Box(-6.5f, 8.5f, -6.5f, 8.5f, -6.5f, 8.5f));
}

void BoxTest::testMisc() {
	const Geometry::Box b1(-1.0f, 1.0f, -2.0f, 2.0f, -3.0f, 3.0f);
	Geometry::Box b2;

	b2 = Geometry::Box();
	CPPUNIT_ASSERT(b2.isValid());
	b2.invalidate();
	CPPUNIT_ASSERT(b2.isInvalid());

	CPPUNIT_ASSERT(b1.getCorner(Geometry::corner_t::xyz) == Geometry::Vec3(b1.getMinX(), b1.getMinY(), b1.getMinZ()));
	CPPUNIT_ASSERT(b1.getCorner(Geometry::corner_t::Xyz) == Geometry::Vec3(b1.getMaxX(), b1.getMinY(), b1.getMinZ()));
	CPPUNIT_ASSERT(b1.getCorner(Geometry::corner_t::xYz) == Geometry::Vec3(b1.getMinX(), b1.getMaxY(), b1.getMinZ()));
	CPPUNIT_ASSERT(b1.getCorner(Geometry::corner_t::XYz) == Geometry::Vec3(b1.getMaxX(), b1.getMaxY(), b1.getMinZ()));
	CPPUNIT_ASSERT(b1.getCorner(Geometry::corner_t::xyZ) == Geometry::Vec3(b1.getMinX(), b1.getMinY(), b1.getMaxZ()));
	CPPUNIT_ASSERT(b1.getCorner(Geometry::corner_t::XyZ) == Geometry::Vec3(b1.getMaxX(), b1.getMinY(), b1.getMaxZ()));
	CPPUNIT_ASSERT(b1.getCorner(Geometry::corner_t::xYZ) == Geometry::Vec3(b1.getMinX(), b1.getMaxY(), b1.getMaxZ()));
	CPPUNIT_ASSERT(b1.getCorner(Geometry::corner_t::XYZ) == Geometry::Vec3(b1.getMaxX(), b1.getMaxY(), b1.getMaxZ()));

	CPPUNIT_ASSERT(Geometry::Box::getOppositeCorner(Geometry::corner_t::xyz) == Geometry::corner_t::XYZ);
	CPPUNIT_ASSERT(Geometry::Box::getOppositeCorner(Geometry::corner_t::Xyz) == Geometry::corner_t::xYZ);
	CPPUNIT_ASSERT(Geometry::Box::getOppositeCorner(Geometry::corner_t::xYz) == Geometry::corner_t::XyZ);
	CPPUNIT_ASSERT(Geometry::Box::getOppositeCorner(Geometry::corner_t::XYz) == Geometry::corner_t::xyZ);
	CPPUNIT_ASSERT(Geometry::Box::getOppositeCorner(Geometry::corner_t::xyZ) == Geometry::corner_t::XYz);
	CPPUNIT_ASSERT(Geometry::Box::getOppositeCorner(Geometry::corner_t::XyZ) == Geometry::corner_t::xYz);
	CPPUNIT_ASSERT(Geometry::Box::getOppositeCorner(Geometry::corner_t::xYZ) == Geometry::corner_t::Xyz);
	CPPUNIT_ASSERT(Geometry::Box::getOppositeCorner(Geometry::corner_t::XYZ) == Geometry::corner_t::xyz);

	CPPUNIT_ASSERT(Geometry::Helper::getNormal(Geometry::side_t::X_NEG) == Geometry::Vec3(-1.0f, 0.0f, 0.0f));
	CPPUNIT_ASSERT(Geometry::Helper::getNormal(Geometry::side_t::X_POS) == Geometry::Vec3(1.0f, 0.0f, 0.0f));
	CPPUNIT_ASSERT(Geometry::Helper::getNormal(Geometry::side_t::Y_NEG) == Geometry::Vec3(0.0f, -1.0f, 0.0f));
	CPPUNIT_ASSERT(Geometry::Helper::getNormal(Geometry::side_t::Y_POS) == Geometry::Vec3(0.0f, 1.0f, 0.0f));
	CPPUNIT_ASSERT(Geometry::Helper::getNormal(Geometry::side_t::Z_NEG) == Geometry::Vec3(0.0f, 0.0f, -1.0f));
	CPPUNIT_ASSERT(Geometry::Helper::getNormal(Geometry::side_t::Z_POS) == Geometry::Vec3(0.0f, 0.0f, 1.0f));

	for (uint_fast8_t s = 0; s < 6; ++s) {
		const Geometry::side_t side = static_cast<const Geometry::side_t>(s);
		const Geometry::corner_t * corners = Geometry::Helper::getCornerIndices(side);
		for (uint_fast8_t i = 0; i < 4; ++i) {
			const uint_fast8_t prevCorner = i % 4;
			const uint_fast8_t currentCorner = (i + 1) % 4;
			const uint_fast8_t nextCorner = (i + 2) % 4;
			const Geometry::Vec3 edgeA = b1.getCorner(corners[nextCorner]) - b1.getCorner(corners[currentCorner]);
			const Geometry::Vec3 edgeB = b1.getCorner(corners[prevCorner]) - b1.getCorner(corners[currentCorner]);
			Geometry::Vec3 normal = edgeA.cross(edgeB);
			normal.normalize();
			CPPUNIT_ASSERT(Geometry::Helper::getNormal(side) == normal);
		}
	}

	const Geometry::Vec3 v1(-1.1f, 0.0f, 0.0f);
	const Geometry::Vec3 v2(0.0f, 0.0f, 0.0f);
	const Geometry::Vec3 v3(1.1f, 0.0f, 0.0f);
	CPPUNIT_ASSERT(!b1.contains(-1.1f, 0.0f, 0.0f));
	CPPUNIT_ASSERT(b1.contains(0.0f, 0.0f, 0.0f));
	CPPUNIT_ASSERT(!b1.contains(1.1f, 0.0f, 0.0f));
	CPPUNIT_ASSERT(!b1.contains(v1));
	CPPUNIT_ASSERT(b1.contains(v2));
	CPPUNIT_ASSERT(!b1.contains(v3));

	CPPUNIT_ASSERT(
			Geometry::Intersection::isBoxIntersectingTriangle(b1, Geometry::Triangle<Geometry::Vec3>(v1, v2, v3)));

	CPPUNIT_ASSERT(b1.contains(Geometry::Box()));
	CPPUNIT_ASSERT(b1.contains(b1));
	CPPUNIT_ASSERT(!b1.contains(Geometry::Box(-1.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)));

	CPPUNIT_ASSERT(Geometry::Intersection::isBoxIntersectingBox(Geometry::Box(0, 1, 0, 1, 0, 1),
																Geometry::Box(0, 1, 0, 1, 0, 1)));
	CPPUNIT_ASSERT(!Geometry::Intersection::isBoxIntersectingBox(Geometry::Box(0, 1, 0, 1, 0, 1),
																 Geometry::Box(2, 3, 2, 3, 2, 3)));
	CPPUNIT_ASSERT(!Geometry::Intersection::isBoxIntersectingBox(Geometry::Box(0, 1, 0, 1, 0, 1),
																 Geometry::Box(0, 3, 2, 3, 2, 3)));
	CPPUNIT_ASSERT(!Geometry::Intersection::isBoxIntersectingBox(Geometry::Box(0, 1, 0, 1, 0, 1),
																 Geometry::Box(2, 3, 0, 3, 2, 3)));
	CPPUNIT_ASSERT(!Geometry::Intersection::isBoxIntersectingBox(Geometry::Box(0, 1, 0, 1, 0, 1),
																 Geometry::Box(2, 3, 2, 3, 0, 3)));
	CPPUNIT_ASSERT(Geometry::Intersection::isBoxIntersectingBox(Geometry::Box(0, 1, 0, 1, 0, 1),
																Geometry::Box(-1, 2, -1, 2, -1, 2)));

	CPPUNIT_ASSERT(
			Geometry::Intersection::isBoxIntersectingBox(b1, Geometry::Box(-1.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)));
	CPPUNIT_ASSERT(
			!Geometry::Intersection::isBoxIntersectingBox(b1, Geometry::Box(-1.1f, -1.0f, -2.1f, -2.0f, -3.0f, -3.0f)));
	b2 = Geometry::Box(0.5f, 1.5f, 1.5f, 2.5f, 2.5f, 3.5f);
	CPPUNIT_ASSERT(Geometry::Intersection::isBoxIntersectingBox(b1, b2));
	CPPUNIT_ASSERT(Geometry::Intersection::getBoxBoxIntersection(b1, b2)
				   == Geometry::Box(0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f));

	CPPUNIT_ASSERT(b1.getDistance(Geometry::Vec3(-2.0f, 0.0f, 0.0f)) == 1.0f);
	CPPUNIT_ASSERT(b1.getDistance(Geometry::Vec3(2.0f, 0.0f, 0.0f)) == 1.0f);
	CPPUNIT_ASSERT(b1.getDistance(Geometry::Vec3(0.0f, -3.0f, 0.0f)) == 1.0f);
	CPPUNIT_ASSERT(b1.getDistance(Geometry::Vec3(0.0f, 3.0f, 0.0f)) == 1.0f);
	CPPUNIT_ASSERT(b1.getDistance(Geometry::Vec3(0.0f, 0.0f, -4.0f)) == 1.0f);
	CPPUNIT_ASSERT(b1.getDistance(Geometry::Vec3(0.0f, 0.0f, 4.0f)) == 1.0f);
	CPPUNIT_ASSERT(b1.getDistanceSquared(Geometry::Vec3(-2.0f, -3.0f, -4.0f)) == 3.0f);
	CPPUNIT_ASSERT(b1.getDistanceSquared(Geometry::Vec3(2.0f, 3.0f, 4.0f)) == 3.0f);

	// Make sure the box b1 was never changed.
	CPPUNIT_ASSERT(b1 == Geometry::Box(-1.0f, 1.0f, -2.0f, 2.0f, -3.0f, 3.0f));
}

void BoxTest::testSerialization() {
	const Geometry::Box b1(-1.0f, 1.0f, -2.0f, 2.0f, -3.0f, 3.0f);
	const Geometry::Box b2;

	std::stringstream stream;
	stream << b1 << ' ';
	stream << b2;

	Geometry::Box b1Copy;
	stream >> b1Copy;
	Geometry::Box b2Copy;
	stream >> b2Copy;

	CPPUNIT_ASSERT(b1 == b1Copy);
	CPPUNIT_ASSERT(b2 == b2Copy);
}

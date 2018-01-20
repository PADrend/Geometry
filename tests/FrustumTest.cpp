/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2012 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "FrustumTest.h"
#include "Box.h"
#include "Frustum.h"
CPPUNIT_TEST_SUITE_REGISTRATION(FrustumTest);

void FrustumTest::testFrustumTest() {
	Geometry::Frustum frustum;
	frustum.setPerspective(Geometry::Angle::deg(90.0f), 1.0f, 1.0f, 10.0f);

	// (-10, *, 10)           (10, *, 10)
	//       X---------------------X      far plane
	//        \                   /
	//          \    Frustum    /
	//            \           /
	//              \       /
	//                X---X               near plane
	//       (-1, *, 1)   (1, *, 1)

	// Box tests from left to right.
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-9.5f, -7.5f, -1.0f, 1.0f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-7.0f, -5.0f, -1.0f, 1.0f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-4.5f, -2.5f, -1.0f, 1.0f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(2.5f, 4.5f, -1.0f, 1.0f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(5.0f, 7.0f, -1.0f, 1.0f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(7.5f, 9.5f, -1.0f, 1.0f, 5.0f, 7.0f)));

	// Box tests from bottom to top.
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -9.5f, -7.5f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -7.0f, -5.0f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -4.5f, -2.5f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, 2.5f, 4.5f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, 5.0f, 7.0f, 5.0f, 7.0f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, 7.5f, 9.5f, 5.0f, 7.0f)));

	// Box test from front to back.
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, -0.5f, 0.5f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 1.5f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 1.5f, 2.5f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 8.5f, 9.5f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 9.5f, 10.5f)));
	CPPUNIT_ASSERT(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 10.5f, 11.5f)));
}

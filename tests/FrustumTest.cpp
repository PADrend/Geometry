/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Box.h"
#include "Frustum.h"
#include <catch2/catch.hpp>

TEST_CASE("FrustumTest_testFrustumTest", "[FrustumTest]") {
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
	REQUIRE(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-9.5f, -7.5f, -1.0f, 1.0f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-7.0f, -5.0f, -1.0f, 1.0f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-4.5f, -2.5f, -1.0f, 1.0f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(2.5f, 4.5f, -1.0f, 1.0f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(5.0f, 7.0f, -1.0f, 1.0f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(7.5f, 9.5f, -1.0f, 1.0f, 5.0f, 7.0f)));

	// Box tests from bottom to top.
	REQUIRE(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -9.5f, -7.5f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -7.0f, -5.0f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -4.5f, -2.5f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, 2.5f, 4.5f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, 5.0f, 7.0f, 5.0f, 7.0f)));
	REQUIRE(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, 7.5f, 9.5f, 5.0f, 7.0f)));

	// Box test from front to back.
	REQUIRE(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, -0.5f, 0.5f)));
	REQUIRE(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 1.5f)));
	REQUIRE(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 1.5f, 2.5f)));
	REQUIRE(Geometry::Frustum::intersection_t::INSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 8.5f, 9.5f)));
	REQUIRE(Geometry::Frustum::intersection_t::INTERSECT ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 9.5f, 10.5f)));
	REQUIRE(Geometry::Frustum::intersection_t::OUTSIDE ==
						 frustum.isBoxInFrustum(Geometry::Box(-1.0f, 1.0f, -1.0f, 1.0f, 10.5f, 11.5f)));
}

/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2011-2013 Ralf Petring <ralf@petring.net>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "VecN.h"
#include <cmath>
#include <limits>
#include <cstdlib>
#include <catch2/catch.hpp>

using Geometry::VecNf;

static bool equal(const float u, const float v) {
	return std::abs(v - u) <= std::numeric_limits<float>::epsilon() * std::min(std::abs(u), std::abs(v));
}

template <typename Type, typename FloatType>
bool equal(const Geometry::_VecN<Type, FloatType> & a, const Geometry::_VecN<Type, FloatType> & b) {
	REQUIRE(a.size() == b.size());
	for (std::size_t i = 0; i < a.size(); i++)
		if (!equal(a[i], b[i]))
			return false;
	return true;
}

TEST_CASE("VecNTest_test", "[VecNTest]") {
	unsigned size = 17;
	VecNf one(size, 1);
	VecNf zero(size);
	VecNf half(size, 0.5);
	VecNf oneZero(one);
	for (unsigned i = 1; i < size; i += 2)
		oneZero[i] = 0;
	VecNf zeroOne = one - oneZero;
	REQUIRE(equal(zero, VecNf::pairwiseMin(oneZero, zeroOne)));
	REQUIRE(equal(one, VecNf::pairwiseMax(oneZero, zeroOne)));
	REQUIRE(equal(half, VecNf::pairwiseAvg(oneZero, zeroOne)));
	REQUIRE(equal(half, one / 2));
	REQUIRE(equal(half * 2, one));
	REQUIRE(equal(half + half, one));
	REQUIRE(equal(half, one - half));
	REQUIRE(equal(oneZero - one, -zeroOne));
	VecNf x(size);
	x = half;
	x *= 2;
	REQUIRE(equal(x, one));
	x = one;
	x /= 2;
	REQUIRE(equal(x, half));
	x = one;
	x -= half;
	REQUIRE(equal(x, half));
	x = half;
	x += half;
	REQUIRE(equal(x, one));
	VecNf norm(3);
	norm[0] = -2;
	norm[1] = 1;
	norm[2] = -3;
	REQUIRE(equal(norm.length(VecNf::MANHATTAN_NORM), 6));
	REQUIRE(equal(norm.length(VecNf::EUCLIDEAN_NORM), std::sqrt(14)));
	REQUIRE(equal(norm.length(VecNf::MAXIMUM_NORM), 3));
	REQUIRE(equal(norm.length(4), pow(98, 1.0f / 4.0f)));
	REQUIRE(equal(norm.length(3), pow(36, 1.0f / 3.0f)));
}

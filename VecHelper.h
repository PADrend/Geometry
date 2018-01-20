/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_VECHELPER_H
#define GEOMETRY_VECHELPER_H

#include "Vec2.h"
#include "Vec3.h"
#include <algorithm>

namespace Geometry {
namespace Helper {

/**
 * Return a vector that is orthogonal to a given vector.
 *
 * @param vector Any two-dimensional input vector
 * @return Vector that is orthogonal to and has the same length as the input
 * vector
 * @author Benjamin Eikel
 * @date 2013-03-05
 */
template <typename value_t>
_Vec2<value_t> createOrthogonal(const _Vec2<value_t> & vector) {
	return _Vec2<value_t>(-vector.getY(), vector.getX());
}

/**
 * Return a vector that is orthogonal to a given vector.
 *
 * Based on the article:
 * David Eberly: Computing Orthonormal Sets in 2D, 3D, and 4D.
 * http://www.geometrictools.com/Documentation/OrthonormalSets.pdf
 *
 * @param vector Normalized three-dimensional input vector
 * @return Vector that is orthogonal to the input vector and has length one
 * @author Benjamin Eikel
 * @date 2013-03-05
 */
template <typename value_t>
_Vec3<value_t> createOrthogonal(const _Vec3<value_t> & vector) {
	const auto x = vector.getX();
	const auto y = vector.getY();
	const auto z = vector.getZ();
	if (std::abs(x) >= std::abs(y)) {
		// x or z is the largest magnitude component
		const auto invLength = 1.0 / std::sqrt(x * x + z * z);
		return _Vec3<value_t>(-z * invLength, 0.0, x * invLength);
	} else {
		// y or z is the largest magnitude component
		const auto invLength = 1.0 / std::sqrt(y * y + z * z);
		return _Vec3<value_t>(0.0, z * invLength, -y * invLength);
	}
}
}
}

#endif /* GEOMETRY_VECHELPER_H */

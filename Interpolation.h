/*
	This file is part of the Geometry library.
	Copyright (C) 2012 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_INTERPOLATION_H
#define GEOMETRY_INTERPOLATION_H

#include <algorithm>

namespace Geometry {
namespace Interpolation {

/**
 * Clamp the value to the given range.
 *
 * @param lower Lower end of range
 * @param value Value to clamp
 * @param upper Upper end of range
 * @retval @p lower, if value < lower
 * @retval @p value, if lower <= value <= upper
 * @retval @p upper, if value > upper
 */
template <class val_t>
val_t clamp(val_t lower, val_t value, val_t upper) {
	return std::max(lower, std::min(value, upper));
}

/**
 * Interpolate linearly between @p p0 and @p p1.
 *
 * @param p0 First end point
 * @param p1 Second end point
 * @param t Interpolation parameter from [0, 1]
 * @return Interpolated point
 */
template <class vec_t, class val_t>
vec_t linear(vec_t p0, vec_t p1, val_t t) {
	const val_t v = clamp(static_cast<val_t>(0), t, static_cast<val_t>(1));
	const val_t oneMinusV = 1.0 - v;
	return p0 * oneMinusV + p1 * v;
}

/**
 * Interpolate between @p p0, @p p1, and @p p2 using a quadratic Bezier curve.
 *
 * @param p0 First end point
 * @param p1 Control point
 * @param p2 Second end point
 * @param t Interpolation parameter from [0, 1]
 * @return Interpolated point
 */
template <class vec_t, class val_t>
vec_t quadraticBezier(vec_t p0, vec_t p1, vec_t p2, val_t t) {
	const val_t v = clamp(static_cast<val_t>(0), t, static_cast<val_t>(1));
	const val_t oneMinusV = 1.0 - v;
	return p0 * oneMinusV * oneMinusV + p1 * 2.0 * oneMinusV * v + p2 * v * v;
}

/**
 * Interpolate between @p p0, @p p1, @p p2, and @p p3 using a cubic Bezier
 * curve.
 *
 * @param p0 First end point
 * @param p1 First control point
 * @param p2 Second control point
 * @param p3 Second end point
 * @param t Interpolation parameter from [0, 1]
 * @return Interpolated point
 */
template <class vec_t, class val_t>
vec_t cubicBezier(vec_t p0, vec_t p1, vec_t p2, vec_t p3, val_t t) {
	const val_t v = clamp(static_cast<val_t>(0), t, static_cast<val_t>(1));
	const val_t oneMinusV = 1.0 - v;
	const val_t vSquared = v * v;
	const val_t oneMinusVSquared = oneMinusV * oneMinusV;
	return p0 * oneMinusVSquared * oneMinusV + p1 * 3.0 * oneMinusVSquared * v + p2 * 3.0 * oneMinusV * vSquared
			+ p3 * vSquared * v;
}
}
}

#endif /* GEOMETRY_INTERPOLATION_H */

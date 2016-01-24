/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_LINETRIANGLEINTERSECTION_H
#define GEOMETRY_LINETRIANGLEINTERSECTION_H

#include "Line.h"
#include "Triangle.h"
#include <limits>

namespace Geometry {
namespace Intersection {

/**
 * Calculate the intersection between a line and a triangle.
 * Based on the article:
 * Tomas Möller, Ben Trumbore: Fast, Minimum Storage Ray-Triangle Intersection.
 * Journal of Graphics Tools 2, 1, pp. 21-28, 1997.
 *
 * @tparam value_t Either @c float or @c double
 * @tparam line_t Line type (e.g. _Line, _Ray, _Segment)
 * @param line Line to test
 * @param triangle Triangle to test
 * @param[out] tLine Line parameter of the intersection point between line and
 * triangle. It is set only if there is an intersection.
 * @param[out] uTri First barycentric coordinate of the intersection point
 * between line and triangle. It is set only if there is an intersection.
 * @param[out] vTri Second barycentric coordinate of the intersection point
 * between line and triangle. It is set only if there is an intersection.
 * @return @c true if the line and the triangle intersect, @c false if there is
 * no intersection
 * @author Benjamin Eikel
 * @date 2013-02-26
 */
template <typename value_t, typename line_t>
bool getLineTriangleIntersection(const line_t & line, const Triangle<_Vec3<value_t>> & triangle, value_t & tLine,
								 value_t & uTri, value_t & vTri) {
	const auto epsilon = std::numeric_limits<value_t>::epsilon();

	const auto & ori = line.getOrigin();
	const auto & dir = line.getDirection();

	// Find vectors for two edges sharing vertex A
	const auto edgeAB = triangle.getEdgeAB();
	const auto edgeAC = triangle.getEdgeAC();

	// Begin calculating determinant - also used to calculate u parameter
	const auto pVec = dir.cross(edgeAC);

	// If determinant is near zero, line lies in plane of triangle
	const value_t det = edgeAB.dot(pVec);
	if (det > -epsilon && det < epsilon) {
		return false;
	}
	const auto invDet = 1.0 / det;

	// Calculate distance from vertex A to line origin
	const auto tVec = ori - triangle.getVertexA();

	// Calculate u parameter and test bounds
	const auto u = tVec.dot(pVec) * invDet;
	if (u < 0.0 || u > 1.0) {
		return false;
	}

	// Prepare to test v parameter
	const auto qVec = tVec.cross(edgeAB);

	// Calculate v parameter and test bounds
	const auto v = dir.dot(qVec) * invDet;
	if (v < 0.0 || u + v > 1.0) {
		return false;
	}

	// Line intersects triangle: Calculate parameters
	tLine = edgeAC.dot(qVec) * invDet;
	uTri = u;
	vTri = v;

	return true;
}
}
}

#endif /* GEOMETRY_LINETRIANGLEINTERSECTION_H */

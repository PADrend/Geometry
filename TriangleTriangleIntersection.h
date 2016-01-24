/*
	This file is part of the Geometry library.
	Copyright (C) 2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_TRIANGLETRIANGLEINTERSECTION_H
#define GEOMETRY_TRIANGLETRIANGLEINTERSECTION_H

#include "Line.h"
#include "Triangle.h"
#include "Plane.h"
#include <limits>
#include <cmath>

/* this edge to edge test is based on Franlin Antonio's gem:
   "Faster Line Segment Intersection", in Graphics Gems III,
   pp. 199-202 */
#define EDGE_EDGE_TEST(V0, U0, U1)                                                                                     \
	B = U0 - U1;                                                                                                       \
	C = V0 - U0;                                                                                                       \
	f = A.y() * B.x() - A.x() * B.y();                                                                                 \
	d = B.y() * C.x() - B.x() * C.y();                                                                                 \
	if ((f > 0 && d >= 0 && d <= f) || (f < 0 && d <= 0 && d >= f)) {                                                  \
		e = A.x() * C.y() - A.y() * C.x();                                                                             \
		if (f > 0) {                                                                                                   \
			if (e >= 0 && e <= f)                                                                                      \
				return 0;                                                                                              \
		} else {                                                                                                       \
			if (e <= 0 && e >= f)                                                                                      \
				return 0;                                                                                              \
		}                                                                                                              \
	}

#define EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2)                                                                     \
	{                                                                                                                  \
		_Vec2<value_t> A, B, C;                                                                                        \
		float e, d, f;                                                                                                 \
		A = V1 - V0;                                                                                                   \
		/* test edge U0,U1 against V0,V1 */                                                                            \
		EDGE_EDGE_TEST(V0, U0, U1);                                                                                    \
		/* test edge U1,U2 against V0,V1 */                                                                            \
		EDGE_EDGE_TEST(V0, U1, U2);                                                                                    \
		/* test edge U2,U1 against V0,V1 */                                                                            \
		EDGE_EDGE_TEST(V0, U2, U0);                                                                                    \
	}

#define POINT_IN_TRI(V0, U0, U1, U2)                                                                                   \
	{                                                                                                                  \
		float a, b, c, d0, d1, d2;                                                                                     \
		/* is T1 completly inside T2? */                                                                               \
		/* check if V0 is inside tri(U0,U1,U2) */                                                                      \
		a = U1[1] - U0[1];                                                                                             \
		b = -(U1[0] - U0[0]);                                                                                          \
		c = -a * U0[0] - b * U0[1];                                                                                    \
		d0 = a * V0[0] + b * V0[1] + c;                                                                                \
                                                                                                                       \
		a = U2[1] - U1[1];                                                                                             \
		b = -(U2[0] - U1[0]);                                                                                          \
		c = -a * U1[0] - b * U1[1];                                                                                    \
		d1 = a * V0[0] + b * V0[1] + c;                                                                                \
                                                                                                                       \
		a = U0[1] - U2[1];                                                                                             \
		b = -(U0[0] - U2[0]);                                                                                          \
		c = -a * U2[0] - b * U2[1];                                                                                    \
		d2 = a * V0[0] + b * V0[1] + c;                                                                                \
		if (d0 * d1 > 0.0) {                                                                                           \
			if (d0 * d2 > 0.0)                                                                                         \
				return 0;                                                                                              \
		}                                                                                                              \
	}

namespace Geometry {
namespace Intersection {

template <typename value_t>
inline void _intersect(const _Vec3<value_t> & v0, const _Vec3<value_t> & v1, const _Vec3<value_t> & v2, value_t pv0,
					   value_t pv1, value_t pv2, float dv0, float dv1, float dv2, float * intersection1,
					   float * intersection2, _Vec3<value_t> & point1, _Vec3<value_t> & point2) {
	float tmp = dv0 / (dv0 - dv1);
	*intersection1 = pv0 + (pv1 - pv0) * tmp;
	point1 += (v0 + (v1 - v0) * tmp);
	tmp = dv0 / (dv0 - dv2);
	*intersection2 = pv0 + (pv2 - pv0) * tmp;
	point2 += (v0 + (v2 - v0) * tmp);
}

template <typename value_t>
inline bool _computeIntersectionInterval(const Triangle<_Vec3<value_t>> & triangle, float pv0, float pv1, float pv2,
										 float dv0, float dv1, float dv2, float dv0dv1, float dv0dv2,
										 float * intersection1, float * intersection2, _Vec3<value_t> & point1,
										 _Vec3<value_t> & point2) {
	auto v0 = triangle.getVertexA();
	auto v1 = triangle.getVertexB();
	auto v2 = triangle.getVertexC();
	if (dv0dv1 > 0.0f) {
		// here we know that dv0dv2<=0.0
		// that is dv0, dv1 are on the same side, dv2 on the other or on the plane
		_intersect(v2, v0, v1, pv2, pv0, pv1, dv2, dv0, dv1, intersection1, intersection2, point1, point2);
	} else if (dv0dv2 > 0.0f) {
		// here we know that dv0dv1<=0.0
		// that is dv0, dv2 are on the same side, dv1 on the other or on the plane
		_intersect(v1, v0, v2, pv1, pv0, pv2, dv1, dv0, dv2, intersection1, intersection2, point1, point2);
	} else if (dv1 * dv2 > 0.0f || dv0 != 0.0f) {
		// here we know that dv0dv1<=0.0 or dv0!=0.0
		// that is dv1, dv2 are on the same side, dv0 on the other or on the plane
		_intersect(v0, v1, v2, pv0, pv1, pv2, dv0, dv1, dv2, intersection1, intersection2, point1, point2);
	} else if (dv1 != 0.0f) {
		_intersect(v1, v0, v2, pv1, pv0, pv2, dv1, dv0, dv2, intersection1, intersection2, point1, point2);
	} else if (dv2 != 0.0f) {
		_intersect(v2, v0, v1, pv2, pv0, pv1, dv2, dv0, dv1, intersection1, intersection2, point1, point2);
	} else {
		// triangles are coplanar
		return true;
	}
	return false;
}

/**
 * Calculate the intersection between a triangle and a triangle.
 * Based on the article:
 * Tomas Möller: A Fast Triangle-Triangle Intersection Test.
 * Journal of Graphics Tools 2, 2, pp. 25-30, 1997.
 *
 * @tparam value_t Either @c float or @c double
 * @param triangle1 First triangle to test
 * @param triangle2 Second triangle to test
 * @param[out] tSegment Line segment of the intersection between the triangles.
 * It is set only if there is an intersection.
 * @return @c 1 if the triangles intersect, @c -1 if there is no intersection, @c 0 if the triangles are coplanar
 * @author Sascha Brandt
 * @date 2015-02-26
 */
template <typename value_t>
int32_t getTriangleTriangleIntersection(const Triangle<_Vec3<value_t>> & triangle1,
										const Triangle<_Vec3<value_t>> & triangle2,
										_Segment<_Vec3<value_t>> & tSegment) {
	const auto epsilon = std::numeric_limits<value_t>::epsilon();

	// compute plane of triangle2 and compute signed distances of triangle1 to the plane
	const _Plane<value_t> plane2(triangle2.getVertexA(), triangle2.getVertexB(), triangle2.getVertexC());
	float du0 = plane2.planeTest(triangle1.getVertexA());
	float du1 = plane2.planeTest(triangle1.getVertexB());
	float du2 = plane2.planeTest(triangle1.getVertexC());

	// coplanarity robustness check
	if (std::fabs(du0) < epsilon)
		du0 = 0.0;
	if (std::fabs(du1) < epsilon)
		du1 = 0.0;
	if (std::fabs(du2) < epsilon)
		du2 = 0.0;

	float du0du1 = du0 * du1;
	float du0du2 = du0 * du2;
	if (du0du1 > 0.0f && du0du2 > 0.0f) // same sign on all of them + not equal 0 ?
		return -1; // no intersection occurs

	// compute plane of triangle1 and compute signed distances of triangle2 to the plane
	const _Plane<value_t> plane1(triangle1.getVertexA(), triangle1.getVertexB(), triangle1.getVertexC());
	float dv0 = plane1.planeTest(triangle2.getVertexA());
	float dv1 = plane1.planeTest(triangle2.getVertexB());
	float dv2 = plane1.planeTest(triangle2.getVertexC());

	// coplanarity robustness check
	if (std::fabs(dv0) < epsilon)
		dv0 = 0.0;
	if (std::fabs(dv1) < epsilon)
		dv1 = 0.0;
	if (std::fabs(dv2) < epsilon)
		dv2 = 0.0;

	float dv0dv1 = dv0 * dv1;
	float dv0dv2 = dv0 * dv2;
	if (dv0dv1 > 0.0f && dv0dv2 > 0.0f) // same sign on all of them + not equal 0 ?
		return -1; // no intersection occurs

	// compute direction of intersection line
	auto dir = plane1.getNormal().cross(plane2.getNormal());

	// compute and index to the largest component of D
	value_t max = std::fabs(dir[0]);
	uint8_t index = 0;
	{
		value_t b = std::fabs(dir[1]);
		value_t c = std::fabs(dir[2]);
		if (b > max)
			max = b, index = 1;
		if (c > max)
			max = c, index = 2;
	}

	// this is the simplified projection onto L
	value_t pu0 = triangle1.getVertexA()[index];
	value_t pu1 = triangle1.getVertexB()[index];
	value_t pu2 = triangle1.getVertexC()[index];

	value_t pv0 = triangle2.getVertexA()[index];
	value_t pv1 = triangle2.getVertexB()[index];
	value_t pv2 = triangle2.getVertexC()[index];

	// compute interval for triangle 1
	float intersectionA1;
	_Vec3<value_t> pointA1;
	float intersectionA2;
	_Vec3<value_t> pointA2;
	if (_computeIntersectionInterval(triangle1, pu0, pu1, pu2, du0, du1, du2, du0du1, du0du2, &intersectionA1,
									 &intersectionA2, pointA1, pointA2)) {
		// first project onto an axis-aligned plane, that maximizes the area of the triangles
		uint8_t i0, i1;
		{
			_Vec3<value_t> a = plane1.getNormal().getAbs();
			if (a.x() > a.y()) {
				if (a.x() > a.z()) {
					i0 = 1; // x is greatest
					i1 = 2;
				} else {
					i0 = 0; // z is greatest
					i1 = 1;
				}
			} else {
				if (a.z() > a.y()) {
					i0 = 0; // z is greatest
					i1 = 1;
				} else {
					i0 = 0; // y is greatest
					i1 = 2;
				}
			}
		}
		_Vec2<value_t> U0(triangle1.getVertexA()[i0], triangle1.getVertexA()[i1]);
		_Vec2<value_t> U1(triangle1.getVertexB()[i0], triangle1.getVertexB()[i1]);
		_Vec2<value_t> U2(triangle1.getVertexC()[i0], triangle1.getVertexC()[i1]);
		_Vec2<value_t> V0(triangle2.getVertexA()[i0], triangle2.getVertexA()[i1]);
		_Vec2<value_t> V1(triangle2.getVertexB()[i0], triangle2.getVertexB()[i1]);
		_Vec2<value_t> V2(triangle2.getVertexC()[i0], triangle2.getVertexC()[i1]);

		// test all edges of triangle 1 against the edges of triangle 2
		EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2);
		EDGE_AGAINST_TRI_EDGES(V1, V2, U0, U1, U2);
		EDGE_AGAINST_TRI_EDGES(V2, V0, U0, U1, U2);

		// finally, test if tri1 is totally contained in tri2 or vice versa
		POINT_IN_TRI(V0, U0, U1, U2)
		POINT_IN_TRI(U0, V0, V1, V2)
		return -1;
	}

	// compute interval for triangle 2
	float intersectionB1 = 0.0f;
	_Vec3<value_t> pointB1;
	float intersectionB2 = 0.0f;
	_Vec3<value_t> pointB2;
	_computeIntersectionInterval(triangle2, pv0, pv1, pv2, dv0, dv1, dv2, dv0dv1, dv0dv2, &intersectionB1,
								 &intersectionB2, pointB1, pointB2);

	if (intersectionA1 > intersectionA2) {
		std::swap(intersectionA1, intersectionA2);
		std::swap(pointA1, pointA2);
	}
	if (intersectionB1 > intersectionB2) {
		std::swap(intersectionB1, intersectionB2);
		std::swap(pointB1, pointB2);
	}

	if (intersectionA2 < intersectionB1 || intersectionB2 < intersectionA1)
		return -1;

	// at this point, we know that the triangles intersect
	if (intersectionB1 < intersectionA1)
		tSegment.setFirstPoint(pointA1);
	else
		tSegment.setFirstPoint(pointB1);

	if (intersectionB2 < intersectionA2)
		tSegment.setSecondPoint(pointB2);
	else
		tSegment.setSecondPoint(pointA2);
	return true;
}
}
}

#endif /* GEOMETRY_TRIANGLETRIANGLEINTERSECTION_H */

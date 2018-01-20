/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "BoxIntersection.h"
#include "Box.h"
#include "Triangle.h"
#include "Vec3.h"

namespace Geometry {
namespace Intersection {

bool isBoxIntersectingTriangle(const Box_f & box, const Triangle_f & triangle) {
	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */

	Vec3 boxcenter = box.getCenter();
	Vec3 boxhalfsize(0.5f * box.getExtentX(), 0.5f * box.getExtentY(), 0.5f * box.getExtentZ());

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
	const auto v0 = triangle.getVertexA() - boxcenter;
	const auto v1 = triangle.getVertexB() - boxcenter;
	const auto v2 = triangle.getVertexC() - boxcenter;

	/* compute triangle edges */
	const auto e0 = triangle.getEdgeAB();
	const auto e1 = triangle.getEdgeBC();
	const auto e2 = triangle.getEdgeCA();

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	float p0, p1, p2, rad, pMin, pMax;

	Vec3 fe = e0.getAbs();
	p0 = e0.z() * v0.y() - e0.y() * v0.z();
	p2 = e0.z() * v2.y() - e0.y() * v2.z();
	if (p0 < p2) {
		pMin = p0;
		pMax = p2;
	} else {
		pMin = p2;
		pMax = p0;
	}
	rad = fe.z() * boxhalfsize.y() + fe.y() * boxhalfsize.z();
	if (pMin > rad || pMax < -rad)
		return 0;

	p0 = -e0.z() * v0.x() + e0.x() * v0.z();
	p2 = -e0.z() * v2.x() + e0.x() * v2.z();
	if (p0 < p2) {
		pMin = p0;
		pMax = p2;
	} else {
		pMin = p2;
		pMax = p0;
	}
	rad = fe.z() * boxhalfsize.x() + fe.x() * boxhalfsize.z();
	if (pMin > rad || pMax < -rad)
		return 0;

	p1 = e0.y() * v1.x() - e0.x() * v1.y();
	p2 = e0.y() * v2.x() - e0.x() * v2.y();
	if (p2 < p1) {
		pMin = p2;
		pMax = p1;
	} else {
		pMin = p1;
		pMax = p2;
	}
	rad = fe.y() * boxhalfsize.x() + fe.x() * boxhalfsize.y();
	if (pMin > rad || pMax < -rad)
		return 0;

	fe = e1.getAbs();
	p0 = e1.z() * v0.y() - e1.y() * v0.z();
	p2 = e1.z() * v2.y() - e1.y() * v2.z();
	if (p0 < p2) {
		pMin = p0;
		pMax = p2;
	} else {
		pMin = p2;
		pMax = p0;
	}
	rad = fe.z() * boxhalfsize.y() + fe.y() * boxhalfsize.z();
	if (pMin > rad || pMax < -rad)
		return 0;

	p0 = -e1.z() * v0.x() + e1.x() * v0.z();
	p2 = -e1.z() * v2.x() + e1.x() * v2.z();
	if (p0 < p2) {
		pMin = p0;
		pMax = p2;
	} else {
		pMin = p2;
		pMax = p0;
	}
	rad = fe.z() * boxhalfsize.x() + fe.x() * boxhalfsize.z();
	if (pMin > rad || pMax < -rad)
		return 0;

	p0 = e1.y() * v0.x() - e1.x() * v0.y();
	p1 = e1.y() * v1.x() - e1.x() * v1.y();
	if (p0 < p1) {
		pMin = p0;
		pMax = p1;
	} else {
		pMin = p1;
		pMax = p0;
	}
	rad = fe.y() * boxhalfsize.x() + fe.x() * boxhalfsize.y();
	if (pMin > rad || pMax < -rad)
		return 0;

	fe = e2.getAbs();
	p0 = e2.z() * v0.y() - e2.y() * v0.z();
	p1 = e2.z() * v1.y() - e2.y() * v1.z();
	if (p0 < p1) {
		pMin = p0;
		pMax = p1;
	} else {
		pMin = p1;
		pMax = p0;
	}
	rad = fe.z() * boxhalfsize.y() + fe.y() * boxhalfsize.z();
	if (pMin > rad || pMax < -rad)
		return 0;

	p0 = -e2.z() * v0.x() + e2.x() * v0.z();
	p1 = -e2.z() * v1.x() + e2.x() * v1.z();
	if (p0 < p1) {
		pMin = p0;
		pMax = p1;
	} else {
		pMin = p1;
		pMax = p0;
	}
	rad = fe.z() * boxhalfsize.x() + fe.x() * boxhalfsize.z();
	if (pMin > rad || pMax < -rad)
		return 0;

	p1 = e2.y() * v1.x() - e2.x() * v1.y();
	p2 = e2.y() * v2.x() - e2.x() * v2.y();
	if (p2 < p1) {
		pMin = p2;
		pMax = p1;
	} else {
		pMin = p1;
		pMax = p2;
	}
	rad = fe.y() * boxhalfsize.x() + fe.x() * boxhalfsize.y();
	if (pMin > rad || pMax < -rad)
		return 0;

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	if (std::min(std::min(v0.x(), v1.x()), v2.x()) > boxhalfsize[0]
		|| std::max(std::max(v0.x(), v1.x()), v2.x()) < -boxhalfsize[0])
		return 0;

	/* test in Y-direction */
	if (std::min(std::min(v0.y(), v1.y()), v2.y()) > boxhalfsize[1]
		|| std::max(std::max(v0.y(), v1.y()), v2.y()) < -boxhalfsize[1])
		return 0;

	/* test in Z-direction */
	if (std::min(std::min(v0.z(), v1.z()), v2.z()) > boxhalfsize[2]
		|| std::max(std::max(v0.z(), v1.z()), v2.z()) < -boxhalfsize[2])
		return 0;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	Vec3 normal = e0.cross(e1);
	float d = -normal.dot(v0); /* plane eq: normal.x+d=0 */

	Vec3 vmin, vmax;
	for (int q = 0; q < 3; q++) {
		if (normal[q] > 0.0f) {
			vmin[q] = -boxhalfsize[q];
			vmax[q] = boxhalfsize[q];
		} else {
			vmin[q] = boxhalfsize[q];
			vmax[q] = -boxhalfsize[q];
		}
	}
	if (normal.dot(vmin) + d > 0.0f)
		return 0;
	if (normal.dot(vmax) + d >= 0.0f)
		return 1;

	return 0;
}
}
}

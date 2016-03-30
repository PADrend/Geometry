/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Frustum.h"
#include "Box.h"
#include "BoxHelper.h"
#include <cmath>
#include <cstdint>

//
///**
// * sphereInFrustum
// *   //  www.lighthouse3d.com/opengl/viewfrustum/index.php?rimp
// */
//// 1. Sphere-Test
//
//    if (angle != 0) // 0 --> Orthogonal Kamera, Spheretest funktioniert noch nicht ;-)
//    {
//        int c=sphereInFrustum(b.getCenter(),b.getBoundingSphereRadius());
//        if (c==Frustum::OUTSIDE || c==Frustum::INSIDE)
//            return c;
//    }

namespace Geometry {

//! [dtor]
Frustum::Frustum() {
	setPosition(Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 0));
	setPerspective(Angle::deg(60.0f), 1.0, 0.1, 1000.0);
}

//! [dtor]
Frustum::Frustum(const Angle & angle, float ratio, float _near, float _far) {
	setPerspective(angle, ratio, _near, _far);
	// ctor
}

void Frustum::setPerspective(const Angle & angle, float ratio, float _near, float _far) {
	const float tang = std::tan(angle.rad() * 0.5f);
	setFrustum(-_near * tang * ratio, _near * tang * ratio, -_near * tang, _near * tang, _near, _far, false);
}

void Frustum::setFrustum(float _left, float _right, float _bottom, float _top, float _near, float _far,
						 bool _orthogonal /*= false*/) {
	orthogonal = _orthogonal;
	nearValue = _near;
	farValue = _far;
	leftValue = _left;
	rightValue = _right;
	topValue = _top;
	bottomValue = _bottom;

	// update matrix
	if (orthogonal) {
		projectionMatrix =
				Matrix4x4::orthographicProjection(leftValue, rightValue, bottomValue, topValue, nearValue, farValue);
	} else {
		projectionMatrix =
				Matrix4x4::perspectiveProjection(leftValue, rightValue, bottomValue, topValue, nearValue, farValue);
	}

	// update corners
	recalculateCornersAndPlanes();
}

void Frustum::setFrustumFromAngles(const Angle & fovLeft, const Angle & fovRight, const Angle & fovBottom,
								   const Angle & fovTop, float _near, float _far) {
	setFrustum(_near * std::tan(fovLeft.rad()), _near * std::tan(fovRight.rad()), _near * std::tan(fovBottom.rad()),
			   _near * std::tan(fovTop.rad()), _near, _far, false);
}

/**
 * @param   _pos vector to pyramid-root
 *          _dir direction-vector relative to (0,0,0)
 *          _up up-vector relative to (0,0,0)
 */
void Frustum::setPosition(const Vec3 & pos, const Vec3 & _dir, const Vec3 & _up) {
	orientation = SRT(pos, Vec3(_dir).normalize(), Vec3(_up).normalize());

	recalculateCornersAndPlanes();
}

void Frustum::recalculateCornersAndPlanes() {
	SRT s2 = SRT(getPos(), -getDir(), getUp());
	Matrix4x4 m = Matrix4x4(s2) * projectionMatrix.inverse();

	// Careful: Z values are inverted by matrix. Therefore the front and back side are swapped (see sign of Z values
	// below).
	corners[static_cast<std::size_t>(corner_t::xyz)] = m.transformPosition(-1, -1, 1);
	corners[static_cast<std::size_t>(corner_t::Xyz)] = m.transformPosition(1, -1, 1);
	corners[static_cast<std::size_t>(corner_t::xYz)] = m.transformPosition(-1, 1, 1);
	corners[static_cast<std::size_t>(corner_t::XYz)] = m.transformPosition(1, 1, 1);
	corners[static_cast<std::size_t>(corner_t::xyZ)] = m.transformPosition(-1, -1, -1);
	corners[static_cast<std::size_t>(corner_t::XyZ)] = m.transformPosition(1, -1, -1);
	corners[static_cast<std::size_t>(corner_t::xYZ)] = m.transformPosition(-1, 1, -1);
	corners[static_cast<std::size_t>(corner_t::XYZ)] = m.transformPosition(1, 1, -1);

	for (uint_fast8_t side = 0; side < 6; ++side) {
		// Side corners
		const corner_t * sC = Helper::getCornerIndices(static_cast<side_t>(side));
		// Calculate plane
		const Vec3 normal(((corners[static_cast<std::size_t>(sC[2])] - corners[static_cast<std::size_t>(sC[1])]).cross(corners[static_cast<std::size_t>(sC[0])] - corners[static_cast<std::size_t>(sC[1])])).normalize());
		planes[side] = Plane(normal, normal.dot(corners[static_cast<std::size_t>(sC[1])]));
	}

	// Calculate bit-fields for bounding box corners.
	for (uint_fast8_t plane = 0; plane < 6; ++plane) {
		const Vec3 & currentNormal = planes[plane].getNormal();

		uint8_t neg = 0;
		if (currentNormal.x() < 0.0f) {
			neg |= 1;
		}
		if (currentNormal.y() < 0.0f) {
			neg |= 2;
		}
		if (currentNormal.z() < 0.0f) {
			neg |= 4;
		}

		uint8_t pos = ~neg & 7;

		negCorner[plane] = static_cast<corner_t>(neg);
		posCorner[plane] = static_cast<corner_t>(pos);
	}
}

Frustum::intersection_t Frustum::isBoxInFrustum(const Box & b) const {
	/*
	 * Implementation corresponding to the pseudo-code in
	 * Ulf Assarsson, Tomas Möller: Optimized view frustum culling algorithms for bounding boxes.
	 * Journal of Graphics Tools, Volume 5, Issue 1, September 2000. Pages 9-22.
	 */
	bool intersect = false;
	for (uint_fast8_t plane = 0; plane < 6; ++plane) {
		const Vec3 nVec = b.getCorner(negCorner[plane]);
		if (planes[plane].planeTest(nVec) > 0) {
			return intersection_t::OUTSIDE;
		}

		const Vec3 pVec = b.getCorner(posCorner[plane]);
		if (planes[plane].planeTest(pVec) > 0) {
			intersect = true;
		}
	}
	if (intersect) {
		return intersection_t::INTERSECT;
	} else {
		return intersection_t::INSIDE;
	}
}

bool Frustum::operator==(const Frustum & other) const {
	return projectionMatrix == other.projectionMatrix;
}
}

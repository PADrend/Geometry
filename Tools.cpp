/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Tools.h"
#include "Box.h"
#include "Definitions.h"
#include "Frustum.h"
#include "Matrix4x4.h"
#include "Rect.h"
#include "Sphere.h"
#include "Vec3.h"
#include <cstdint>

namespace Geometry {

Rect_f projectBox(const Box & box, const Matrix4x4f & modelView, const Matrix4x4f & projection,
				  const Rect_f & viewport) {
	const auto transformation = projection * modelView;
	Rect_f projectedRect;
	projectedRect.invalidate();
	for (uint_fast8_t i = 0; i < 8; ++i) {
		const Vec3f corner = box.getCorner(static_cast<corner_t>(i));
		Vec3f result = project(corner, transformation, viewport);
		if (result.z() > 1.0) // behind the origin-plane of the viewing volume? -> invert the coordinates
			result *= -1;
		projectedRect.include(result.getX(), result.getY());
	}
	return projectedRect;
}

Frustum calcEnclosingOrthoFrustum(const Box & box, const Matrix4x4f & modelView) {
	const Matrix4x4f projection = Matrix4x4f::createScale(1.0f, 1.0f, -1.0f);
	const Matrix4x4f modelViewProjection = projection * modelView;

	const Vec3f projectedFirstCorner = modelViewProjection.transformPosition(box.getCorner(static_cast<corner_t>(0)));
	float left = projectedFirstCorner.getX();
	float right = projectedFirstCorner.getX();
	float bottom = projectedFirstCorner.getY();
	float top = projectedFirstCorner.getY();
	float near = projectedFirstCorner.getZ();
	float far = projectedFirstCorner.getZ();
	for (uint_fast8_t c = 1; c < 8; ++c) {
		const Vec3f corner = box.getCorner(static_cast<corner_t>(c));
		const Vec3f projectedCorner = modelViewProjection.transformPosition(corner);
		if (projectedCorner.getX() > right) {
			right = projectedCorner.getX();
		}
		if (projectedCorner.getX() < left) {
			left = projectedCorner.getX();
		}
		if (projectedCorner.getY() > top) {
			top = projectedCorner.getY();
		}
		if (projectedCorner.getY() < bottom) {
			bottom = projectedCorner.getY();
		}
		if (projectedCorner.getZ() > far) {
			far = projectedCorner.getZ();
		}
		if (projectedCorner.getZ() < near) {
			near = projectedCorner.getZ();
		}
	}
	if (right == left || top == bottom || near == far)
		throw std::invalid_argument("tried to create frustum with zero volume"); // better throw this than the resulting
																				 // division by zero in Matrix4x4
	Frustum frustum;
	frustum.setOrthogonal(left, right, bottom, top, near, far);
	return frustum;
}

//! \see
//http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-sphere-intersection/
std::tuple<bool, float, float> normLineSphereIntersections(const Line3f & line, const Sphere_f & sphere) {
	const Vec3f relCenter = sphere.getCenter() - line.getOrigin();
	const float tca = relCenter.dot(line.getDirection());

	const float dSquared = relCenter.lengthSquared() - tca * tca;
	const float radiusSquared = sphere.getRadius() * sphere.getRadius();
	if (dSquared > radiusSquared) {
		return std::make_tuple(false, 0.0f, 0.0f);
	} else {
		const float thc = std::sqrt(radiusSquared - dSquared);
		return std::make_tuple(true, tca - thc, tca + thc);
	}
}

std::tuple<bool, float, float> lineSphereIntersections(const Line3f & line, const Sphere_f & sphere) {
	const float length = line.getDirection().length();
	const Line3 normalizedLine(line.getOrigin(), line.getDirection() / length);
	const auto result = normLineSphereIntersections(normalizedLine, sphere);
	return std::make_tuple(std::get<0>(result), std::get<1>(result) / length, std::get<2>(result) / length);
}
}

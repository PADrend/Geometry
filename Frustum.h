/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_FRUSTUM_H
#define GEOMETRY_FRUSTUM_H

#include "Definitions.h"
#include "Matrix4x4.h"
#include "SRT.h"
#include "Vec3.h"
#include "Plane.h"
#include <stdexcept>

namespace Geometry {
template <typename value_t>
class _Box;
using Box = _Box<float>;

/**
 * Three-dimensional frustum.
 *
 * [Frustum]
 */
class Frustum {
public:
	/**
	 * @name Main
	 */
	//@{
	Frustum();
	Frustum(const Angle & angle, float ratio, float nearD, float farD);
	//@}

	/**
	 * @name Information
	 */
	//@{
	enum class intersection_t { INSIDE = 0, INTERSECT = 1, OUTSIDE = 2 };

	const Vec3 & getPos() const {
		return orientation.getTranslation();
	}
	Vec3 getDir() const {
		return orientation.getDirVector();
	}
	Vec3 getUp() const {
		return orientation.getUpVector();
	}
	float getNear() const {
		return nearValue;
	}
	float getFar() const {
		return farValue;
	}
	float getLeft() const {
		return leftValue;
	}
	float getRight() const {
		return rightValue;
	}
	float getBottom() const {
		return bottomValue;
	}
	float getTop() const {
		return topValue;
	}
	bool isOrthogonal() const {
		return orthogonal;
	}

	const Matrix4x4 & getProjectionMatrix() const {
		return projectionMatrix;
	}

	intersection_t isBoxInFrustum(const Box & b) const;
	inline bool pointInFrustum(const Vec3 & p) const;
	inline Vec3 operator[](corner_t nr) const;
	bool operator==(const Frustum & other) const;

	const Plane & getPlane(side_t side) const {
		return planes[static_cast<std::size_t>(side)];
	}
	//@}

	/**
	 * @name Modification
	 */
	//@{
	void setPerspective(const Angle & angle, float ratio, float near, float far);
	void setFrustum(float left, float right, float bottom, float top, float near, float far, bool orthogonal = false);
	void setFrustumFromAngles(const Angle & fovLeft, const Angle & fovRight, const Angle & fovBottom,
							  const Angle & fovTop, float near, float far);
	void setOrthogonal(float l, float r, float b, float t, float n, float f) {
		setFrustum(l, r, b, t, n, f, true);
	}
	void setPosition(const Vec3 & pos, const Vec3 & dir, const Vec3 & up);

protected:
	void recalculateCornersAndPlanes();
	//@}

	// ---- Data
private:
	float leftValue;
	float rightValue;
	float bottomValue;
	float topValue;
	// Do not use "near", or "far" as variable identifiers here. They already seem to be defined on Windows.
	float nearValue;
	float farValue;
	bool orthogonal;
	SRT orientation;

	Matrix4x4 projectionMatrix;

	Vec3 corners[8];
	Plane planes[6];

	/**
	 * Corner that is the nearest one of an axis-aligned bounding box to the corresponding frustum plane.
	 * The values are calculated once per frame and re-used for box tests.
	 */
	corner_t negCorner[8];
	//! @see negCorner
	corner_t posCorner[8];
};
// --------------------------------------------------------------------------------------------------------------------------

inline bool Frustum::pointInFrustum(const Vec3 & p) const {
	return planes[0].planeTest(p) >= 0 && planes[1].planeTest(p) >= 0 && planes[2].planeTest(p) >= 0
			&& planes[3].planeTest(p) >= 0 && planes[4].planeTest(p) >= 0 && planes[5].planeTest(p) >= 0;
}
inline Vec3 Frustum::operator[](corner_t nr) const {
	return corners[static_cast<std::size_t>(nr)];
}
}

#endif /* GEOMETRY_FRUSTUM_H */

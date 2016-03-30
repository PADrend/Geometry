/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_RAYBOXINTERSECTION_H
#define GEOMETRY_RAYBOXINTERSECTION_H

#include "Box.h"
#include "Line.h"
#include "Vec3.h"
#include <cstdint>

namespace Geometry {
namespace Intersection {

/**
 * Fast intersection tests of a ray with multiple axis-aligned boxes. A class is
 * used instead of a single function because precomputations for the ray have to
 * be carried out before the test can be performed.
 * Based on the article:
 * Martin Eisemann, Marcus Magnor, Thorsten Grosch, Stefan Müller:
 * Fast Ray/Axis-Aligned Bounding Box Overlap Tests using Ray Slopes.
 * Journal of Graphics, GPU, and Game Tools 12, 4, pp. 35-46, 2007.
 *
 * @note Parts of the public domain source code accompanying the article were
 * used.
 * @tparam value_t Either @c float or @c double
 * @author Benjamin Eikel
 * @date 2013-02-19
 */
template <typename value_t>
class Slope {
private:
	typedef _Vec3<value_t> vec_t;
	typedef _Box<value_t> box_t;
	typedef _Ray<vec_t> ray_t;

	//! The original ray.
	const ray_t ray;

	//! The inverse of the ray direction (1 / direction).
	const vec_t inverseDirection;

	/**
	* Classification of the sign of the direction components (M = minus,
	* P = plus, O = zero).
	*/
	enum class SlopeClassification : std::uint8_t {
		MMM,
		MMP,
		MPM,
		MPP,
		PMM,
		PMP,
		PPM,
		PPP,
		POO,
		MOO,
		OPO,
		OMO,
		OOP,
		OOM,
		OMM,
		OMP,
		OPM,
		OPP,
		MOM,
		MOP,
		POM,
		POP,
		MMO,
		MPO,
		PMO,
		PPO
	};
	//! Ray slope classification
	SlopeClassification classification;

	//! Ray slope
	value_t xByY, xByZ, yByX, yByZ, zByY, zByX;
	value_t c_xy, c_xz, c_yx, c_yz, c_zx, c_zy;

public:
	//! Create a new instance and associate it with a ray.
	Slope(const ray_t & ray);

	/**
	 * Check if the associated ray intersects a box.
	 *
	 * @param box Axis-aligned box
	 * @return @c true if the ray and the box intersect, @c false if there
	 * is no intersection
	 * @note Function in the article is called "slope"
	 * @note This function is usually a little bit faster than
	 * getRayBoxIntersection()
	 */
	bool isRayIntersectingBox(const box_t & box) const;

	/**
	 * Calculate the intersection between the associated ray and a box.
	 *
	 * @param box Axis-aligned box
	 * @param[out] intersection Intersection point between ray and box. It
	 * is given as the ray parameter and set only if there is an
	 * intersection.
	 * @return @c true if the ray and the box intersect, @c false if there
	 * is no intersection
	 * @note Function in the article is called "slopeint_mul"
	 * @note This function is usually a little bit slower than
	 * isRayIntersectingBox()
	 */
	bool getRayBoxIntersection(const box_t & box, value_t & intersection) const;

	//! Return the stored ray.
	const ray_t & getRay() const {
		return ray;
	}
};
}
}

#endif /* GEOMETRY_RAYBOXINTERSECTION_H */

/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_TETRAHEDRON_H
#define GEOMETRY_TETRAHEDRON_H

#include "Plane.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix4x4.h"
#include <istream>
#include <limits>
#include <ostream>

namespace Geometry {

/**
 * Tetrahedron defined by four vertices.
 *
 * @author Benjamin Eikel, Claudius Jähn
 * @date 2011-12-03
 * @verbatim
 *            b
 *           /|\
 *         /  |  \
 *       /    | - -\--- pA
 *     /  pD  | pC   \
 *    c- - - -|- - - -d
 *     \      |      /
 *       \    | - - /----- pB
 *         \  |   /
 *           \| /
 *            a
 * @endverbatim
 */
template <typename T_>
class Tetrahedron {
public:
	typedef T_ value_t;
	typedef _Vec3<value_t> vec3_t;
	typedef _Vec4<value_t> vec4_t;
	typedef _Matrix4x4<value_t> matrix4x4_t;

private:
	//! First vertex
	vec3_t a;
	//! Second vertex
	vec3_t b;
	//! Third vertex
	vec3_t c;
	//! Fourth vertex
	vec3_t d;

	//! Plane through @a b, @a d, and @a c (counterclockwise).
	_Plane<value_t> pA;
	//! Plane through @a a, @a c, and @a d (counterclockwise).
	_Plane<value_t> pB;
	//! Plane through @a a, @a d, and @a b (counterclockwise).
	_Plane<value_t> pC;
	//! Plane through @a a, @a b, and @a c (counterclockwise).
	_Plane<value_t> pD;

	void updatePlanes() {
		pA = _Plane<value_t>(b, d, c);
		pB = _Plane<value_t>(a, c, d);
		pC = _Plane<value_t>(a, d, b);
		pD = _Plane<value_t>(a, b, c);
	}

public:
	//! @name Creation
	//@{
	//! Default constructor.
	Tetrahedron() : a(), b(), c(), d(), pA(), pB(), pC(), pD() {
	}

	//! Default constructor with vertices.
	Tetrahedron(const vec3_t & vertexA, const vec3_t & vertexB, const vec3_t & vertexC, const vec3_t & vertexD)
			: a(vertexA), b(vertexB), c(vertexC), d(vertexD), pA(), pB(), pC(), pD() {
		updatePlanes();
	}
	//@}

	//! @name Operators
	//@{
	//! Equality operator.
	bool operator==(const Tetrahedron & other) const {
		// No need to test the planes here.
		return a == other.a && b == other.b && c == other.c && d == other.d;
	}
	//@}

	//! @name Information
	//@{
	//! Return the first vertex.
	const vec3_t & getVertexA() const {
		return a;
	}
	//! Return the second vertex.
	const vec3_t & getVertexB() const {
		return b;
	}
	//! Return the third vertex.
	const vec3_t & getVertexC() const {
		return c;
	}
	//! Return the fourth vertex.
	const vec3_t & getVertexD() const {
		return d;
	}

	//! Return the first face.
	Triangle<vec3_t> getFaceA() const {
		return Triangle<vec3_t>(b, d, c);
	}
	//! Return the second face.
	Triangle<vec3_t> getFaceB() const {
		return Triangle<vec3_t>(a, c, d);
	}
	//! Return the third face.
	Triangle<vec3_t> getFaceC() const {
		return Triangle<vec3_t>(a, d, b);
	}
	//! Return the fourth face.
	Triangle<vec3_t> getFaceD() const {
		return Triangle<vec3_t>(a, b, c);
	}

	//! Return the first plane.
	const _Plane<value_t> & getPlaneA() const {
		return pA;
	}
	//! Return the second plane.
	const _Plane<value_t> & getPlaneB() const {
		return pB;
	}
	//! Return the third plane.
	const _Plane<value_t> & getPlaneC() const {
		return pC;
	}
	//! Return the fourth plane.
	const _Plane<value_t> & getPlaneD() const {
		return pD;
	}
	//@}

	//! @name Modification
	//@{
	//! Assign the first vertex.
	void setVertexA(const vec3_t & v) {
		a = v;
		updatePlanes();
	}
	//! Assign the second vertex.
	void setVertexB(const vec3_t & v) {
		b = v;
		updatePlanes();
	}
	//! Assign the third vertex.
	void setVertexC(const vec3_t & v) {
		c = v;
		updatePlanes();
	}
	//! Assign the fourth vertex.
	void setVertexD(const vec3_t & v) {
		d = v;
		updatePlanes();
	}
	//@}

	//! @name Calculation
	//@{

	/**
	 * Calculate the barycentric coordinates of the given point @a p with reference to this tetrahedron.
	 *
	 * @param p Position in Cartesian coordinates that should be converted to barycentric coordinates
	 * @return pair(success as bool,Barycentric coordinates as vec4) success is false, iff the tetrahedron is
	 *degenerated.
	 * @see http://steve.hollasch.net/cgindex/geometry/ptintet.html
	 */
	std::pair<bool, vec4_t> calcBarycentricCoordinates(const vec3_t & _p) const {
		const vec4_t p(_p, 1);
		const vec4_t v0(getVertexA(), 1);
		const vec4_t v1(getVertexB(), 1);
		const vec4_t v2(getVertexC(), 1);
		const vec4_t v3(getVertexD(), 1);
		const value_t det = matrix4x4_t(v0, v1, v2, v3).det();
		if (det == 0) { // degenerate tetrahedron
			return std::make_pair(false, vec4_t());
		}
		const value_t d0 = matrix4x4_t(p, v1, v2, v3).det();
		const value_t d1 = matrix4x4_t(v0, p, v2, v3).det();
		const value_t d2 = matrix4x4_t(v0, v1, p, v3).det();
		const value_t d3 = matrix4x4_t(v0, v1, v2, p).det();

		return std::make_pair(true, vec4_t(d0 / det, d1 / det, d2 / det, d3 / det));
	}

	vec3_t calcPointFromBarycentricCoordinates(const vec4_t & bary) const {
		return (getVertexA() * bary.x() + getVertexB() * bary.y() + getVertexC() * bary.z() + getVertexD() * bary.w())
				/ (bary.x() + bary.y() + bary.z() + bary.w());
	}

	/**
	 * Returns the volume of the tetrahedron.
	 * @return The volume.
	 */
	float calcVolume() const {
		const vec3_t ad = getVertexA() - getVertexD();
		const vec3_t bd = getVertexB() - getVertexD();
		const vec3_t cd = getVertexC() - getVertexD();
		return std::abs(ad.cross(bd).dot(cd)) / 6.0f;
	}

	/**
	 * Check if the given point is inside, or on the surface of this tetrahedron.
	 *
	 * @param[in] point Query position
	 * @param[in] epsilon Safety margin around the tetrahedron that is still considered to be inside.
	 * Set this to zero, if you do not want to have a safety margin.
	 * @return @c true if the point is inside or on the surface, @c false if it is outside
	 */
	bool containsPoint(const vec3_t & point, value_t epsilon) const {
		return pA.planeTest(point) <= epsilon && pB.planeTest(point) <= epsilon && pC.planeTest(point) <= epsilon
				&& pD.planeTest(point) <= epsilon;
	}

	/**
	 * Calculate the minimum squared distance of the point to the tetrahedron.
	 * If the point is inside or on the surface of the tetrahedron, the distance will be zero.
	 *
	 * @param[in] point Query position
	 * @return Minimum squared distance of the point to the tetrahedron
	 */
	value_t distanceSquared(const vec3_t & point) const {
		// Implementation taken from
		// Christer Ericson: Real-Time Collision Detection. Morgan Kaufmann, 2004.

		// Start out assuming point inside all halfspaces, so closest to itself
		value_t bestSquaredDist = std::numeric_limits<value_t>::max();

		// If point outside face abc then compute closest point on abc
		if (pD.planeTest(point) >= 0.0f) {
			const value_t squaredDist = getFaceD().distanceSquared(point);
			// Update best closest point if (squared) distance is less than current best
			if (squaredDist < bestSquaredDist) {
				bestSquaredDist = squaredDist;
			}
		}
		// Repeat test for face acd
		if (pB.planeTest(point) >= 0.0f) {
			const value_t squaredDist = getFaceB().distanceSquared(point);
			if (squaredDist < bestSquaredDist) {
				bestSquaredDist = squaredDist;
			}
		}
		// Repeat test for face adb
		if (pC.planeTest(point) >= 0.0f) {
			const value_t squaredDist = getFaceC().distanceSquared(point);
			if (squaredDist < bestSquaredDist) {
				bestSquaredDist = squaredDist;
			}
		}
		// Repeat test for face bdc
		if (pA.planeTest(point) >= 0.0f) {
			const value_t squaredDist = getFaceA().distanceSquared(point);
			if (squaredDist < bestSquaredDist) {
				bestSquaredDist = squaredDist;
			}
		}
		if (bestSquaredDist == std::numeric_limits<value_t>::max()) {
			bestSquaredDist = 0;
		}
		return bestSquaredDist;
	}
	//@}

	//! @name Serialization
	//@{
	friend std::ostream & operator<<(std::ostream & out, const Tetrahedron<value_t> & tetrahedron) {
		return out << tetrahedron.a << ' ' << tetrahedron.b << ' ' << tetrahedron.c << ' ' << tetrahedron.d;
	}
	friend std::istream & operator>>(std::istream & in, Tetrahedron<value_t> & tetrahedron) {
		in >> tetrahedron.a >> tetrahedron.b >> tetrahedron.c >> tetrahedron.d;
		tetrahedron.updatePlanes();
		return in;
	}
	//@}
};
}

#endif /* GEOMETRY_TETRAHEDRON_H */

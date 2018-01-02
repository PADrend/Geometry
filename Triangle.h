/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_TRIANGLE_H
#define GEOMETRY_TRIANGLE_H

#include "Vec3.h"
#include <istream>
#include <ostream>
#include <limits>

namespace Geometry {

/**
 * Triangle defined by three vertices.
 *
 * @author Benjamin Eikel
 * @date 2011-12-02
 * @verbatim
 *      c
 *      X
 *     / \
 *    /   \
 *   X-----X
 *   a     b
 * @endverbatim
 */
template <typename T_>
class Triangle {
public:
	using vec_t = T_;
	using value_t = typename vec_t::value_t;

private:
	//! First vertex
	vec_t a;
	//! Second vertex
	vec_t b;
	//! Third vertex
	vec_t c;

public:
	//! @name Creation
	//@{
	//! Default constructor with vertices.
	Triangle(const vec_t & vertexA, const vec_t & vertexB, const vec_t & vertexC) : a(vertexA), b(vertexB), c(vertexC) {
	}
	//@}

	//! @name Operators
	//@{
	//! Equality operator.
	bool operator==(const Triangle & other) const {
		return a == other.a && b == other.b && c == other.c;
	}
	//@}

	//! @name Information
	//@{
	//! Return the first vertex.
	const vec_t & getVertexA() const {
		return a;
	}
	//! Return the second vertex.
	const vec_t & getVertexB() const {
		return b;
	}
	//! Return the third vertex.
	const vec_t & getVertexC() const {
		return c;
	}

	//! Return the first edge.
	vec_t getEdgeAB() const {
		return (b - a);
	}
	//! Return the second edge.
	vec_t getEdgeBC() const {
		return (c - b);
	}
	//! Return the third edge.
	vec_t getEdgeCA() const {
		return (a - c);
	}

	//! Return the first reverse edge.
	vec_t getEdgeBA() const {
		return (a - b);
	}
	//! Return the second reverse edge.
	vec_t getEdgeCB() const {
		return (b - c);
	}
	//! Return the third reverse edge.
	vec_t getEdgeAC() const {
		return (c - a);
	}
	//@}

	//! @name Modification
	//@{
	//! Assign the first vertex.
	void setVertexA(const vec_t & v) {
		a = v;
	}
	//! Assign the second vertex.
	void setVertexB(const vec_t & v) {
		b = v;
	}
	//! Assign the third vertex.
	void setVertexC(const vec_t & v) {
		c = v;
	}
	//@}

	//! @name Calculation
	//@{

	/**
	 * Return the area of the triangle.
	 *
	 * @return The area.
	 */
	value_t calcArea() const {
		// Triangle area is half of the area of a parallelogram.
		return 0.5 * getEdgeBA().cross(getEdgeBC()).length();
	}

	/**
	 * Return the unit length normal of this triangle.
	 *
	 * @return Normal vector
	 */
	vec_t calcNormal() const {
		return getEdgeAB().cross(getEdgeAC()).normalize();
	}

	/**
	 * Calculate a point inside the triangle based on two barycentric
	 * coordinates with vertex A as origin.
	 *
	 * @param u Barycentric coordinate in AB direction
	 * @param v Barycentric coordinate in AC direction
	 * @return Point corresponding to the given barycentric coordinates
	 */
	vec_t calcPoint(value_t u, value_t v) const {
		return a + getEdgeAB() * u + getEdgeAC() * v;
	}

	/**
	 * Calculate the barycentric coordinates of the given point @a p with reference to this triangle.
	 *
	 * @param p Position in Cartesian coordinates that should be converted to barycentric coordinates
	 * @return Barycentric coordinates
	 * @see http://science.kennesaw.edu/~plaval/math4490/barycentric.pdf
	 * @verbatim
	 *           c
	 *           X
	 *          /.\
	 *         / . \
	 *        /  .  \      alpha = A_a / A
	 *       /A_b.A_a\     beta = A_b / A
	 *      /   .X.   \    gamma = A_c / A
	 *     /  .  p  .  \
	 *    /.    A_c    .\
	 *   X---------------X
	 *   a               b
	 * @endverbatim
	 */
	vec_t calcBarycentricCoordinates(const vec_t & p) const {
		const vec_t ba = b - a;
		const vec_t ca = c - a;
		const vec_t cb = c - b;
		const vec_t ac = a - c;
		const vec_t pa = p - a;
		const vec_t pb = p - b;
		const vec_t pc = p - c;

		// Normal of triangle (a, b, c)
		const vec_t n = ba.cross(ca);
		// Normal of triangle (b, c, p)
		const vec_t na = cb.cross(pb);
		// Normal of triangle (c, a, p)
		const vec_t nb = ac.cross(pc);
		// Normal of triangle (a, b, p)
		const vec_t nc = ba.cross(pa);

		const value_t lengthSquared = n.lengthSquared();

		return vec_t(n.dot(na) / lengthSquared, n.dot(nb) / lengthSquared, n.dot(nc) / lengthSquared);
	}

	/**
	 * Calculate the circumcenter of the triangle, 
	 * i.e., the center of the sphere that is defined by the three triangle vertices.
	 *
	 * @see http://mathworld.wolfram.com/BarycentricCoordinates.html
	 * @return Circumcenter of the triangle
	 */
	vec_t calcCircumcenter() const {
		const value_t bc2 = getEdgeBC().lengthSquared();
		const value_t ca2 = getEdgeCA().lengthSquared();
		const value_t ab2 = getEdgeAB().lengthSquared();
		
		const vec_t bc(bc2*(ca2+ab2-bc2), ca2*(ab2+bc2-ca2), ab2*(bc2+ca2-ab2));
		const value_t sum = bc.x() + bc.y() + bc.z();				
		
		return a * bc.x()/sum + b * bc.y()/sum + c * bc.z()/sum;
	}

	/**
	 * Return the point of this triangle that has minimum distance to a point @a p.
	 *
	 * Implementation taken from
	 * Christer Ericson: Real-Time Collision Detection. Morgan Kaufmann, 2004.
	 *
	 * @param[in] p Query position
	 * @param[out] barycentric Barycentric coordinates of the point inside the triangle
	 * @return Point of this triangle with minimum distance to the query position
	 * @verbatim
	 *         c
	 *         X   result
	 *        / \  |      p
	 *       /   \ /  _,--X
	 *      /     X--´
	 *     /       \
	 *    /         \
	 *   X-----------X
	 *   a           b
	 * @endverbatim
	 */
	vec_t closestPoint(const vec_t & p, _Vec3<value_t> & barycentric) const {
		// Check if p in vertex region outside a
		const vec_t ab = b - a;
		const vec_t ac = c - a;
		const vec_t ap = p - a;
		const value_t d1 = ab.dot(ap);
		const value_t d2 = ac.dot(ap);
		if (d1 <= 0 && d2 <= 0) {
			barycentric.setValue(1, 0, 0);
			return a; // barycentric coordinates (1, 0, 0)
		}

		// Check if p in vertex region outside b
		const vec_t bp = p - b;
		const value_t d3 = ab.dot(bp);
		const value_t d4 = ac.dot(bp);
		if (d3 >= 0 && d4 <= d3) {
			barycentric.setValue(0, 1, 0);
			return b; // barycentric coordinates (0, 1, 0)
		}

		// Check if p in edge region of ab, if so return projection of p onto ab
		const value_t vc = d1 * d4 - d3 * d2;
		if (vc <= 0 && d1 >= 0 && d3 <= 0) {
			const value_t v = d1 / (d1 - d3);
			barycentric.setValue(1 - v, v, 0);
			return a + ab * v; // barycentric coordinates (1 - v, v, 0)
		}

		// Check if p in vertex region outside c
		const vec_t cp = p - c;
		const value_t d5 = ab.dot(cp);
		const value_t d6 = ac.dot(cp);
		if (d6 >= 0 && d5 <= d6) {
			barycentric.setValue(0, 0, 1);
			return c; // barycentric coordinates (0, 0, 1)
		}

		// Check if p in edge region of ac, if so return projection of p onto ac
		const value_t vb = d5 * d2 - d1 * d6;
		if (vb <= 0 && d2 >= 0 && d6 <= 0) {
			const value_t w = d2 / (d2 - d6);
			barycentric.setValue(1 - w, 0, w);
			return a + ac * w; // barycentric coordinates (1 - w, 0, w)
		}

		// Check if p in edge region of bc, if so return projection of p onto bc
		const value_t va = d3 * d6 - d5 * d4;
		if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0) {
			const value_t w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
			barycentric.setValue(0, 1 - w, w);
			return b + (c - b) * w; // barycentric coordinates (0, 1 - w, w)
		}

		// p inside face region. Compute result through its barycentric coordinates (u, v, w)
		const value_t denom = 1 / (va + vb + vc);
		const value_t v = vb * denom;
		const value_t w = vc * denom;
		barycentric.setValue(1 - v - w, v, w);
		return a + ab * v + ac * w; // = u * a + v * b + w * c, u = va * denom = 1.0f - v - w
	}

	/**
	 * Calculate the squared distance of a point @a p to this triangle.
	 *
	 * @param p Query position
	 * @return Squared distance of @a p to the triangle
	 * @verbatim
	 *         c
	 *         X
	 *        / \         p
	 *       /   \    _,--X
	 *      /     X--´|
	 *     /       \  distance
	 *    /         \
	 *   X-----------X
	 *   a           b
	 * @endverbatim
	 */
	value_t distanceSquared(const vec_t & p) const {
		_Vec3<value_t> barycentric;
		return closestPoint(p, barycentric).distanceSquared(p);
	}

	/**
	 * Check if the triangle is degenerate. The check is done by testing if
	 * the area of the triangle is nearly zero.
	 *
	 * @return @c true if this is a degenerate triangle, @c false otherwise.
	 */
	bool isDegenerate() const {
		const value_t minimumArea = std::numeric_limits<value_t>::epsilon();
		// area = 1/2 * |AB x AC|
		// area^2 = 1/4 * |AB x AC|^2
		// 4 * area^2 = |AB x AC|^2
		return (4.0 * minimumArea * minimumArea) > getEdgeAB().cross(getEdgeAC()).lengthSquared();
	}

	//@}

	//! @name Serialization
	//@{
	friend std::ostream & operator<<(std::ostream & out, const Triangle<vec_t> & triangle) {
		return out << triangle.a << ' ' << triangle.b << ' ' << triangle.c;
	}
	friend std::istream & operator>>(std::istream & in, Triangle<vec_t> & triangle) {
		return in >> triangle.a >> triangle.b >> triangle.c;
	}
	//@}
};
}

#endif /* GEOMETRY_TRIANGLE_H */

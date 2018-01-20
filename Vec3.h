/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_VEC3_H
#define GEOMETRY_VEC3_H

#include "Interpolation.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace Geometry {

/*!
 * Three-dimensional vector.
 *
 *  [_Vec3]
 */
template <typename T_>
class _Vec3 {
public:
	typedef T_ value_t;
	typedef _Vec3<value_t> vec3_t;

	/*!
	 * comparator class for comparing vectors when using as key in a map or set
	 * comparison is done by values only not by pointers so different vectors with same values are equal
	 */
	class Comparator {
	public:
		bool operator()(const vec3_t & a, const vec3_t & b) const {
			if (a.getX() != b.getX())
				return a.getX() < b.getX();
			if (a.getY() != b.getY())
				return a.getY() < b.getY();
			return a.getZ() < b.getZ();
		}

		bool operator()(const vec3_t * a, const vec3_t * b) const {
			return (*this)(*a, *b);
		}
	};

	/*!
	 * @name Main
	 */
	//@{
	/*! [ctor]
		@note all components are initialized with 0	*/
	_Vec3() {
		setValue(static_cast<value_t>(0));
	}

	/*!	[ctor]
		@param x used to initialize the first component
		@param y used to initialize the second component
		@param z used to initialize the third component	*/
	_Vec3(value_t _x, value_t _y, value_t _z) {
		setValue(_x, _y, _z);
	}

	/*!	[ctor]
		@param v used to initialize the new Vec3
		@note values are converted by casting	*/
	template <class other_t>
	explicit _Vec3(const _Vec3<other_t> & v) {
		setValue(static_cast<value_t>(v.x()), static_cast<value_t>(v.y()), static_cast<value_t>(v.z()));
	}

	/*!	[ctor]
		interpolation between v1 and sv2, according to the factor blend
		@param v1, v2 vectors to be interpolated
		@param blend blendingfactor (should be between 0 and 1)	*/
	_Vec3(const vec3_t & v1, const vec3_t & v2, value_t blend) {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		*this = Interpolation::linear(v1, v2, blend);
	}

	/*!	[ctor]
		@param v the first three entries of v are used for for initialisation	*/
	explicit _Vec3(const value_t * v) {
		setValue(v[0], v[1], v[2]);
	}

	/*!	[ctor]
		@param v used for initialisation of all components	*/
	explicit _Vec3(const value_t v) {
		setValue(v);
	}
	//@}

	/*!
	 * @name Information
	 */
	//@{
	/*! array-access operator */
	inline value_t & operator[](uint_fast8_t nr) {
		return vec[nr];
	}

	/*! const array-access operator	*/
	inline const value_t & operator[](uint_fast8_t nr) const {
		return vec[nr];
	}

	/*! @return the underlying array of this	*/
	inline const value_t * getVec() const {
		return vec;
	}

	/*! @return the x component of this
		@note equal to this[0]	*/
	inline const value_t getX() const {
		return vec[0];
	}
	inline const value_t x() const {
		return vec[0];
	}

	/*! @return the i-th component of this	*/
	inline const value_t get(const int & i) const {
		return vec[i];
	}

	/*! @return the y component of this
		@note equal to this[1]	*/
	inline const value_t getY() const {
		return vec[1];
	}
	inline const value_t y() const {
		return vec[1];
	}

	/*! @return the z component of this
		@note equal to this[2]	*/
	inline const value_t getZ() const {
		return vec[2];
	}
	inline const value_t z() const {
		return vec[2];
	}

	/*! @return the maximum absolute value of the components of this	*/
	inline value_t maxAbsValue() const {
		value_t maxi = std::abs(vec[0]);
		if (vec[1] > maxi)
			maxi = vec[1];
		else if (-vec[1] > maxi)
			maxi = -vec[1];
		if (vec[2] > maxi)
			maxi = vec[2];
		else if (-vec[2] > maxi)
			maxi = -vec[2];
		return maxi;
	}

	/*! @return the length of this	*/
	template <typename float_t>
	float_t length() const {
		static_assert(std::is_floating_point<float_t>::value, "template argument not a floating point type");
		return std::sqrt(static_cast<float_t>(dot(*this)));
	}
	value_t length() const {
		return length<value_t>();
	}

	/*! @return the quadrated length of this	*/
	inline value_t lengthSquared() const {
		return dot(*this);
	}

	/*! calculates the dotproduct of this and another Vec3
		@param p the vector to be used for calculation
		@return the dotproduct of this and p	*/
	inline value_t dot(const vec3_t & p) const {
		return (vec[0] * p.vec[0] + vec[1] * p.vec[1] + vec[2] * p.vec[2]);
	}

	/*! calculates the distance between this and another Vec3
	 * @param p the vector to be used for calculation
	 * @return the distance between this and p	*/
	template <typename float_t>
	inline float_t distance(const vec3_t & p) const {
		static_assert(std::is_floating_point<float_t>::value, "template argument not a floating point type");
		return std::sqrt(static_cast<float_t>(distanceSquared(p)));
	}
	value_t distance(const vec3_t & p) const {
		return distance<value_t>(p);
	}

	/*! calculates the squared distance between this and another Vec3
	 * @param p the vector to be used for calculation
	 * @return the squared distance between this and p	*/
	value_t distanceSquared(const vec3_t & p) const {
		value_t dx = vec[0] - p.vec[0];
		value_t dy = vec[1] - p.vec[1];
		value_t dz = vec[2] - p.vec[2];
		return dx * dx + dy * dy + dz * dz;
	}

	/*! @return vec == (0,0,0)	*/
	bool isZero() const {
		return vec[0] == 0 && vec[1] == 0 && vec[2] == 0;
	}

	/*! @return   on plane    = 0
	 *           below plane < 0
	 *           above plane > 0
	 */
	inline value_t planeTest(const vec3_t & planePosition, const vec3_t & planeNormal) const {
		return ((*this) - planePosition).dot(planeNormal);
	}

	/**
	 * Determine the location of the point relative to the given plane.
	 *
	 * @param planeNormal Normal vector of the plane
	 * @param planeDist Distance of the plane to the origin
	 * @return Distance of the point to the plane
	 */
	inline value_t planeTest(const vec3_t & planeNormal, const value_t planeDist) const {
		return dot(planeNormal) - planeDist;
	}
	//@}

	/*!
	 * @name Modification
	 */
	//@{
	/*! sets the components of this
		@param _x used to set the first component
		@param _y used to set the second component
		@param _z used to set the third component */
	inline void setValue(const value_t _x, const value_t _y, const value_t _z) {
		vec[0] = _x;
		vec[1] = _y;
		vec[2] = _z;
	}

	/*! sets all components of this
	 * @param t used to set all components	*/
	inline void setValue(const value_t t) {
		setValue(t, t, t);
	}

	/*! sets the components of this
		@param v the first three values of v are used to set the components of this	*/
	inline void setValue(const value_t * v) {
		setValue(v[0], v[1], v[2]);
	}

	/*! sets the x component
		@param x the value the x component of this is set to
		@note equal to this[0]=x	*/
	inline void setX(const value_t _x) {
		vec[0] = _x;
	}
	inline void x(const value_t v) {
		vec[0] = v;
	}

	/*! sets the y component
		@param y the value the y component of this is set to
		@note equal to this[1]=y	*/
	inline void setY(const value_t _y) {
		vec[1] = _y;
	}
	inline void y(const value_t v) {
		vec[1] = v;
	}

	/*! sets the z component
		@param z the value the z component of this is set to
		@note equal to this[2]=z	*/
	inline void setZ(const value_t _z) {
		vec[2] = _z;
	}
	inline void z(const value_t v) {
		vec[2] = v;
	}

	/*! adds another Vec3 to this
		@param the Vec3 to be added
		@return this
		@note this will be modified	*/
	inline vec3_t & operator+=(const vec3_t & p) {
		vec[0] += p.vec[0];
		vec[1] += p.vec[1];
		vec[2] += p.vec[2];
		return *this;
	}

	/*! subtracts another Vec3 from this
		@param the Vec3 to be substracted
		@return this
		@note this will be modified	*/
	inline vec3_t & operator-=(const vec3_t & p1) {
		vec[0] -= p1.vec[0];
		vec[1] -= p1.vec[1];
		vec[2] -= p1.vec[2];
		return *this;
	}

	/*! multiplies this componentwise with a scalar value
		@param f the scalar
		@return this
		@note this will be modified	*/
	vec3_t & operator*=(const value_t & f) {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		vec[0] *= f;
		vec[1] *= f;
		vec[2] *= f;
		return *this;
	}

	/*! divides this componentwise by a scalar value
		@param f the scalar
		@return a new Vec3
		@note this will be modified	*/
	vec3_t & operator/=(const value_t & f) {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		vec[0] /= f;
		vec[1] /= f;
		vec[2] /= f;
		return *this;
	}

	/*! normalizes this
		@return this
		@note this will be modified	*/
	inline vec3_t & normalize() {
		value_t l = length();
		if (l == 0) {
			std::cerr << "Cannot normalize a vector with zero length." << std::endl;
			return *this;
		}
		/* --- */
		if (l == 1.0)
			return *this;
		l = 1 / l;
		vec[0] *= l;
		vec[1] *= l;
		vec[2] *= l;
		return *this;
	}

	/*! Change direction as reflected on a plane
		@param planeNormal	*/
	inline vec3_t & reflect(const vec3_t & planeNormal) {
		vec3_t n = vec3_t(planeNormal).normalize();
		(*this) -= n * 2 * dot(n);
		return *this;
	}
	//@}

	/*!
	 * @name Creation
	 */
	//@{
	/*! adds another Vec3 to this
		@param the Vec3 to be added
		@return a new Vec3
		@note this remains unchanged	*/
	const vec3_t operator+(const vec3_t & p) const {
		return vec3_t(vec[0] + p.vec[0], vec[1] + p.vec[1], vec[2] + p.vec[2]);
	}

	/*! subtracts another Vec3 from this
		@param the Vec3 to be substracted
		@return a new Vec3
		@note this remains unchanged	*/
	const vec3_t operator-(const vec3_t & p) const {
		return vec3_t(vec[0] - p.vec[0], vec[1] - p.vec[1], vec[2] - p.vec[2]);
	}

	/*! @return a new Vec3 representing the negation of this
		@note this remains unchanged	*/
	const vec3_t operator-() const {
		return vec3_t(-vec[0], -vec[1], -vec[2]);
	}

	/*! multiplies this componentwise with a scalar value
		@param f the scalar
		@return a new Vec3
		@note this remains unchanged	*/
	const vec3_t operator*(const value_t & f) const {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		return vec3_t(vec[0] * f, vec[1] * f, vec[2] * f);
	}

	/*! divides this componentwise by a scalar value
		@param f the scalar
		@return a new Vec3
		@note this remains unchanged	*/
	const vec3_t operator/(const value_t & f) const {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		return vec3_t(vec[0] / f, vec[1] / f, vec[2] / f);
	}

	/*! calculates the crossproduct of this and another Vec3
		@param p the vector to be used for calculation
		@return a vector standing orthogonal on the plane defined by this and p
		@note the returned vector is a normalvector iff this and p are normalvectors	*/
	inline vec3_t cross(const vec3_t & p) const {
		return vec3_t(vec[1] * p.vec[2] - vec[2] * p.vec[1], vec[2] * p.vec[0] - vec[0] * p.vec[2],
					  vec[0] * p.vec[1] - vec[1] * p.vec[0]);
	}

	/*! returns a normalized copy of this.	*/
	inline vec3_t getNormalized() const {
		vec3_t n = *this;
		return n.normalize();
	}

	/*! returns a normalized copy of this.	*/
	inline vec3_t getAbs() const {
		return vec3_t(std::abs(vec[0]), std::abs(vec[1]), std::abs(vec[2]));
	}

	/*! calculates the projection of this on a plane.
		@param planeBase plane base point
		@param planeNormal planeNormal	*/
	inline vec3_t getProjection(const vec3_t & planeBase, const vec3_t & planeNormal) const {
		vec3_t d = planeNormal.getNormalized();
		return *this - d * d.dot(*this - planeBase);
	}

	//@}

	/*!
	 * @name Comparators
	 */
	//@{
	/*! compares this with other componentwise
		@param other the object to compare with
		@param epsilon the maximum allowed difference between the component pairs
		@return true iff any the absolute difference between any pai of components is larger than epsilon */
	inline bool equals(const vec3_t & other, value_t epsilon) const {
		return std::abs(x() - other.x()) <= epsilon && std::abs(y() - other.y()) <= epsilon
				&& std::abs(z() - other.z()) <= epsilon;
	}
	/*! compares this with another Vec3
		@param p the Vec3 this shall be compared to
		@return true iff all components of this are equal to the corresponding ones in p	*/
	inline bool operator==(const vec3_t & p) const {
		return ((p.vec[0] == vec[0] && p.vec[1] == vec[1] && p.vec[2] == vec[2]));
	}

	/*! compares this with another Vec3
		@param p the Vec3 this shall be compared to
		@return true iff at least one component of this is not equal to the corresponding one in p	*/
	inline bool operator!=(const vec3_t & p) const {
		return ((p.vec[0] != vec[0] || p.vec[1] != vec[1] || p.vec[2] != vec[2]));
	}

	static inline vec3_t pairwiseMin(const vec3_t & a, const vec3_t & b) {
		return vec3_t(a.vec[0] < b.vec[0] ? a.vec[0] : b.vec[0], a.vec[1] < b.vec[1] ? a.vec[1] : b.vec[1],
					  a.vec[2] < b.vec[2] ? a.vec[2] : b.vec[2]);
	}
	static inline vec3_t pairwiseMax(const vec3_t & a, const vec3_t & b) {
		return vec3_t(a.vec[0] > b.vec[0] ? a.vec[0] : b.vec[0], a.vec[1] > b.vec[1] ? a.vec[1] : b.vec[1],
					  a.vec[2] > b.vec[2] ? a.vec[2] : b.vec[2]);
	}
	//@}

	/*!
	 * @name Conversion
	 */
	//@{
	/*! copies the components of this into the first three values of an array
		@param the arra in which the components shall be copied	*/
	inline void toArray(value_t * t) const {
		t[0] = vec[0];
		t[1] = vec[1];
		t[2] = vec[2];
	}
	//@}

	// ---- Data
private:
	value_t vec[3];

	//! @name Serialization
	//@{
public:
	friend std::ostream & operator<<(std::ostream & out, const vec3_t & v) {
		return out << v.vec[0] << ' ' << v.vec[1] << ' ' << v.vec[2];
	}
	friend std::istream & operator>>(std::istream & in, vec3_t & v) {
		return in >> v.vec[0] >> v.vec[1] >> v.vec[2];
	}
	//@}
};
typedef _Vec3<float> Vec3;
typedef _Vec3<float> Vec3f;
typedef _Vec3<double> Vec3d;
typedef _Vec3<int> Vec3i;
}

#endif /* GEOMETRY_VEC3_H */

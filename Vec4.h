/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_VEC4_H
#define GEOMETRY_VEC4_H

#include "Interpolation.h"
#include "Vec3.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace Geometry {

/*!
 * Four-dimensional vector.
 *
 *  [_Vec4]
 */
template <typename T_>
class _Vec4 {
public:
	typedef T_ value_t;
	typedef _Vec3<value_t> vec3_t;

	/*!
	 * comparator class for comparing vectors when using as key in a map or set
	 * comparison is done by values only not by pointers so different vectors with same values are equal
	 */
	class Comparator {
	public:
		bool operator()(const _Vec4 & a, const _Vec4 & b) const {
			if (a.getX() != b.getX())
				return a.getX() < b.getX();
			if (a.getY() != b.getY())
				return a.getY() < b.getY();
			if (a.getZ() != b.getZ())
				return a.getZ() < b.getZ();
			return a.getW() < b.getW();
		}

		bool operator()(const _Vec4 * a, const _Vec4 * b) const {
			return (*this)(*a, *b);
		}
	};

	/*!
	 * @name Main
	 */
	//@{
	/*! [ctor]
		@note all components are initialized with 0	*/
	_Vec4() {
		setValue(static_cast<value_t>(0));
	}

	/*!	[ctor]
		@param x used to initialize the first component
		@param y used to initialize the second component
		@param z used to initialize the third component	*/
	_Vec4(value_t _x, value_t _y, value_t _z, value_t _w) {
		setValue(_x, _y, _z, _w);
	}

	/*!	[ctor]
		@param v used to initialize the new Vec4
		@note values are converted by casting	*/
	template <class other_t>
	explicit _Vec4(const _Vec4<other_t> & v) {
		setValue(static_cast<value_t>(v.x()), static_cast<value_t>(v.y()), static_cast<value_t>(v.z()),
				 static_cast<value_t>(v.w()));
	}

	/*!	[ctor]
		interpolation between v1 and sv2, according to the factor blend
		@param v1, v2 vectors to be interpolated
		@param blend blendingfactor (should be between 0 and 1)	*/
	_Vec4(const _Vec4 & v1, const _Vec4 & v2, value_t blend) {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		*this = Interpolation::linear(v1, v2, blend);
	}

	/*!	[ctor]
		@param v the first four entries of v are used for for initialisation	*/
	explicit _Vec4(const value_t * v) {
		setValue(v[0], v[1], v[2], v[3]);
	}

	/*!	[ctor]
		@param v the first three entries of v are used for for initialisation	*/
	_Vec4(const vec3_t & v3, value_t _w) {
		setValue(v3.x(), v3.y(), v3.z(), _w);
	}
	//@}

	/*!
	 * @name Information
	 */
	//@{
	/*! array-access operator */
	value_t & operator[](uint_fast8_t nr) {
		return vec[nr];
	}

	/*! const array-access operator	*/
	const value_t & operator[](uint_fast8_t nr) const {
		return vec[nr];
	}

	/*! @return the underlying array of this	*/
	const value_t * getVec() const {
		return vec;
	}

	/*! @return the i-th component of this	*/
	const value_t get(const int & i) const {
		return vec[i];
	}

	/*! @return the x component of this
		@note equal to this[0]	*/
	const value_t getX() const {
		return vec[0];
	}
	const value_t x() const {
		return vec[0];
	}

	/*! @return the y component of this
		@note equal to this[1]	*/
	const value_t getY() const {
		return vec[1];
	}
	const value_t y() const {
		return vec[1];
	}

	/*! @return the z component of this
		@note equal to this[2]	*/
	const value_t getZ() const {
		return vec[2];
	}
	const value_t z() const {
		return vec[2];
	}

	/*! @return the z component of this
		@note equal to this[3]	*/
	const value_t getW() const {
		return vec[3];
	}
	const value_t w() const {
		return vec[3];
	}

	/*! @return Vec3(x,y,z) */
	vec3_t xyz() const {
		return vec3_t(x(), y(), z());
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

	/*! calculates the dotproduct of this and another Vec4
		@param p the vector to be used for calculation
		@return the dotproduct of this and p	*/
	inline value_t dot(const _Vec4 & p) const {
		return (vec[0] * p.vec[0] + vec[1] * p.vec[1] + vec[2] * p.vec[2] + vec[3] * p.vec[3]);
	}

	/*! calculates the distance between this and another Vec4
	 * @param p the vector to be used for calculation
	 * @return the distance between this and p	*/
	template <typename float_t>
	float_t distance(const _Vec4 & p) const {
		return (*this - p).template length<float_t>();
	}
	value_t distance(const _Vec4 & p) const {
		return distance<value_t>(p);
	}

	/*! @return vec == (0,0,0,0)	*/
	bool isZero() const {
		return vec[0] == 0 && vec[1] == 0 && vec[2] == 0 && vec[3] == 0;
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
	void setValue(const value_t _x, const value_t _y, const value_t _z, const value_t _w) {
		vec[0] = _x;
		vec[1] = _y;
		vec[2] = _z;
		vec[3] = _w;
	}

	/*! sets all components of this
	 * @param t used to set all components	*/
	void setValue(const value_t t) {
		setValue(t, t, t, t);
	}

	/*! sets the components of this
		@param v the first three values of v are used to set the components of this	*/
	void setValue(const value_t * v) {
		setValue(v[0], v[1], v[2], v[3]);
	}

	/*! sets the x component
		@param x the value the x component of this is set to
		@note equal to this[0]=x	*/
	void setX(const value_t _x) {
		vec[0] = _x;
	}
	void x(const value_t v) {
		vec[0] = v;
	}

	/*! sets the y component
		@param y the value the y component of this is set to
		@note equal to this[1]=y	*/
	void setY(const value_t _y) {
		vec[1] = _y;
	}
	void y(const value_t v) {
		vec[1] = v;
	}

	/*! sets the z component
		@param z the value the z component of this is set to
		@note equal to this[2]=z	*/
	void setZ(const value_t _z) {
		vec[2] = _z;
	}
	void z(const value_t v) {
		vec[2] = v;
	}

	/*! sets the z component
		@param z the value the z component of this is set to
		@note equal to this[2]=z	*/
	void setW(const value_t _w) {
		vec[3] = _w;
	}
	void w(const value_t v) {
		vec[3] = v;
	}

	/*! adds another Vec4 to this
		@param the Vec4 to be added
		@return this
		@note this will be modified	*/
	_Vec4 & operator+=(const _Vec4 & p) {
		vec[0] += p.vec[0];
		vec[1] += p.vec[1];
		vec[2] += p.vec[2];
		vec[3] += p.vec[3];
		return *this;
	}

	/*! subtracts another Vec4 from this
		@param the Vec4 to be substracted
		@return this
		@note this will be modified	*/
	_Vec4 & operator-=(const _Vec4 & p1) {
		vec[0] -= p1.vec[0];
		vec[1] -= p1.vec[1];
		vec[2] -= p1.vec[2];
		vec[3] -= p1.vec[3];
		return *this;
	}

	/*! multiplies this componentwise with a scalar value
		@param f the scalar
		@return this
		@note this will be modified	*/
	_Vec4 & operator*=(const value_t & f) {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		vec[0] *= f;
		vec[1] *= f;
		vec[2] *= f;
		vec[3] *= f;
		return *this;
	}

	/*! divides this componentwise by a scalar value
		@param f the scalar
		@return a new Vec4
		@note this will be modified	*/
	_Vec4 & operator/=(const value_t & f) {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		vec[0] /= f;
		vec[1] /= f;
		vec[2] /= f;
		vec[3] /= f;
		return *this;
	}

	/*! normalizes this
		@return this
		@note this will be modified	*/
	_Vec4 & normalize() {
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
		vec[3] *= l;
		return *this;
	}
	//@}

	/*!
	 * @name Creation
	 */
	//@{
	/*! adds another Vec4 to this
		@param the Vec4 to be added
		@return a new Vec4
		@note this remains unchanged	*/
	const _Vec4 operator+(const _Vec4 & p) const {
		return _Vec4(vec[0] + p.vec[0], vec[1] + p.vec[1], vec[2] + p.vec[2], vec[3] + p.vec[3]);
	}

	/*! subtracts another Vec4 from this
		@param the Vec4 to be substracted
		@return a new Vec4
		@note this remains unchanged	*/
	const _Vec4 operator-(const _Vec4 & p) const {
		return _Vec4(vec[0] - p.vec[0], vec[1] - p.vec[1], vec[2] - p.vec[2], vec[3] - p.vec[3]);
	}

	/*! @return a new Vec4 representing the negation of this
		@note this remains unchanged	*/
	const _Vec4 operator-() const {
		return _Vec4(-vec[0], -vec[1], -vec[2], -vec[3]);
	}

	/*! multiplies this componentwise with a scalar value
		@param f the scalar
		@return a new Vec4
		@note this remains unchanged	*/
	const _Vec4 operator*(const value_t & f) const {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		return _Vec4(vec[0] * f, vec[1] * f, vec[2] * f, vec[3] * f);
	}

	/*! divides this componentwise by a scalar value
		@param f the scalar
		@return a new Vec4
		@note this remains unchanged	*/
	const _Vec4 operator/(const value_t & f) const {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		return _Vec4(vec[0] / f, vec[1] / f, vec[2] / f, vec[3] / f);
	}

	/*! returns a normalized copy of this.	*/
	inline _Vec4 getNormalized() const {
		_Vec4 n = *this;
		return n.normalize();
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
		return std::abs(other.vec[0] - vec[0]) <= epsilon && std::abs(other.vec[1] - vec[1]) <= epsilon
				&& std::abs(other.vec[2] - vec[2]) <= epsilon && std::abs(other.vec[3] - vec[3]) <= epsilon;
	}
	/*! compares this with another Vec4
		@param p the Vec4 this shall be compared to
		@return true iff all components of this are equal to the corresponding ones in p	*/
	inline bool operator==(const _Vec4 & p) const {
		return ((p.vec[0] == vec[0] && p.vec[1] == vec[1] && p.vec[2] == vec[2] && p.vec[3] == vec[3]));
	}

	/*! compares this with another Vec4
		@param p the Vec4 this shall be compared to
		@return true iff at least one component of this is not equal to the corresponding one in p	*/
	inline bool operator!=(const _Vec4 & p) const {
		return ((p.vec[0] != vec[0] || p.vec[1] != vec[1] || p.vec[2] != vec[2] || p.vec[3] != vec[3]));
	}

	/**
	 * do not implement <, >, <=, >= methods because these are ambiguous
	 * either if you want to use vec4 as key in a map or to compare two vectors geometrically
	 */
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
		t[3] = vec[3];
	}
	//@}

	// ---- Data
private:
	value_t vec[4];

	//! @name Serialization
	//@{
public:
	friend std::ostream & operator<<(std::ostream & out, const _Vec4 & v) {
		return out << v.vec[0] << ' ' << v.vec[1] << ' ' << v.vec[2] << ' ' << v.vec[3];
	}
	friend std::istream & operator>>(std::istream & in, _Vec4 & v) {
		return in >> v.vec[0] >> v.vec[1] >> v.vec[2] >> v.vec[3];
	}
	//@}
};
typedef _Vec4<float> Vec4;
typedef _Vec4<float> Vec4f;
typedef _Vec4<double> Vec4d;
typedef _Vec4<int> Vec4i;
}

#endif /* GEOMETRY_VEC4_H */

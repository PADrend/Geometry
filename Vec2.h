/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_VEC2_H
#define GEOMETRY_VEC2_H

#include <cmath>
#include <istream>
#include <ostream>
#include <type_traits>

namespace Geometry {
/**
 * Two-dimensional vector.
 *
 *  [_Vec2]
 */
template <typename T_>
class _Vec2 {
public:
	typedef T_ value_t;
	typedef _Vec2<value_t> vec2_t;

	/*!
	 * comparator class for comparing vectors when using as key in a map or set
	 * comparison is done by values only not by pointers so different vectors with same values are equal
	 */
	class Comparator {
	public:
		bool operator()(const vec2_t & a, const vec2_t & b) const {
			if (a.getX() != b.getX())
				return a.getX() < b.getX();
			return a.getY() < b.getY();
		}

		bool operator()(const vec2_t * a, const vec2_t * b) const {
			return (*this)(*a, *b);
		}
	};

	/**
	 * @name Main
	 */
	//@{
public:
	_Vec2() {
		x(static_cast<value_t>(0));
		y(static_cast<value_t>(0));
		// ctor
	}
	_Vec2(value_t _x, value_t _y) {
		x(_x);
		y(_y);
		// ctor
	}
	template <class other_t>
	_Vec2(const _Vec2<other_t> & v) {
		x(static_cast<value_t>(v.x()));
		y(static_cast<value_t>(v.y()));
		// ctor
	}
	_Vec2(const value_t * v) {
		x(v[0]);
		y(v[1]);
		// ctor
	}
	//@}

	/**
	 * @name Information
	 */
	//@{
	inline const value_t x() const {
		return vec[0];
	}
	inline const value_t y() const {
		return vec[1];
	}
	inline const value_t getX() const {
		return x();
	}
	inline const value_t getY() const {
		return y();
	}
	inline const value_t getWidth() const {
		return x();
	}
	inline const value_t getHeight() const {
		return y();
	}
	inline const value_t width() const {
		return x();
	}
	inline const value_t height() const {
		return y();
	}

	template <typename float_t>
	float_t length() const {
		static_assert(std::is_floating_point<float_t>::value, "template argument not a floating point type");
		return std::sqrt(static_cast<float_t>(dot(*this)));
	}
	value_t length() const {
		return length<value_t>();
	}

	inline value_t & operator[](const unsigned int nr) {
		// assert nr<2;
		return vec[nr];
	}

	inline value_t * getVec() {
		return vec;
	}
	inline const value_t * getVec() const {
		return vec;
	}

	/**
	 * Calculate the Euclidean distance between this and another vector.
	 *
	 * @param other Other vector
	 * @return Distance
	 */
	template <typename float_t>
	inline float_t distance(const _Vec2 & other) const {
		static_assert(std::is_floating_point<float_t>::value, "template argument not a floating point type");
		return std::sqrt(static_cast<float_t>(distanceSquared(other)));
	}
	value_t distance(const _Vec2 & other) const {
		return distance<value_t>(other);
	}

	/**
	 * Calculate the squared Euclidean distance between this and another vector.
	 *
	 * @param other Other vector
	 * @return Squared distance
	 */
	value_t distanceSquared(const _Vec2 & other) const {
		const value_t dx = vec[0] - other.vec[0];
		const value_t dy = vec[1] - other.vec[1];
		return dx * dx + dy * dy;
	}

	inline value_t dot(const vec2_t & p) const {
		return x() * p.x() + y() * p.y();
	}
	bool isZero() const {
		return x() == 0 && y() == 0;
	}
	//@}

	/**
	 * @name Modification
	 */
	//@{
	inline void setValue(const value_t a, const value_t b) {
		x(a);
		y(b);
	}

	inline void setValue(const value_t * v) {
		x(v[0]);
		y(v[1]);
	}

	inline void x(const value_t a) {
		vec[0] = a;
	}
	inline void y(const value_t a) {
		vec[1] = a;
	}
	inline void setX(const value_t a) {
		x(a);
	}
	inline void setY(const value_t a) {
		y(a);
	}
	inline void setWidth(const value_t a) {
		x(a);
	}
	inline void setHeight(const value_t a) {
		y(a);
	}
	inline void width(const value_t a) {
		x(a);
	}
	inline void height(const value_t a) {
		y(a);
	}

	inline vec2_t & normalize() {
		value_t norme = x() * x() + y() * y();
		if (norme == 0.0)
			return *this;
		if (norme == 1.0)
			return *this;
		norme = sqrt(norme);
		vec[0] /= norme;
		vec[1] /= norme;
		return *this;
	}
	inline vec2_t & operator+=(const vec2_t & p1) {
		vec[0] += p1.x();
		vec[1] += p1.y();
		return *this;
	}
	inline vec2_t & operator-=(const vec2_t & p1) {
		vec[0] -= p1.x();
		vec[1] -= p1.y();
		return *this;
	}
	vec2_t & operator*=(const value_t & f) {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		vec[0] *= f;
		vec[1] *= f;
		return *this;
	}
	vec2_t & operator/=(const value_t & f) {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		vec[0] /= f;
		vec[1] /= f;
		return *this;
	}
	//@}

	/**
	 * @name Creation
	 */
	//@{
	inline const vec2_t operator-() const {
		return vec2_t(-x(), -y());
	}
	inline const vec2_t operator+(const vec2_t & p) const {
		return vec2_t(x() + p.x(), y() + p.y());
	}
	inline const vec2_t operator-(const vec2_t & p) const {
		return vec2_t(x() - p.x(), y() - p.y());
	}
	const vec2_t operator*(const value_t & f) const {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		return vec2_t(x() * f, y() * f);
	}
	const vec2_t operator/(const value_t & f) const {
		static_assert(std::is_floating_point<value_t>::value, "template argument not a floating point type");
		return vec2_t(x() / f, y() / f);
	}
	//@}

	/**
	 * @name Comparators
	 */
	//@{
	/*! compares this with other componentwise
	@param other the object to compare with
	@param epsilon the maximum allowed difference between the component pairs
	@return true iff any the absolute difference between any pai of components is larger than epsilon */
	inline bool equals(const vec2_t & other, value_t epsilon) const {
		return std::abs(x() - other.x()) <= epsilon && std::abs(y() - other.y()) <= epsilon;
	}
	inline int operator==(const vec2_t & p1) const {
		return (p1.x() == x() && p1.y() == y());
	}
	inline int operator!=(const vec2_t & p1) const {
		return (p1.x() != x() || p1.y() != y());
	}
	/**
	 * do not implement <, >, <=, >= methods because these are ambiguous
	 * either if you want to use vec2 as key in a map or to compare two vectors geometrically
	 */
	//@}

	// -----------------------------------
	// ---- Data
private:
	value_t vec[2];

	//! @name Serialization
	//@{
public:
	friend std::ostream & operator<<(std::ostream & out, const vec2_t & v) {
		return out << v.vec[0] << ' ' << v.vec[1];
	}
	friend std::istream & operator>>(std::istream & in, vec2_t & v) {
		return in >> v.vec[0] >> v.vec[1];
	}
	//@}
};

typedef _Vec2<float> Vec2;
typedef _Vec2<float> Vec2f;
typedef _Vec2<int> Vec2i;
typedef _Vec2<double> Vec2d;
}

#endif /* GEOMETRY_VEC2_H */

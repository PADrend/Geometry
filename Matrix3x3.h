/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_MATRIX3X3_H
#define GEOMETRY_MATRIX3X3_H

#include "Angle.h"
#include "Interpolation.h"
#include "Vec3.h"
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <limits>

namespace Geometry {

/**
 * 3*3 matrix.
 * designed to be used in SRTs where the 3x3 matrix is always othonormal.
 * the matrix gets re-orthonormalized aftersome number of multiplications.
 * so the matrix can not be used for other purposes.
 *
 * [_Matrix3x3]
 */
template <typename T_>
class _Matrix3x3 {
public:
	typedef T_ value_t;
	typedef _Angle<value_t> angle_t;
	typedef _Vec3<value_t> vec3_t;

	// -----------------------------------
	// ---- Data
private:
	/*	index = row * 3 + column
		0	1	2
		3	4	5
		6	7	8	*/
	value_t data[9];
	static const uint_fast8_t R0C0 = 0, R0C1 = 1, R0C2 = 2;
	static const uint_fast8_t R1C0 = 3, R1C1 = 4, R1C2 = 5;
	static const uint_fast8_t R2C0 = 6, R2C1 = 7, R2C2 = 8;

	/**
	 * @name Main
	 */
	//@{
public:
	const static int FRONT = 2; // column index
	const static int RIGHT = 0; // column index
	const static int UP = 1; // column index

	_Matrix3x3() {
		setIdentity();
	}

	/**
	 * [ctor] interpolation between m1 and m2, according to the factor blend (should be between 0 and 1);
	 */
	_Matrix3x3(const _Matrix3x3<value_t> & m1, const _Matrix3x3<value_t> & m2, value_t blend) {
		for (uint_fast8_t i = 0; i < 9; ++i) {
			set(i, Interpolation::linear(m1.at(i), m2.at(i), blend));
		}
		normOrthoLize();
	}
	explicit _Matrix3x3(value_t r0c0, value_t r0c1, value_t r0c2, value_t r1c0, value_t r1c1, value_t r1c2,
						value_t r2c0, value_t r2c1, value_t r2c2) {
		set(r0c0, r0c1, r0c2, r1c0, r1c1, r1c2, r2c0, r2c1, r2c2);
	}

	explicit _Matrix3x3(const value_t * m) {
		std::copy(m, m + 9, data);
	}
	//@}

	/**
	 * @name Information
	 */
	//@{
	inline value_t at(uint_fast8_t row, uint_fast8_t column) const {
		return data[row * 3 + column];
	}
	inline value_t at(uint_fast8_t index) const {
		return data[index];
	}
	inline vec3_t getCol(uint_fast8_t col) const {
		return vec3_t(at(col), at(col + 3), at(col + 6));
	}
	inline vec3_t getRow(uint_fast8_t row) const {
		return vec3_t(data + row * 3);
	}

	value_t det() const {
		return +at(R0C0) * (at(R1C1) * at(R2C2) - at(R2C1) * at(R1C2))
				- at(R0C1) * (at(R1C0) * at(R2C2) - at(R2C0) * at(R1C2))
				+ at(R0C2) * (at(R1C0) * at(R2C1) - at(R2C0) * at(R1C1));
	}
	inline bool isIdentity() const {
		return at(R0C0) == 1 && at(R0C1) == 0 && at(R0C2) == 0 && at(R1C0) == 0 && at(R1C1) == 1 && at(R1C2) == 0
				&& at(R2C0) == 0 && at(R2C1) == 0 && at(R2C2) == 1;
	}
	void getRotation(vec3_t & axis, angle_t & angle) const {
		axis.setX(at(R2C1) - at(R1C2));
		axis.setY(at(R0C2) - at(R2C0));
		axis.setZ(at(R1C0) - at(R0C1));
		value_t length = axis.length();
		value_t diagonal = at(R0C0) + at(R1C1) + at(R2C2);
		const float rad = std::atan2(length, diagonal - 1.0f);
		if (rad == 0.0f) {
			axis.setValue(0.0f, 0.0f, 0.0f);
		} else {
			axis.normalize();
		}
		angle = angle_t::rad(rad);
	}
	void getRotation_rad(vec3_t & axis, value_t & rad) const {
		angle_t angle;
		getRotation(axis, angle);
		rad = angle.rad();
	}
	void getRotation_deg(vec3_t & axis, value_t & deg) const {
		angle_t angle;
		getRotation(axis, angle);
		deg = angle.deg();
	}
	//@}

	/**
	 * @name Modification
	 */
	//@{
	inline void set(uint_fast8_t index, value_t value) {
		data[index] = value;
	}
	inline void set(uint_fast8_t row, uint_fast8_t column, value_t value) {
		set(row * 3 + column, value);
	}
	void set(value_t r0c0, value_t r0c1, value_t r0c2, value_t r1c0, value_t r1c1, value_t r1c2, value_t r2c0,
			 value_t r2c1, value_t r2c2) {
		set(R0C0, r0c0);
		set(R0C1, r0c1);
		set(R0C2, r0c2);
		set(R1C0, r1c0);
		set(R1C1, r1c1);
		set(R1C2, r1c2);
		set(R2C0, r2c0);
		set(R2C1, r2c1);
		set(R2C2, r2c2);
	}
	inline void setIdentity() {
		set(1, 0, 0, 0, 1, 0, 0, 0, 1);
	}

	inline void setRow(uint_fast8_t row, value_t c0, value_t c1, value_t c2) {
		const uint_fast8_t i = row * 3;
		set(i, c0);
		set(i + 1, c1);
		set(i + 2, c2);
	}
	inline void setCol(uint_fast8_t col, value_t r0, value_t r1, value_t r2) {
		set(col, r0);
		set(col + 3, r1);
		set(col + 6, r2);
	}
	inline void setRow(uint_fast8_t row, const vec3_t & r) {
		setRow(row, r.x(), r.y(), r.z());
	}
	inline void setCol(uint_fast8_t col, const vec3_t & c) {
		setCol(col, c.x(), c.y(), c.z());
	}

	void normOrthoLize() {
		const vec3_t r = getCol(RIGHT).normalize();
		vec3_t u = getCol(UP).normalize();
		const vec3_t f = r.cross(u).normalize();
		u = f.cross(r).normalize();
		setCol(FRONT, f);
		setCol(UP, u);
		setCol(RIGHT, r);
	}

	//! Return a rotation matrix that rotates by an @p angle about an axis in the direction of the unit vector @p axis.
	static _Matrix3x3 createRotation(const angle_t & angle, const vec3_t & axis) {
		const value_t c = std::cos(angle.rad());
		const value_t s = std::sin(angle.rad());
		const value_t t = 1 - c;
		const value_t xx = axis.getX() * axis.getX();
		const value_t xy = axis.getX() * axis.getY();
		const value_t xz = axis.getX() * axis.getZ();
		const value_t yy = axis.getY() * axis.getY();
		const value_t yz = axis.getY() * axis.getZ();
		const value_t zz = axis.getZ() * axis.getZ();
		return _Matrix3x3(t * xx + c, t * xy - s * axis.getZ(), t * xz + s * axis.getY(), t * xy + s * axis.getZ(),
						  t * yy + c, t * yz - s * axis.getX(), t * xz - s * axis.getY(), t * yz + s * axis.getX(),
						  t * zz + c);
	}

	_Matrix3x3 & setRotation(const vec3_t & dir, const vec3_t & up) {
		vec3_t baseZ = vec3_t(dir).normalize();
		vec3_t baseX = vec3_t(up).normalize().cross(baseZ).normalize();
		vec3_t baseY = baseZ.cross(baseX);
		set(baseX.getX(), baseY.getX(), baseZ.getX(), baseX.getY(), baseY.getY(), baseZ.getY(), baseX.getZ(),
			baseY.getZ(), baseZ.getZ());
		return *this;
	}
	//@}

	/**
	 * @name Creation
	 */
	//@{
	_Matrix3x3 getTransposed() const {
		return _Matrix3x3(at(R0C0), at(R1C0), at(R2C0), at(R0C1), at(R1C1), at(R2C1), at(R0C2), at(R1C2), at(R2C2));
	}
	_Matrix3x3 getInverse() const {
		const value_t d = det();
		if (std::abs(d) < std::numeric_limits<value_t>::epsilon()) {
			std::cerr << "Matrix not invertible, det = " << d << std::endl;
			return *this;
		}
		const value_t dInv = 1 / d;
		return _Matrix3x3(
				(at(R1C1) * at(R2C2) - at(R1C2) * at(R2C1)) * dInv, -(at(R0C1) * at(R2C2) - at(R2C1) * at(R0C2)) * dInv,
				(at(R0C1) * at(R1C2) - at(R1C1) * at(R0C2)) * dInv,

				-(at(R1C0) * at(R2C2) - at(R1C2) * at(R2C0)) * dInv, (at(R0C0) * at(R2C2) - at(R2C0) * at(R0C2)) * dInv,
				-(at(R0C0) * at(R1C2) - at(R1C0) * at(R0C2)) * dInv,

				(at(R1C0) * at(R2C1) - at(R2C0) * at(R1C1)) * dInv, -(at(R0C0) * at(R2C1) - at(R2C0) * at(R0C1)) * dInv,
				(at(R0C0) * at(R1C1) - at(R0C1) * at(R1C0)) * dInv);
	}
	const _Matrix3x3 operator+(const _Matrix3x3 & b) const {
		return _Matrix3x3(at(R0C0) + b.at(R0C0), at(R0C1) + b.at(R0C1), at(R0C2) + b.at(R0C2), at(R1C0) + b.at(R1C0),
						  at(R1C1) + b.at(R1C1), at(R1C2) + b.at(R1C2), at(R2C0) + b.at(R2C0), at(R2C1) + b.at(R2C1),
						  at(R2C2) + b.at(R2C2));
	}
	const _Matrix3x3 operator-(const _Matrix3x3 & b) const {
		return _Matrix3x3(at(R0C0) - b.at(R0C0), at(R0C1) - b.at(R0C1), at(R0C2) - b.at(R0C2), at(R1C0) - b.at(R1C0),
						  at(R1C1) - b.at(R1C1), at(R1C2) - b.at(R1C2), at(R2C0) - b.at(R2C0), at(R2C1) - b.at(R2C1),
						  at(R2C2) - b.at(R2C2));
	}
	const _Matrix3x3 operator*(const _Matrix3x3 & b) const {
		return _Matrix3x3(at(R0C0) * b.at(R0C0) + at(R0C1) * b.at(R1C0) + at(R0C2) * b.at(R2C0),
						  at(R0C0) * b.at(R0C1) + at(R0C1) * b.at(R1C1) + at(R0C2) * b.at(R2C1),
						  at(R0C0) * b.at(R0C2) + at(R0C1) * b.at(R1C2) + at(R0C2) * b.at(R2C2),

						  at(R1C0) * b.at(R0C0) + at(R1C1) * b.at(R1C0) + at(R1C2) * b.at(R2C0),
						  at(R1C0) * b.at(R0C1) + at(R1C1) * b.at(R1C1) + at(R1C2) * b.at(R2C1),
						  at(R1C0) * b.at(R0C2) + at(R1C1) * b.at(R1C2) + at(R1C2) * b.at(R2C2),

						  at(R2C0) * b.at(R0C0) + at(R2C1) * b.at(R1C0) + at(R2C2) * b.at(R2C0),
						  at(R2C0) * b.at(R0C1) + at(R2C1) * b.at(R1C1) + at(R2C2) * b.at(R2C1),
						  at(R2C0) * b.at(R0C2) + at(R2C1) * b.at(R1C2) + at(R2C2) * b.at(R2C2));
	}
	const vec3_t operator*(const vec3_t & v) const {
		return vec3_t(at(R0C0) * v.x() + at(R0C1) * v.y() + at(R0C2) * v.z(),
					  at(R1C0) * v.x() + at(R1C1) * v.y() + at(R1C2) * v.z(),
					  at(R2C0) * v.x() + at(R2C1) * v.y() + at(R2C2) * v.z());
	}
	const _Matrix3x3 operator*(value_t f) const {
		return _Matrix3x3(at(R0C0) * f, at(R0C1) * f, at(R0C2) * f, at(R1C0) * f, at(R1C1) * f, at(R1C2) * f,
						  at(R2C0) * f, at(R2C1) * f, at(R2C2) * f);
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
	inline bool equals(const _Matrix3x3 & other, value_t epsilon) const {
		return std::abs(at(R0C0) - other.at(R0C0)) <= epsilon && std::abs(at(R0C1) - other.at(R0C1)) <= epsilon
				&& std::abs(at(R0C2) - other.at(R0C2)) <= epsilon && std::abs(at(R1C0) - other.at(R1C0)) <= epsilon
				&& std::abs(at(R1C1) - other.at(R1C1)) <= epsilon && std::abs(at(R1C2) - other.at(R1C2)) <= epsilon
				&& std::abs(at(R2C0) - other.at(R2C0)) <= epsilon && std::abs(at(R2C1) - other.at(R2C1)) <= epsilon
				&& std::abs(at(R2C2) - other.at(R2C2)) <= epsilon;
	}
	inline bool operator==(const _Matrix3x3 & m) const {
		return at(R0C0) == m.at(R0C0) && at(R0C1) == m.at(R0C1) && at(R0C2) == m.at(R0C2) && at(R1C0) == m.at(R1C0)
				&& at(R1C1) == m.at(R1C1) && at(R1C2) == m.at(R1C2) && at(R2C0) == m.at(R2C0) && at(R2C1) == m.at(R2C1)
				&& at(R2C2) == m.at(R2C2);
	}
	inline bool operator!=(const _Matrix3x3 & m) const {
		return at(R0C0) != m.at(R0C0) || at(R0C1) != m.at(R0C1) || at(R0C2) != m.at(R0C2) || at(R1C0) != m.at(R1C0)
				|| at(R1C1) != m.at(R1C1) || at(R1C2) != m.at(R1C2) || at(R2C0) != m.at(R2C0) || at(R2C1) != m.at(R2C1)
				|| at(R2C2) != m.at(R2C2);
	}
	//@}

	//! @name Serialization
	//@{
	friend std::ostream & operator<<(std::ostream & out, const _Matrix3x3 & matrix) {
		return out << matrix.at(R0C0) << ' ' << matrix.at(R0C1) << ' ' << matrix.at(R0C2) << ' ' << matrix.at(R1C0)
				   << ' ' << matrix.at(R1C1) << ' ' << matrix.at(R1C2) << ' ' << matrix.at(R2C0) << ' '
				   << matrix.at(R2C1) << ' ' << matrix.at(R2C2);
	}
	friend std::istream & operator>>(std::istream & in, _Matrix3x3 & matrix) {
		return in >> matrix.at(R0C0) >> matrix.at(R0C1) >> matrix.at(R0C2) >> matrix.at(R1C0) >> matrix.at(R1C1)
				>> matrix.at(R1C2) >> matrix.at(R2C0) >> matrix.at(R2C1) >> matrix.at(R2C2);
	}
	//@}
};
typedef _Matrix3x3<float> Matrix3f;
typedef _Matrix3x3<float> Matrix3x3;
typedef _Matrix3x3<float> Matrix3x3f;
typedef _Matrix3x3<double> Matrix3x3d;
}

#endif /* GEOMETRY_MATRIX3X3_H */

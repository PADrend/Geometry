/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_MATRIX4X4_H
#define GEOMETRY_MATRIX4X4_H

#include "Angle.h"
#include "SRT.h"
#include "Matrix3x3.h"
#include "Vec3.h"
#include "Vec4.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace Geometry {

/**
 * 4*4 matrix.
 *
 *  [_Matrix4x4]
 */
template <typename T_>
class _Matrix4x4 {
public:
	typedef T_ value_t;
	typedef _Angle<value_t> angle_t;
	typedef _SRT<value_t> srt_t;
	typedef _Vec3<value_t> vec3_t;
	typedef _Vec4<value_t> vec4_t;

	// -----------------------------------
	// ---- Data
private:
	value_t m[16];

	/**
	 * @name Construction
	 */
	//@{
public:
	_Matrix4x4() {
		setIdentity();
	}

	_Matrix4x4(value_t d0, value_t d1, value_t d2, value_t d3, value_t d4, value_t d5, value_t d6, value_t d7,
			   value_t d8, value_t d9, value_t d10, value_t d11, value_t d12, value_t d13, value_t d14, value_t d15) {
		m[0] = d0;
		m[1] = d1;
		m[2] = d2;
		m[3] = d3;
		m[4] = d4;
		m[5] = d5;
		m[6] = d6;
		m[7] = d7;
		m[8] = d8;
		m[9] = d9;
		m[10] = d10;
		m[11] = d11;
		m[12] = d12;
		m[13] = d13;
		m[14] = d14;
		m[15] = d15;
	}

	explicit _Matrix4x4(const value_t * data) {
		std::copy(data, data + 16, m);
	}

	explicit _Matrix4x4(const srt_t & srt) {
		srt.toArray(m);
	}

	//! Convert a 3 times 3 matrix to a 4 times 4 matrix by adding a one in the last entry, and zeros in the fourth
	//column and fourth row.
	explicit _Matrix4x4(const _Matrix3x3<value_t> & mat) {
		m[0] = mat.at(0, 0);
		m[1] = mat.at(0, 1);
		m[2] = mat.at(0, 2);
		m[3] = 0;
		m[4] = mat.at(1, 0);
		m[5] = mat.at(1, 1);
		m[6] = mat.at(1, 2);
		m[7] = 0;
		m[8] = mat.at(2, 0);
		m[9] = mat.at(2, 1);
		m[10] = mat.at(2, 2);
		m[11] = 0;
		m[12] = 0;
		m[13] = 0;
		m[14] = 0;
		m[15] = 1;
	}

	/**
	 *	v0.x v1.x v2.x v3.x
	 *	v0.y v1.y v2.y v3.y
	 *	v0.z v1.z v2.z v3.z
	 *	v0.w v1.w v2.w v3.w
	 */
	_Matrix4x4(const vec4_t & v0, const vec4_t & v1, const vec4_t & v2, const vec4_t & v3) {
		m[0] = v0.x();
		m[1] = v1.x();
		m[2] = v2.x();
		m[3] = v3.x();
		m[4] = v0.y();
		m[5] = v1.y();
		m[6] = v2.y();
		m[7] = v3.y();
		m[8] = v0.z();
		m[9] = v1.z();
		m[10] = v2.z();
		m[11] = v3.z();
		m[12] = v0.w();
		m[13] = v1.w();
		m[14] = v2.w();
		m[15] = v3.w();
	}

	//@}

	/**
	 * @name Information
	 */
	//@{
	/**
	 *   @param i an index
	 *   @return the i-th element of this matrix
	 *   @note elements are arranged row-wise, so i=3 will result in the 1st column in the 2nd row
	 */
	value_t & operator[](const unsigned int i) {
		return m[i];
	}
	/**
	 *   @param i an index
			  *   @return the i-th element of this matrix
	 *   @note elements are arranged row-wise, so i=3 will result in the 1st column in the 2nd row
	 */
	value_t at(const unsigned int i) const {
		return m[i];
	}

	//! Return the first three values of column @a c as three-dimensional vector.
	vec3_t getColumnAsVec3(unsigned int c) const {
		if (c > 3) {
			throw std::out_of_range("Invalid column index");
		}
		return vec3_t(m[0 + c], m[4 + c], m[8 + c]);
	}

	//! Return the values of column @a c as four-dimensional vector.
	vec4_t getColumn(unsigned int c) const {
		if (c > 3) {
			throw std::out_of_range("Invalid column index");
		}
		return vec4_t(m[0 + c], m[4 + c], m[8 + c], m[12 + c]);
	}

	/**
	 *   @return the underlying array of this matrix
	 *   @note elements in array are arranged row-wise
	 */
	const value_t * getData() const {
		return m;
	}
	/**
	*   @return the determinant of this
	*/
	value_t det() const {
		return m[0] * m[5] * m[10] * m[15] - m[0] * m[5] * m[11] * m[14] - m[0] * m[9] * m[6] * m[15]
				+ m[0] * m[9] * m[7] * m[14] + m[0] * m[13] * m[6] * m[11] - m[0] * m[13] * m[7] * m[10]
				- m[4] * m[1] * m[10] * m[15] + m[4] * m[1] * m[11] * m[14] + m[4] * m[9] * m[2] * m[15]
				- m[4] * m[9] * m[3] * m[14] - m[4] * m[13] * m[2] * m[11] + m[4] * m[13] * m[3] * m[10]
				+ m[8] * m[1] * m[6] * m[15] - m[8] * m[1] * m[7] * m[14] - m[8] * m[5] * m[2] * m[15]
				+ m[8] * m[5] * m[3] * m[14] + m[8] * m[13] * m[2] * m[7] - m[8] * m[13] * m[3] * m[6]
				- m[12] * m[1] * m[6] * m[11] + m[12] * m[1] * m[7] * m[10] + m[12] * m[5] * m[2] * m[11]
				- m[12] * m[5] * m[3] * m[10] - m[12] * m[9] * m[2] * m[7] + m[12] * m[9] * m[3] * m[6];
	}
	bool isIdentity() const {
		return m[0] == 1 && m[1] == 0 && m[2] == 0 && m[3] == 0 && m[4] == 0 && m[5] == 1 && m[6] == 0 && m[7] == 0
				&& m[8] == 0 && m[9] == 0 && m[10] == 1 && m[11] == 0 && m[12] == 0 && m[13] == 0 && m[14] == 0
				&& m[15] == 1;
	}
	//@}

	/**
	 * @name Modification
	 */
	//@{
	void setIdentity() {
		m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] =
				static_cast<value_t>(0.0);
		m[0] = m[5] = m[10] = m[15] = static_cast<value_t>(1.0);
	}
	_Matrix4x4<value_t> & operator+=(const _Matrix4x4<value_t> & m2) {
		for (uint_fast8_t i = 0; i < 16; i++)
			m[i] += m2.m[i];
		return *this;
	}
	_Matrix4x4<value_t> & operator-=(const _Matrix4x4<value_t> & m2) {
		for (uint_fast8_t i = 0; i < 16; i++)
			m[i] -= m2.m[i];
		return *this;
	}
	_Matrix4x4<value_t> & operator*=(const _Matrix4x4<value_t> & m2) {
		{
			value_t old[4] = {m[0], m[1], m[2], m[3]};
			m[0] = old[0] * m2.m[0] + old[1] * m2.m[4] + old[2] * m2.m[8] + old[3] * m2.m[12];
			m[1] = old[0] * m2.m[1] + old[1] * m2.m[5] + old[2] * m2.m[9] + old[3] * m2.m[13];
			m[2] = old[0] * m2.m[2] + old[1] * m2.m[6] + old[2] * m2.m[10] + old[3] * m2.m[14];
			m[3] = old[0] * m2.m[3] + old[1] * m2.m[7] + old[2] * m2.m[11] + old[3] * m2.m[15];
		}
		{
			value_t old[4] = {m[4], m[5], m[6], m[7]};
			m[4] = old[0] * m2.m[0] + old[1] * m2.m[4] + old[2] * m2.m[8] + old[3] * m2.m[12];
			m[5] = old[0] * m2.m[1] + old[1] * m2.m[5] + old[2] * m2.m[9] + old[3] * m2.m[13];
			m[6] = old[0] * m2.m[2] + old[1] * m2.m[6] + old[2] * m2.m[10] + old[3] * m2.m[14];
			m[7] = old[0] * m2.m[3] + old[1] * m2.m[7] + old[2] * m2.m[11] + old[3] * m2.m[15];
		}
		{
			value_t old[4] = {m[8], m[9], m[10], m[11]};
			m[8] = old[0] * m2.m[0] + old[1] * m2.m[4] + old[2] * m2.m[8] + old[3] * m2.m[12];
			m[9] = old[0] * m2.m[1] + old[1] * m2.m[5] + old[2] * m2.m[9] + old[3] * m2.m[13];
			m[10] = old[0] * m2.m[2] + old[1] * m2.m[6] + old[2] * m2.m[10] + old[3] * m2.m[14];
			m[11] = old[0] * m2.m[3] + old[1] * m2.m[7] + old[2] * m2.m[11] + old[3] * m2.m[15];
		}
		{
			value_t old[4] = {m[12], m[13], m[14], m[15]};
			m[12] = old[0] * m2.m[0] + old[1] * m2.m[4] + old[2] * m2.m[8] + old[3] * m2.m[12];
			m[13] = old[0] * m2.m[1] + old[1] * m2.m[5] + old[2] * m2.m[9] + old[3] * m2.m[13];
			m[14] = old[0] * m2.m[2] + old[1] * m2.m[6] + old[2] * m2.m[10] + old[3] * m2.m[14];
			m[15] = old[0] * m2.m[3] + old[1] * m2.m[7] + old[2] * m2.m[11] + old[3] * m2.m[15];
		}
		return *this;
	}
	_Matrix4x4<value_t> & operator*=(value_t d) {
		for (auto & elem : m)
			elem *= d;
		return *this;
	}
	_Matrix4x4<value_t> & operator/=(value_t d) {
		for (auto & elem : m)
			elem /= d;
		return *this;
	}
	_Matrix4x4<value_t> & translate(value_t x, value_t y, value_t z) {
		_Matrix4x4<value_t> m2;
		m2.m[3] = x;
		m2.m[7] = y;
		m2.m[11] = z;
		return (*this) *= m2;
	}
	_Matrix4x4<value_t> & translate(const vec3_t & v) {
		return translate(v.getX(), v.getY(), v.getZ());
	}
	static _Matrix4x4 createScale(value_t sx, value_t sy, value_t sz) {
		return _Matrix4x4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
	}
	static _Matrix4x4 createScale(value_t s) {
		return createScale(s, s, s);
	}
	_Matrix4x4 & scale(value_t sx, value_t sy, value_t sz) {
		return (*this) *= createScale(sx, sy, sz);
	}
	_Matrix4x4 & scale(value_t s) {
		return scale(s, s, s);
	}

	/*! \see http://de.wikipedia.org/wiki/Rotationsmatrix
		\see http://wiki.delphigl.com/index.php/glRotate*/
	_Matrix4x4<value_t> & rotate(const angle_t & angle, value_t x, value_t y, value_t z) {
		const value_t d = angle.rad();
		const value_t s = std::sin(d);
		const value_t c = std::cos(d);
		const value_t lengthInv = 1.0 / sqrt(x * x + y * y + z * z);
		x *= lengthInv, y *= lengthInv, z *= lengthInv;

		const value_t xy = x * y, yz = y * z, xz = x * z, xs = x * s, ys = y * s, zs = z * s;
		_Matrix4x4<value_t> rot;
		rot.m[0] = x * x * (1 - c) + c;
		rot.m[1] = xy * (1 - c) - zs;
		rot.m[2] = xz * (1 - c) + ys;

		rot.m[4] = xy * (1 - c) + zs;
		rot.m[5] = y * y * (1 - c) + c;
		rot.m[6] = yz * (1 - c) - xs;

		rot.m[8] = xz * (1 - c) - ys;
		rot.m[9] = yz * (1 - c) + xs;
		rot.m[10] = z * z * (1 - c) + c;
		return (*this) *= rot;
	}
	inline _Matrix4x4<value_t> & rotate(const angle_t & angle, const vec3_t & axis) {
		return rotate(angle, axis.getX(), axis.getY(), axis.getZ());
	}
	inline _Matrix4x4<value_t> & rotate_deg(const value_t deg, const vec3_t & axis) {
		return rotate(angle_t::deg(deg), axis.getX(), axis.getY(), axis.getZ());
	}
	inline _Matrix4x4<value_t> & rotate_deg(const value_t deg, value_t x, value_t y, value_t z) {
		return rotate(angle_t::deg(deg), x, y, z);
	}
	inline _Matrix4x4<value_t> & rotate_rad(const value_t rad, const vec3_t & axis) {
		return rotate(angle_t::rad(rad), axis.getX(), axis.getY(), axis.getZ());
	}
	inline _Matrix4x4<value_t> & rotate_rad(const value_t rad, value_t x, value_t y, value_t z) {
		return rotate(angle_t::rad(rad), x, y, z);
	}

	/**
	* // www.gamedev.net/community/forums/topic.asp?topic_id=384661
	*/
	inline _Matrix4x4<value_t> & rotateToDirection(const vec3_t & _baseZ) {
		vec3_t baseZ = vec3_t(_baseZ);
		vec3_t baseX;
		vec3_t baseY;

		if (baseZ.getX() == 0 && baseZ.getY() == 0 && baseZ.getZ() == 0)
			return *this;
		baseZ.normalize();

		baseX = vec3_t(baseZ.getZ(), static_cast<value_t>(0.0), -baseZ.getX());
		baseX.normalize();
		//   baseY=vec3_t ((value_t)0,(value_t)1,(value_t)0);
		baseY = baseZ.cross(baseX); // vec3_t ((value_t)0,(value_t)1,(value_t)0);
		baseY.normalize();
		_Matrix4x4<value_t> rot;

		rot.m[0] = baseX.getX();
		rot.m[1] = baseX.getY();
		rot.m[2] = baseX.getZ();
		rot.m[3] = 0;
		rot.m[4] = baseY.getX();
		rot.m[5] = baseY.getY();
		rot.m[6] = baseY.getZ();
		rot.m[7] = 0;
		rot.m[8] = baseZ.getX();
		rot.m[9] = baseZ.getY();
		rot.m[10] = baseZ.getZ();
		rot.m[11] = 0;
		rot.m[12] = 0;
		rot.m[13] = 0;
		rot.m[14] = 0;
		rot.m[15] = 1;
		return (*this) *= rot;
	}
	/**
	 *  //http://pyopengl.sourceforge.net/documentation/manual/gluLookAt.3G.html
	 */
	inline _Matrix4x4<value_t> & lookAt(const vec3_t & _pos, const vec3_t & _target, const vec3_t & _up) {
		vec3_t baseZ = vec3_t(_target - _pos).normalize();
		vec3_t baseX = baseZ.cross(vec3_t(_up).normalize());
		vec3_t baseY = baseX.cross(baseZ);

		_Matrix4x4<value_t> rot;
		rot.m[0] = baseX.getX();
		rot.m[1] = baseX.getY();
		rot.m[2] = baseX.getZ();
		rot.m[3] = 0;
		rot.m[4] = baseY.getX();
		rot.m[5] = baseY.getY();
		rot.m[6] = baseY.getZ();
		rot.m[7] = 0;
		rot.m[8] = -baseZ.getX();
		rot.m[9] = -baseZ.getY();
		rot.m[10] = -baseZ.getZ();
		rot.m[11] = 0;
		rot.m[12] = 0;
		rot.m[13] = 0;
		rot.m[14] = 0;
		rot.m[15] = 1;
		(*this) *= rot;
		translate(-_pos);

		return *this;
	}
	/**
	 * http://pyopengl.sourceforge.net/documentation/manual/gluPerspective.3G.html
	 */
	static _Matrix4x4 perspectiveProjection(const angle_t & angle, value_t ratio, value_t zNear, value_t zFar) {
		const value_t s = std::tan(angle.rad() * 0.5) * zNear;
		return perspectiveProjection(-s * ratio, s * ratio, -s, s, zNear, zFar);
	}
	/**
	 * http://wiki.delphigl.com/index.php/glFrustum
	 *  glFrustum(left,right,bottom,top,near,far);
	 */
	static _Matrix4x4 perspectiveProjection(value_t l, value_t r, value_t b, value_t t, value_t n, value_t f) {
		const value_t twoN = static_cast<value_t>(2) * n;
		const value_t minusOne = static_cast<value_t>(-1);
		return _Matrix4x4(twoN / (r - l), 0, (r + l) / (r - l), 0, 0, twoN / (t - b), (t + b) / (t - b), 0, 0, 0,
						  -(f + n) / (f - n), -(twoN * f) / (f - n), 0, 0, minusOne, 0);
	}
	/**
	 * http://wiki.delphigl.com/index.php/glOrtho
	 *  glOrtho(left,right,bottom,top,near,far);
	 */
	static _Matrix4x4 orthographicProjection(value_t l, value_t r, value_t b, value_t t, value_t n, value_t f) {
		const value_t one = static_cast<value_t>(1);
		const value_t two = static_cast<value_t>(2);
		return _Matrix4x4(two / (r - l), 0, 0, -(r + l) / (r - l), 0, two / (t - b), 0, -(t + b) / (t - b), 0, 0,
						  -two / (f - n), -(f + n) / (f - n), 0, 0, 0, one);
	}
	/**
	 * Exchange rows and columns.
	 */
	_Matrix4x4<value_t> & transpose() {
		value_t t;
		t = m[1];
		m[1] = m[4];
		m[4] = t;
		t = m[2];
		m[2] = m[8];
		m[8] = t;
		t = m[3];
		m[3] = m[12];
		m[12] = t;
		t = m[6];
		m[6] = m[9];
		m[9] = t;
		t = m[7];
		m[7] = m[13];
		m[13] = t;
		t = m[11];
		m[11] = m[14];
		m[14] = t;
		return *this;
	}
	//@}

	/**
	 * @name Creation
	 */
	//@{
	const _Matrix4x4<value_t> operator+(const _Matrix4x4<value_t> & m2) const {
		_Matrix4x4<value_t> m3(*this);
		return m3 += m2;
	}
	const _Matrix4x4<value_t> operator-(const _Matrix4x4<value_t> & m2) const {
		_Matrix4x4<value_t> m3(*this);
		return m3 -= m2;
	}
	const _Matrix4x4<value_t> operator*(const _Matrix4x4<value_t> & m2) const {
		_Matrix4x4<value_t> m3(*this);
		m3 *= m2;
		return m3;
	}
	const _Matrix4x4<value_t> operator*(value_t d) const {
		_Matrix4x4<value_t> m3(*this);
		return m3 *= d;
	}
	const _Matrix4x4<value_t> operator/(value_t d) const {
		_Matrix4x4<value_t> m3(*this);
		return m3 /= d;
	}
	const vec3_t transformDirection(const vec3_t & v) const {
		return transformDirection(v.x(), v.y(), v.z());
	}
	const vec3_t transformDirection(value_t x, value_t y, value_t z) const {
		/*
		 * w component of a direction is always zero. Simply ignore w here,
		 * because there is no representation of a homogeneous coordinate
		 * with w=0 in cartesian coordinates. Affine transformations always
		 * lead to w=0 after multiplication with a direction. Overall, this
		 * leads to multiplication with the upper-left 3x3 submatrix.
		 */
		return vec3_t(m[0] * x + m[1] * y + m[2] * z, m[4] * x + m[5] * y + m[6] * z, m[8] * x + m[9] * y + m[10] * z);
	}
	const vec3_t transformPosition(const vec3_t & v) const {
		return transformPosition(v.x(), v.y(), v.z());
	}
	const vec3_t transformPosition(value_t x, value_t y, value_t z) const {
		const value_t w = m[12] * x + m[13] * y + m[14] * z + m[15];
		if (w == 0) {
			return vec3_t(0, 0, 0);
		}
		const value_t wInv = 1 / w;
		return vec3_t((m[0] * x + m[1] * y + m[2] * z + m[3]) * wInv, (m[4] * x + m[5] * y + m[6] * z + m[7]) * wInv,
					  (m[8] * x + m[9] * y + m[10] * z + m[11]) * wInv);
	}
	const vec4_t operator*(const vec4_t & v) const {
		return vec4_t(m[0] * v.x() + m[1] * v.y() + m[2] * v.z() + m[3] * v.w(),
					  m[4] * v.x() + m[5] * v.y() + m[6] * v.z() + m[7] * v.w(),
					  m[8] * v.x() + m[9] * v.y() + m[10] * v.z() + m[11] * v.w(),
					  m[12] * v.x() + m[13] * v.y() + m[14] * v.z() + m[15] * v.w());
	}
	const srt_t operator*(const srt_t & srt) const {
		const Vec3 pos((*this).transformPosition(srt.getTranslation()));
		const Vec3 dir((*this).transformDirection(srt.getDirVector()));
		const Vec3 up((*this).transformDirection(srt.getUpVector()));
		const float newScale = dir.length() * srt.getScale();
		return srt_t(pos, dir, up, newScale);
	}

	/**
	 *   @return the inverse matrix of this
	 *   @exception a division by zero will occur if det is zero
	 *   @note numerical problems will occur if det is nearly zero
	 *   @note a new matrix is created, this remains unchanged
	 */
	_Matrix4x4<value_t> inverse() const {
		_Matrix4x4<value_t> ret;

		value_t d = 1 / det();

		ret.m[0] = (m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14]
					+ m[13] * m[6] * m[11] - m[13] * m[7] * m[10]) * d;
		ret.m[1] = (m[1] * m[10] * m[15] - m[1] * m[11] * m[14] - m[9] * m[2] * m[15] + m[9] * m[3] * m[14]
					+ m[13] * m[2] * m[11] - m[13] * m[3] * m[10]) * -d;
		ret.m[2] = (-m[1] * m[6] * m[15] + m[1] * m[7] * m[14] + m[5] * m[2] * m[15] - m[5] * m[3] * m[14]
					- m[13] * m[2] * m[7] + m[13] * m[3] * m[6]) * -d;
		ret.m[3] = (m[1] * m[6] * m[11] - m[1] * m[7] * m[10] - m[5] * m[2] * m[11] + m[5] * m[3] * m[10]
					+ m[9] * m[2] * m[7] - m[9] * m[3] * m[6]) * -d;

		ret.m[4] = (m[4] * m[10] * m[15] - m[4] * m[11] * m[14] - m[8] * m[6] * m[15] + m[8] * m[7] * m[14]
					+ m[12] * m[6] * m[11] - m[12] * m[7] * m[10]) * -d;
		ret.m[5] = (m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14]
					+ m[12] * m[2] * m[11] - m[12] * m[3] * m[10]) * d;
		ret.m[6] = (m[0] * m[6] * m[15] - m[0] * m[7] * m[14] - m[4] * m[2] * m[15] + m[4] * m[3] * m[14]
					+ m[12] * m[2] * m[7] - m[12] * m[3] * m[6]) * -d;
		ret.m[7] = (-m[0] * m[6] * m[11] + m[0] * m[7] * m[10] + m[4] * m[2] * m[11] - m[4] * m[3] * m[10]
					- m[8] * m[2] * m[7] + m[8] * m[3] * m[6]) * -d;

		ret.m[8] = (-m[4] * m[9] * m[15] + m[4] * m[11] * m[13] + m[8] * m[5] * m[15] - m[8] * m[7] * m[13]
					- m[12] * m[5] * m[11] + m[12] * m[7] * m[9]) * -d;
		ret.m[9] = (m[0] * m[9] * m[15] - m[0] * m[11] * m[13] - m[8] * m[1] * m[15] + m[8] * m[3] * m[13]
					+ m[12] * m[1] * m[11] - m[12] * m[3] * m[9]) * -d;
		ret.m[10] = (m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13]
					 + m[12] * m[1] * m[7] - m[12] * m[3] * m[5]) * d;
		ret.m[11] = (m[0] * m[5] * m[11] - m[0] * m[7] * m[9] - m[4] * m[1] * m[11] + m[4] * m[3] * m[9]
					 + m[8] * m[1] * m[7] - m[8] * m[3] * m[5]) * -d;

		ret.m[12] = (m[4] * m[9] * m[14] - m[4] * m[10] * m[13] - m[8] * m[5] * m[14] + m[8] * m[6] * m[13]
					 + m[12] * m[5] * m[10] - m[12] * m[6] * m[9]) * -d;
		ret.m[13] = (-m[0] * m[9] * m[14] + m[0] * m[10] * m[13] + m[8] * m[1] * m[14] - m[8] * m[2] * m[13]
					 - m[12] * m[1] * m[10] + m[12] * m[2] * m[9]) * -d;
		ret.m[14] = (m[0] * m[5] * m[14] - m[0] * m[6] * m[13] - m[4] * m[1] * m[14] + m[4] * m[2] * m[13]
					 + m[12] * m[1] * m[6] - m[12] * m[2] * m[5]) * -d;
		ret.m[15] = (m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9]
					 + m[8] * m[1] * m[6] - m[8] * m[2] * m[5]) * d;

		return ret;
	}
	/**
	 * Returns a new matrix whose rows and columns are exchanged.
	 */
	_Matrix4x4<value_t> getTransposed() const {
		return _Matrix4x4<value_t>(m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3],
								   m[7], m[11], m[15]);
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
	inline bool equals(const _Matrix4x4 & other, value_t epsilon) const {
		for (uint_fast8_t i = 0; i < 16; i++)
			if (std::abs(m[i] - other.m[i]) > epsilon)
				return false;
		return true;
	}
	inline bool operator==(const _Matrix4x4<value_t> & m2) const {
		for (uint_fast8_t i = 0; i < 16; i++)
			if (m[i] != m2.m[i])
				return false;
		return true;
	}
	inline bool operator!=(const _Matrix4x4<value_t> & m2) const {
		return !(*this == m2);
	}
	//@}

	//! Can be used to check whether it is (relatively) safe to convert a matrix to an SRT without loss of information.
	bool convertsSafelyToSRT() const {
		const vec3_t ex = getColumnAsVec3(0);
		const vec3_t ey = getColumnAsVec3(1);
		const vec3_t ez = getColumnAsVec3(2);

		const value_t l1 = ex.lengthSquared();
		const value_t l2 = ey.lengthSquared();
		const value_t l3 = ez.lengthSquared();

		const value_t squaredLengthEpsilon = std::numeric_limits<value_t>::epsilon();
		const value_t epsilon = 1.0e-3;

		return l1 >= squaredLengthEpsilon && l2 >= squaredLengthEpsilon && l3 >= squaredLengthEpsilon
				&& std::abs((l1 / l2) - 1) <= epsilon && std::abs((l2 / l3) - 1) <= epsilon;
	}

	//! Converts a matrix to a SRT; throws an exception if the transformation can not be captured by a SRT.
	srt_t toSRT() const {
		if (!convertsSafelyToSRT())
			throw std::domain_error("Matrix can not be converted to SRT.");
		return std::move(_toSRT());
	}

	//! Converts a matrix to a SRT even if information may be lost.
	srt_t _toSRT() const {
		const vec3_t right = getColumnAsVec3(0);
		const vec3_t up = getColumnAsVec3(1);
		const vec3_t dir = getColumnAsVec3(2);
		const vec3_t pos = getColumnAsVec3(3);

		const value_t lengthRight = right.length();
		const value_t lengthUp = up.length();
		const value_t lengthDir = dir.length();

		const vec3_t rightNorm = right / lengthRight;
		const vec3_t upNorm = up / lengthUp;
		const vec3_t dirNorm = dir / lengthDir;

		// Check if the resulting coordinate system is right-handed.
		if (!rightNorm.cross(upNorm).equals(dirNorm, 1.0e-3)) {
			// Negative scaling.
			return srt_t(pos, -dirNorm, -upNorm, -lengthDir);
		}
		return srt_t(pos, dirNorm, upNorm, lengthDir);
	}

	//@}

	//! @name Serialization
	//@{
public:
	friend std::ostream & operator<<(std::ostream & out, const _Matrix4x4<value_t> & matrix) {
		return out << matrix.m[0] << ' ' << matrix.m[1] << ' ' << matrix.m[2] << ' ' << matrix.m[3] << ' '
				   << matrix.m[4] << ' ' << matrix.m[5] << ' ' << matrix.m[6] << ' ' << matrix.m[7] << ' '
				   << matrix.m[8] << ' ' << matrix.m[9] << ' ' << matrix.m[10] << ' ' << matrix.m[11] << ' '
				   << matrix.m[12] << ' ' << matrix.m[13] << ' ' << matrix.m[14] << ' ' << matrix.m[15];
	}
	friend std::istream & operator>>(std::istream & in, _Matrix4x4<value_t> & matrix) {
		return in >> matrix.m[0] >> matrix.m[1] >> matrix.m[2] >> matrix.m[3] >> matrix.m[4] >> matrix.m[5]
				>> matrix.m[6] >> matrix.m[7] >> matrix.m[8] >> matrix.m[9] >> matrix.m[10] >> matrix.m[11]
				>> matrix.m[12] >> matrix.m[13] >> matrix.m[14] >> matrix.m[15];
	}
	//@}
};

typedef _Matrix4x4<float> Matrix4x4f;
typedef _Matrix4x4<double> Matrix4x4d;
typedef _Matrix4x4<float> Matrix4x4;
}

#endif /* GEOMETRY_MATRIX4X4_H */

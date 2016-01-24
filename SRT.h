/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_SRT_H
#define GEOMETRY_SRT_H

#include "Angle.h"
#include "Interpolation.h"
#include "Matrix3x3.h"
#include "Quaternion.h"
#include "Vec3.h"
#include <array>
#include <istream>
#include <ostream>

namespace Geometry {

/**
 * SRT - scale,rotate and translate.
 * @see 3D Game Engine Design, David H. Eberly, http://www.geometrictools.com/
 *
 *  [SRT]
 */
template <typename T_>
class _SRT {
public:
	typedef T_ value_t;
	typedef _Matrix3x3<value_t> matrix3x3_t;
	typedef _Angle<value_t> angle_t;
	typedef _Vec3<value_t> vec3_t;

private:
	//! Scale
	value_t s;
	//! Rotation
	matrix3x3_t r;
	//! Translation
	vec3_t t;

	uint16_t rotationCounter;

public:
	/**
	 * @name Mainvec3_t
	 */
	//@{
	/**
	 * [ctor]
	 */
	_SRT() : s(static_cast<value_t>(1.0)), r(), t(), rotationCounter(0) {
	}
	/**
	 * [ctor] interpolation between srt1 and srt2, according to the factor blend (should be between 0 and 1);
	 */
	_SRT(const _SRT<value_t> & srt1, const _SRT<value_t> & srt2, float blend)
			: s(Interpolation::linear(srt1.s, srt2.s, blend)),
			  r(srt1.r, srt2.r, blend),
			  t(srt1.t, srt2.t, blend),
			  rotationCounter(0) {
	}

	//! Create an SRT by specifying all components explicitly.
	_SRT(const vec3_t & translation, const matrix3x3_t & rotation, value_t scaling)
			: s(scaling), r(rotation), t(translation), rotationCounter(0) {
		r.normOrthoLize();
	}

	/**
	 * [ctor]  pos, dir, up [,scale]
	 *	\note dir and up are normalized automatically.
	 */
	_SRT(vec3_t _pos, const vec3_t & _dir, const vec3_t & _up, value_t _scale = 1.0)
			: s(_scale), r(), t(std::move(_pos)), rotationCounter(0) {
		r.setRotation(_dir, _up);
	}

	/*! [ x,y,z, rx,ry,rz,rw, scale ]
		\see toArray()	*/
	_SRT(const std::array<value_t, 8> & arr)
			: s(arr[7]),
			  r(Quaternion(arr[3], arr[4], arr[5], arr[6]).toMatrix()),
			  t(arr[0], arr[1], arr[2]),
			  rotationCounter(0) {
	}

	/**
	 * @name Information
	 */
	//@{
	value_t getScale() const {
		return s;
	}
	const vec3_t & getTranslation() const {
		return t;
	}
	const matrix3x3_t & getRotation() const {
		return r;
	}
	vec3_t getDirVector() const {
		return r.getCol(matrix3x3_t::FRONT);
	}
	vec3_t getRightVector() const {
		return r.getCol(matrix3x3_t::RIGHT);
	}
	vec3_t getUpVector() const {
		return r.getCol(matrix3x3_t::UP);
	}
	//@}

	/**
	 * @name Modification
	 */
	//@{
	void reset() {
		rotationCounter = 0;
		r.setIdentity();
		t.setValue(static_cast<value_t>(0.0));
		s = 1.0;
	}
	void setScale(value_t x) {
		s = x;
	}
	void scale(value_t x) {
		s *= x;
	}

	void resetRotation() {
		rotationCounter = 0;
		r.setIdentity();
	}
	void setRotation(const matrix3x3_t & x) {
		rotationCounter = 0;
		r = x;
		r.normOrthoLize();
	}
	void setRotation(const vec3_t & dir, const vec3_t & up) {
		rotationCounter = 0;
		r.setRotation(dir, up);
	}

	void setTranslation(const vec3_t & x) {
		t = x;
	}
	void translate(const vec3_t & v) {
		t += v;
	}
	void translateLocal(const vec3_t & v) {
		t += r * v * s;
	}

	void rotateLocal(const angle_t & angle, const vec3_t & axis) {
		if (((rotationCounter += static_cast<uint16_t>(100 * angle.rad())) + 1) > 200) {
			rotationCounter = 0;
			r.normOrthoLize();
		}
		r = r * matrix3x3_t::createRotation(angle, axis);
	}
	void rotateLocal_rad(float rad, const vec3_t & axis) {
		rotateLocal(angle_t::rad(rad), axis);
	}
	void rotateLocal_deg(float deg, const vec3_t & axis) {
		rotateLocal(angle_t::deg(deg), axis);
	}

	void rotateRel(const angle_t & angle, const vec3_t & axis) {
		if (((rotationCounter += static_cast<uint16_t>(100 * angle.rad())) + 1) > 200) {
			rotationCounter = 0;
			r.normOrthoLize();
		}
		r = matrix3x3_t::createRotation(angle, axis) * r;
	}

	void rotateRel_rad(value_t rad, const vec3_t & axis) {
		rotateRel(angle_t::rad(rad), axis);
	}
	void rotateRel_deg(value_t deg, const vec3_t & axis) {
		rotateRel(angle_t::deg(deg), axis);
	}
	//@}

	/**
	 * @name Creation
	 */
	//@{
	const vec3_t operator*(const vec3_t & v) const {
		return r * v * s + t;
	}
	const _SRT<value_t> operator*(const _SRT<value_t> & srt) const {
		_SRT<value_t> e;
		e.s = s * srt.s;
		e.t = r * srt.t * e.s + t;
		e.r = r * srt.r;
		return e;
	}

	/*! Like a multiplication, but applies the rotation of this srt to translation
		of the given srt which results in the same behaviour as
		( Matrix4x4 (*this) * Matrix4x4(srt) )._toSRT() */
	_SRT<value_t> getTransformation(const _SRT<value_t> & srt) const {
		const vec3_t pos((*this) * srt.getTranslation());
		const vec3_t dir((*this) * (srt.getDirVector() + srt.getTranslation()) - pos);
		const vec3_t up((*this) * (srt.getUpVector() + srt.getTranslation()) - pos);
		return _SRT<value_t>(pos, dir, up, dir.length() * srt.getScale());
	}
	_SRT<value_t> inverse() const {
		_SRT<value_t> e;
		e.s = 1 / s;
		e.r = r.getTransposed();
		e.t = e.r * t * (-e.s);
		return e;
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
	bool equals(const _SRT & other, value_t epsilon) const {
		return t.equals(other.t, epsilon) && r.equals(other.r, epsilon) && std::abs(s - other.s) <= epsilon;
	}
	bool operator==(const _SRT<value_t> & srt) const {
		return srt.t == (*this).t && srt.r == (*this).r && srt.s == (*this).s;
	}
	bool operator!=(const _SRT<value_t> & srt) const {
		return !((*this) == srt);
	}
	//@}

	/**
	 * @name Conversion
	 */
	//@{
	void toArray(value_t fa[16]) const {
		fa[0] = static_cast<value_t>(r.at(0, 0) * s);
		fa[4] = static_cast<value_t>(r.at(1, 0) * s);
		fa[8] = static_cast<value_t>(r.at(2, 0) * s);
		fa[12] = static_cast<value_t>(0.0f);
		fa[1] = static_cast<value_t>(r.at(0, 1) * s);
		fa[5] = static_cast<value_t>(r.at(1, 1) * s);
		fa[9] = static_cast<value_t>(r.at(2, 1) * s);
		fa[13] = static_cast<value_t>(0.0f);
		fa[2] = static_cast<value_t>(r.at(0, 2) * s);
		fa[6] = static_cast<value_t>(r.at(1, 2) * s);
		fa[10] = static_cast<value_t>(r.at(2, 2) * s);
		fa[14] = static_cast<value_t>(0.0f);
		fa[3] = static_cast<value_t>(t.getX());
		fa[7] = static_cast<value_t>(t.getY());
		fa[11] = static_cast<value_t>(t.getZ());
		fa[15] = static_cast<value_t>(1.0f);
	}

	std::array<value_t, 8> toArray() const {
		std::array<value_t, 8> arr;
		arr[0] = t.x(), arr[1] = t.y(), arr[2] = t.z();
		const auto & q = Quaternion::matrixToQuaternion(r);
		arr[3] = q.x(), arr[4] = q.y(), arr[5] = q.z(), arr[6] = q.w();
		arr[7] = s;
		return arr;
	}
	//@}

	//! @name Serialization
	//@{
	friend std::ostream & operator<<(std::ostream & out, const _SRT & srt) {
		return out << srt.s << ' ' << srt.r << ' ' << srt.t;
	}
	friend std::istream & operator>>(std::istream & in, _SRT & srt) {
		return in >> srt.s >> srt.r >> srt.t;
	}
	//@}
};
typedef _SRT<float> SRT;
typedef _SRT<float> SRTf;
typedef _SRT<double> SRTd;
}

#endif /* GEOMETRY_SRT_H */

/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>
	Copyright (C) 2020 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_SQT_H
#define GEOMETRY_SQT_H

#include "Angle.h"
#include "Interpolation.h"
#include "Quaternion.h"
#include "Matrix3x3.h"
#include "Vec3.h"
#include "SRT.h"
#include <array>
#include <istream>
#include <ostream>

namespace Geometry {

/**
 * SQT - scale, quaternion and translate.
 * A more compact qay to store scale, rotation & translation.
 * 
 * @note internally the values are stored as translate, scale, quaternion to get a more GPU friendly memory layout (2 x vec4).
 *
 *  [SQT]
 */
template <typename T_>
class _SQT {
public:
	typedef T_ value_t;
	typedef _Quaternion<value_t> quaternion_t;
	typedef _Angle<value_t> angle_t;
	typedef _Vec3<value_t> vec3_t;

private:
	//! Translation
	vec3_t t;
	//! Scale
	value_t s;
	//! Rotation
	quaternion_t q;

public:
	/**
	 * @name Mainvec3_t
	 */
	//@{
	/**
	 * [ctor]
	 */
	_SQT() : t(), s(static_cast<value_t>(1.0)), q() {
	}
	/**
	 * [ctor] interpolation between sqt1 and sqt2, according to the factor blend (should be between 0 and 1);
	 */
	_SQT(const _SQT<value_t> & sqt1, const _SQT<value_t> & sqt2, float blend)
			: t(sqt1.t, sqt2.t, blend),
				s(Interpolation::linear(sqt1.s, sqt2.s, blend)),
				q(Interpolation::linear(sqt1.q, sqt2.q, blend)){
	}

	//! Create an SQT by specifying all components explicitly.
	_SQT(const vec3_t & translation, const quaternion_t & rotation, value_t scaling)
			: t(translation), s(scaling), q(rotation) {
		q.normOrthoLize();
	}

	/**
	 * [ctor] Create an SQT from an SRT
	 */
	_SQT(const _SRT<value_t> & srt)
			: t(srt.getTranslation()), 
				s(srt.getScale()),
				q(quaternion_t::matrixToQuaternion(srt.getRotation())) {
	}

	/**
	 * [ctor]  pos, dir, up [,scale]
	 *	\note dir and up are normalized automatically.
	 */
	_SQT(vec3_t _pos, const vec3_t & _dir, const vec3_t & _up, value_t _scale = 1.0) : _SQT(_SRT<value_t>(_pos, _dir, _up, _scale)) {
	}

	/*! [ x,y,z, scale, rx,ry,rz,rw ]
		\see toArray()	*/
	_SQT(const std::array<value_t, 8> & arr)
			: t(arr[0], arr[1], arr[2]),
				s(arr[3]),
				q(Quaternion(arr[4], arr[5], arr[6], arr[7])) {
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
	const quaternion_t & getRotation() const {
		return q;
	}
	const _Matrix3x3<value_t> & getRotationMatrix() const {
		return q.toMatrix();
	}
	_SRT<value_t> toSRT() const {
		return _SRT<value_t>(t, q.toMatrix(), s);
	}
	//@}

	/**
	 * @name Modification
	 */
	//@{
	void reset() {
		q.set(static_cast<value_t>(0.0), static_cast<value_t>(0.0), static_cast<value_t>(0.0), static_cast<value_t>(1.0));
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
		q.set(static_cast<value_t>(0.0), static_cast<value_t>(0.0), static_cast<value_t>(0.0), static_cast<value_t>(1.0));
	}
	void setRotation(const quaternion_t & x) {
		q = x;
	}
	void setRotation(const vec3_t & dir, const vec3_t & up) {
		_Matrix3x3<value_t> m;
		m.setRotation(dir, up);
		q = quaternion_t::matrixToQuaternion(m);
	}

	void setTranslation(const vec3_t & x) {
		t = x;
	}
	void translate(const vec3_t & v) {
		t += v;
	}
	void translateLocal(const vec3_t & v) {
		t += q.rotatePoint(v * s);
	}

	void rotateLocal(const angle_t & angle, const vec3_t & axis) {
		quaternion_t q2;
		q2.makeRotate(angle, axis);
		q *= q2;
	}
	void rotateLocal_rad(float rad, const vec3_t & axis) {
		rotateLocal(angle_t::rad(rad), axis);
	}
	void rotateLocal_deg(float deg, const vec3_t & axis) {
		rotateLocal(angle_t::deg(deg), axis);
	}

	void rotateRel(const angle_t & angle, const vec3_t & axis) {
		quaternion_t q2;
		q2.makeRotate(angle, axis);
		q = q2 * q;
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
		return q.rotatePoint(v * s) + t;
	}
	const _SQT<value_t> operator*(const _SQT<value_t> & sqt) const {
		_SQT<value_t> e;
		e.s = s * sqt.s;
		e.t = q.rotatePoint(sqt.t * e.s) + t;
		e.q = q * sqt.q;
		return e;
	}

	/*! Like a multiplication, but applies the rotation of this sqt to translation
		of the given sqt which results in the same behaviour as
		( Matrix4x4 (*this) * Matrix4x4(sqt) )._toSQT() */
	_SQT<value_t> getTransformation(const _SQT<value_t> & sqt) const {
		const vec3_t pos((*this) * sqt.getTranslation());
		const vec3_t dir((*this) * (sqt.getDirVector() + sqt.getTranslation()) - pos);
		const vec3_t up((*this) * (sqt.getUpVector() + sqt.getTranslation()) - pos);
		return _SQT<value_t>(pos, dir, up, dir.length() * sqt.getScale());
	}
	_SQT<value_t> inverse() const {
		_SQT<value_t> e;
		e.s = 1 / s;
		e.q = q.inverse();
		e.t = e.q.rotatePoint(t * (-e.s));
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
	bool equals(const _SQT & other, value_t epsilon) const {
		return t.equals(other.t, epsilon) && q.equals(other.q, epsilon) && std::abs(s - other.s) <= epsilon;
	}
	bool operator==(const _SQT<value_t> & sqt) const {
		return sqt.t == (*this).t && sqt.q == (*this).q && sqt.s == (*this).s;
	}
	bool operator!=(const _SQT<value_t> & sqt) const {
		return !((*this) == sqt);
	}
	//@}

	/**
	 * @name Conversion
	 */
	//@{
	void toArray(value_t fa[8]) const {
		fa[0] = static_cast<value_t>(t.getX());
		fa[1] = static_cast<value_t>(t.getY());
		fa[2] = static_cast<value_t>(t.getZ());
		fa[3] = static_cast<value_t>(s);
		fa[4] = static_cast<value_t>(q.x());
		fa[5] = static_cast<value_t>(q.y());
		fa[6] = static_cast<value_t>(q.z());
		fa[7] = static_cast<value_t>(q.w());
	}

	std::array<value_t, 8> toArray() const {
		std::array<value_t, 8> arr;
		arr[0] = t.x(), arr[1] = t.y(), arr[2] = t.z();
		arr[3] = s;
		arr[4] = q.x(), arr[5] = q.y(), arr[6] = q.z(), arr[7] = q.w();
		return arr;
	}
	//@}

	//! @name Serialization
	//@{
	friend std::ostream & operator<<(std::ostream & out, const _SQT & sqt) {
		return out << sqt.t << ' ' << sqt.s << ' ' << sqt.q;
	}
	friend std::istream & operator>>(std::istream & in, _SQT & sqt) {
		return in >> sqt.t >> sqt.s >> sqt.q;
	}
	//@}
};
typedef _SQT<float> SQT;
typedef _SQT<float> SQTf;
typedef _SQT<double> SQTd;
}

#endif /* GEOMETRY_SQT_H */

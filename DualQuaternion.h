/*
 This file is part of the Geometry library.
 Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
 Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
 Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

 This library is subject to the terms of the Mozilla Public License, v. 2.0.
 You should have received a copy of the MPL along with this library; see the
 file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GEOMETRY_DUALQUATERNION_H
#define GEOMETRY_DUALQUATERNION_H

#include "Interpolation.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Vec3.h"

namespace Geometry {
template <typename T_>
/**
 * Class used to represent a rotation and positoin replacement as dual numbers based on quaternions.
 * Currently does not support scaling in any form!
 *
 * Dual Quaternions allow transformation and deformation of an mesh without artefacts. Such gimbal Lock
 * or texture artefacts caused by false interpolation and / or transformation.
 *
 * The dual part describes the rotation and the non dualpart the position replacement.
 *
 * @author Lukas Kopecki
 * @date 2012-08-07
 */
class _DualQuaternion {
public:
	using value_t = T_;
	using vec3_t = _Vec3<value_t>;

	/*
	 * [ Constructor ]
	 */
	_DualQuaternion() {
		setRotation(0.0, 0.0, 0.0, 1.0);
		setTranslation(0.0, 0.0, 0.0, 1.0);
	}

	_DualQuaternion(value_t _rx, value_t _ry, value_t _rz, value_t _rw, value_t _tx, value_t _ty, value_t _tz,
					value_t _tw) {
		setRotation(_rx, _ry, _rz, _rw);
		setTranslation(_tx, _ty, _tz, _tw);
	}

	_DualQuaternion(const Quaternion & rotation, const vec3_t & translation) {
		set(rotation, translation);
	}

	_DualQuaternion(const Quaternion & rotation, const Quaternion & translation) {
		setRotation(rotation.x(), rotation.y(), rotation.z(), rotation.w());
		setTranslation(translation.x(), translation.y(), translation.z(), translation.w());
	}

	_DualQuaternion(const _DualQuaternion & src) {
		setRotation(src.getRotation());
		setTranslation(src.getTranslation());
	}

	/*
	 * [ getter & setter ]
	 */

	const value_t & rX() const {
		return rot[0];
	}

	const value_t & rY() const {
		return rot[1];
	}

	const value_t & rZ() const {
		return rot[2];
	}

	const value_t & rW() const {
		return rot[3];
	}

	const value_t & tX() const {
		return trans[0];
	}

	const value_t & tY() const {
		return trans[1];
	}

	const value_t & tZ() const {
		return trans[2];
	}

	const value_t & tW() const {
		return trans[3];
	}

	void setRX(value_t _rx) {
		rot[0] = _rx;
	}

	void setRY(value_t _ry) {
		rot[1] = _ry;
	}

	void setRZ(value_t _rz) {
		rot[2] = _rz;
	}

	void setRW(value_t _rw) {
		rot[3] = _rw;
	}

	void setTX(value_t _tx) {
		trans[0] = _tx;
	}

	void setTY(value_t _ty) {
		trans[1] = _ty;
	}

	void setTZ(value_t _tz) {
		trans[2] = _tz;
	}

	void setTW(value_t _tw) {
		trans[3] = _tw;
	}

	void setRotation(value_t _x, value_t _y, value_t _z, value_t _w) {
		setRX(_x);
		setRY(_y);
		setRZ(_z);
		setRW(_w);
	}

	void setRotation(const Quaternion & _rot) {
		setRotation(_rot.x(), _rot.y(), _rot.z(), _rot.w());
	}

	const Quaternion getRotation() const {
		return Quaternion(rot[0], rot[1], rot[2], rot[3]);
	}

	void setTranslation(value_t _x, value_t _y, value_t _z, value_t _w) {
		setTX(_x);
		setTY(_y);
		setTZ(_z);
		setTW(_w);
	}

	void setTranslation(const Quaternion & _trans) {
		setTranslation(_trans.x(), _trans.y(), _trans.z(), _trans.w());
	}

	const Quaternion getTranslation() const {
		return Quaternion(trans[0], trans[1], trans[2], trans[3]);
	}

	const vec3_t getTranslationAsVec3() const {
		return vec3_t((2.0 * (-trans[3] * rot[0] + trans[0] * rot[3] - trans[1] * rot[2] + trans[2] * rot[1])),
					  (2.0 * (-trans[3] * rot[1] + trans[0] * rot[2] + trans[1] * rot[3] - trans[2] * rot[0])),
					  (2.0 * (-trans[3] * rot[2] - trans[0] * rot[1] + trans[1] * rot[0] + trans[2] * rot[3])));
	}

	const _DualQuaternion normalizeRotation() const {
		value_t magn = 1 / std::sqrt(getRotation().dot(getRotation()));
		return _DualQuaternion(getRotation() * magn, getTranslation() * magn);
	}

	const _DualQuaternion normalizeTranslation() const {
		value_t magn = 1.0 / std::sqrt(getRotation().dot(getRotation()));
		return _DualQuaternion(getRotation(), getTranslation()
									   - getRotation().operator*((getRotation().dot(getTranslation())) * magn));
	}

	const _DualQuaternion normalize() const {
		return normalizeRotation().normalizeTranslation();
	}

	const value_t dotRotation(const Quaternion & rotation) const {
		return getRotation().dot(rotation);
	}

	const value_t dotTranslation(const Quaternion & translation) const {
		return getTranslation().dot(translation);
	}

	const value_t dot(const _DualQuaternion & dua) const {
		return dotRotation(dua.getRotation()) + dotTranslation(dua.getTranslation());
	}

	_DualQuaternion conjugate() const {
		return _DualQuaternion(getRotation().conjugate(), getTranslation().conjugate());
	}

	/* source:
	 * http://isg.cs.tcd.ie/kavanl/dq/dqconv.c
	 * Converts a unit quaternion and a translation vector into one dual quaternion.
	 */
	void set(const Quaternion & q, const vec3_t & t) {
		// regular quaternion (just copy the non-dual part):
		rot[0] = q.x();
		rot[1] = q.y();
		rot[2] = q.z();
		rot[3] = q.w();

		// translation vector:
		trans[0] = 0.5 * (t.x() * q.w() + t.y() * q.z() - t.z() * q.y());
		trans[1] = 0.5 * (-t.x() * q.z() + t.y() * q.w() + t.z() * q.x());
		trans[2] = 0.5 * (t.x() * q.y() - t.y() * q.x() + t.z() * q.w());
		trans[3] = -0.5 * (t.x() * q.x() + t.y() * q.y() + t.z() * q.z());
	}

	/* source:
	 * http://isg.cs.tcd.ie/kavanl/dq/dqconv.c
	 * Converts one dual quaternion into one unit quaternion and one vector.
	 */
	void getQuaternionAndVec3(Quaternion & q, vec3_t & t) const {
		// regular quaternion (just copy the non-dual part):
		q.set(rot[0], rot[1], rot[2], rot[3]);
		// translation vector:
		t.setX(2.0 * (-trans[3] * rot[0] + trans[0] * rot[3] - trans[1] * rot[2] + trans[2] * rot[1]));
		t.setY(2.0 * (-trans[3] * rot[1] + trans[0] * rot[2] + trans[1] * rot[3] - trans[2] * rot[0]));
		t.setZ(2.0 * (-trans[3] * rot[2] - trans[0] * rot[1] + trans[1] * rot[0] + trans[2] * rot[3]));
	}

	/**
	 * [ Basic functions ]
	 */

	const value_t lengthRotation() const {
		return std::sqrt(length2Rotation());
	}

	const value_t length2Rotation() const {
		return rot[0] * rot[0] + rot[1] * rot[1] + rot[2] * rot[2] + rot[3] * rot[3];
	}

	const value_t lengthTranslation() const {
		return std::sqrt(length2Rotation());
	}

	const value_t length2Translation() const {
		return trans[0] * trans[0] + trans[1] * trans[1] + trans[2] * trans[2] + trans[3] * trans[3];
	}

	const value_t length() const {
		value_t l = getRotation().length();
		return l + getRotation().dot(getTranslation()) / l;
	}

	_DualQuaternion & operator=(const _DualQuaternion & src) {
		if (this != &src) {
			setRotation(src.getRotation());
			setTranslation(src.getTranslation());
		}

		return *this;
	}

	const _DualQuaternion operator*(const value_t factor) const {
		return _DualQuaternion(rot[0] * factor, rot[1] * factor, rot[2] * factor, rot[3] * factor, trans[0] * factor,
							   trans[1] * factor, trans[2] * factor, trans[3] * factor);
	}

	const _DualQuaternion operator/(const value_t factor) const {
		return (factor != 0.0) ? operator*(1.0 / factor) : _DualQuaternion::identity();
	}

	const _DualQuaternion operator+(const _DualQuaternion & dq) const {
		return _DualQuaternion(rX() + dq.rX(), rY() + dq.rY(), rZ() + dq.rZ(), rW() + dq.rW(), tX() + dq.tX(),
							   tY() + dq.tY(), tZ() + dq.tZ(), tW() + dq.tW());
	}

	const _Matrix4x4<value_t> toMatrix() const {
		Quaternion rotation = Quaternion(rot[0], rot[1], rot[2], rot[3]);
		if (rotation.length() != 0)
			rotation /= rotation.length();
		_Matrix4x4<value_t> mat(rotation.toMatrix());
		vec3_t v = getTranslationAsVec3();
		mat[3] = v.x();
		mat[7] = v.y();
		mat[11] = v.z();

		return mat;
	}

	void setFromMatrix(const _Matrix4x4<value_t> & matrix) {
		_DualQuaternion tmp = convertFromMatrix(matrix);

		set(tmp.getRotation(), tmp.getTranslationAsVec3());
	}

	/*
	 *  [ Static ]
	 */

	static _DualQuaternion identity() {
		return _DualQuaternion(0, 0, 0, 1, 0, 0, 0, 1);
	}

	static _DualQuaternion dualQuaternionLinearInterpolation(const _DualQuaternion & dq1, const _DualQuaternion & dq2,
															 const value_t factor, bool takeShortestArc = false) {
		value_t usedFactor = factor;
		if (takeShortestArc) {
			if (dq1.dot(dq2) < 0.0)
				usedFactor = -factor;
		}

		_DualQuaternion idq((dq1 * (1.0 - usedFactor)) + (dq2 * usedFactor));

		value_t length = idq.length();
		idq.normalize();
		return (length != 0) ? idq / idq.length() : _DualQuaternion::identity();
	}

	static _DualQuaternion convertFromMatrix(const _Matrix4x4<value_t> & matrix) {
		Quaternion rotation;
		vec3_t translation(matrix.at(3), matrix.at(7), matrix.at(11));

		_Matrix3x3<value_t> rotMat(matrix.at(0), matrix.at(1), matrix.at(2), matrix.at(4), matrix.at(5), matrix.at(6),
								   matrix.at(8), matrix.at(9), matrix.at(10));
		rotation = Quaternion::matrixToQuaternion(rotMat);
		return _DualQuaternion(rotation, translation);
	}

	/*
	 *  [ Serialization ]
	 */
	friend std::ostream & operator<<(std::ostream & out, const _DualQuaternion & dq) {
		return out << dq.rX() << ' ' << dq.rY() << ' ' << dq.rZ() << ' ' << dq.rW() << ' ' << dq.tX() << ' ' << dq.tY()
				   << ' ' << dq.tZ() << ' ' << dq.tW();
	}

private:
	value_t rot[4]; // dual part representing rotation
	value_t trans[4]; // non dual part for position displacement
};

using DualQuaternion = _DualQuaternion<float>;
using DualQuaternionf = _DualQuaternion<float>;
using DualQuaterniond = _DualQuaternion<double>;
}

#endif /* GEOMETRY_DUALQUATERNION_H */

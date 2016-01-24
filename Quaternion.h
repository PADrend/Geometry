/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_QUATERNION_H
#define GEOMETRY_QUATERNION_H

#include "Angle.h"
#include "Interpolation.h"
#include "Matrix3x3.h"
#include "Vec3.h"
#include <algorithm>
#include <cmath>
#include <istream>
#include <ostream>

namespace Geometry {
/**
 * Class used to represent a rotation as a quaternion.
 *
 * @author Benjamin Eikel
 * @date 2009-12-13
 */
class Quaternion {
public:
	typedef float value_t;
	typedef _Angle<value_t> angle_t;
	typedef _Vec3<value_t> vec3_t;

	Quaternion() {
		set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Quaternion(value_t _x, value_t _y, value_t _z, value_t _w) {
		set(_x, _y, _z, _w);
	}

	bool operator==(const Quaternion & other) const {
		return values[0] == other.values[0] && values[1] == other.values[1] && values[2] == other.values[2]
				&& values[3] == other.values[3];
	}
	bool operator!=(const Quaternion & other) const {
		return values[0] != other.values[0] || values[1] != other.values[1] || values[2] != other.values[2]
				|| values[3] != other.values[3];
	}

	void set(value_t _x, value_t _y, value_t _z, value_t _w) {
		values[0] = _x;
		values[1] = _y;
		values[2] = _z;
		values[3] = _w;
	}

	value_t & operator[](int i) {
		return values[i];
	}
	value_t operator[](int i) const {
		return values[i];
	}

	value_t & x() {
		return values[0];
	}
	value_t & y() {
		return values[1];
	}
	value_t & z() {
		return values[2];
	}
	value_t & w() {
		return values[3];
	}

	value_t x() const {
		return values[0];
	}
	value_t y() const {
		return values[1];
	}
	value_t z() const {
		return values[2];
	}
	value_t w() const {
		return values[3];
	}

	const Quaternion operator*(value_t factor) const {
		return Quaternion(values[0] * factor, values[1] * factor, values[2] * factor, values[3] * factor);
	}

	Quaternion & operator*=(value_t factor) {
		values[0] *= factor;
		values[1] *= factor;
		values[2] *= factor;
		values[3] *= factor;
		return *this;
	}

	const Quaternion operator/(value_t divisor) const {
		value_t factor = 1.0f / divisor;
		return Quaternion(values[0] * factor, values[1] * factor, values[2] * factor, values[3] * factor);
	}

	Quaternion & operator/=(value_t divisor) {
		value_t factor = 1.0f / divisor;
		values[0] *= factor;
		values[1] *= factor;
		values[2] *= factor;
		values[3] *= factor;
		return *this;
	}

	const Quaternion operator*(const Quaternion & q2) const {
		return Quaternion(values[3] * q2[0] + values[0] * q2[3] + values[1] * q2[2] - values[2] * q2[1],
						  values[3] * q2[1] - values[0] * q2[2] + values[1] * q2[3] + values[2] * q2[0],
						  values[3] * q2[2] + values[0] * q2[1] - values[1] * q2[0] + values[2] * q2[3],
						  values[3] * q2[3] - values[0] * q2[0] - values[1] * q2[1] - values[2] * q2[2]);
	}

	Quaternion operator*=(const Quaternion & q2) {
		value_t tempX = values[3] * q2[0] + values[0] * q2[3] + values[1] * q2[2] - values[2] * q2[1];
		value_t tempY = values[3] * q2[1] - values[0] * q2[2] + values[1] * q2[3] + values[2] * q2[0];
		value_t tempZ = values[3] * q2[2] + values[0] * q2[1] - values[1] * q2[0] + values[2] * q2[3];
		values[3] = values[3] * q2[3] - values[0] * q2[0] - values[1] * q2[1] - values[2] * q2[2];

		values[0] = tempX;
		values[1] = tempY;
		values[2] = tempZ;

		return *this;
	}

	const Quaternion operator/(const Quaternion & q2) const {
		return Quaternion((*this) * q2.inverse());
	}

	Quaternion operator/=(const Quaternion & q2) {
		*this = (*this) * q2.inverse();
		return *this;
	}

	const Quaternion operator+(const Quaternion & source) const {
		return Quaternion(values[0] + source[0], values[1] + source[1], values[2] + source[2], values[3] + source[3]);
	}

	Quaternion operator+=(const Quaternion & source) {
		values[0] += source[0];
		values[1] += source[1];
		values[2] += source[2];
		values[3] += source[3];
		return *this;
	}

	const Quaternion operator-(const Quaternion & source) const {
		return Quaternion(values[0] - source[0], values[1] - source[1], values[2] - source[2], values[3] - source[3]);
	}

	Quaternion operator-=(const Quaternion & source) {
		values[0] -= source[0];
		values[1] -= source[1];
		values[2] -= source[2];
		values[3] -= source[3];
		return *this;
	}

	value_t length() const {
		return std::sqrt(length2());
	}

	value_t length2() const {
		return values[0] * values[0] + values[1] * values[1] + values[2] * values[2] + values[3] * values[3];
	}

	Quaternion conjugate() const {
		return Quaternion(-x(), -y(), -z(), w());
	}

	Quaternion inverse() const {
		return conjugate() / length2();
	}

	void normalize() {
		*this /= length();
	}

	value_t dot(const Quaternion & q) const {
		return values[0] * q[0] + values[1] * q[1] + values[2] * q[2] + values[3] * q[3];
	}

	/* source;
	   http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	 */
	static Quaternion matrixToQuaternion(const Matrix3x3 & src) {
		const value_t trace = 1.0f + src.at(0, 0) + src.at(1, 1) + src.at(2, 2);

		if (trace > 0.00001f) {
			const value_t s = std::sqrt(trace) * 2;
			return Quaternion((src.at(2, 1) - src.at(1, 2)) / s, (src.at(0, 2) - src.at(2, 0)) / s,
							  (src.at(1, 0) - src.at(0, 1)) / s, s / 4);
		} else if (src.at(0, 0) > src.at(1, 1) && src.at(0, 0) > src.at(2, 2)) {
			const value_t s = std::sqrt(1.0f + src.at(0, 0) - src.at(1, 1) - src.at(2, 2)) * 2;
			return Quaternion(s / 4, (src.at(1, 0) + src.at(0, 1)) / s, (src.at(0, 2) + src.at(2, 0)) / s,
							  (src.at(2, 1) - src.at(1, 2)) / s);
		} else if (src.at(1, 1) > src.at(2, 2)) {
			const value_t s = std::sqrt(1.0f + src.at(1, 1) - src.at(0, 0) - src.at(2, 2)) * 2;
			return Quaternion((src.at(1, 0) + src.at(0, 1)) / s, s / 4, (src.at(2, 1) + src.at(1, 2)) / s,
							  (src.at(0, 2) - src.at(2, 0)) / s);
		} else {
			const value_t s = std::sqrt(1.0f + src.at(2, 2) - src.at(0, 0) - src.at(1, 1)) * 2;
			return Quaternion((src.at(0, 2) + src.at(2, 0)) / s, (src.at(2, 1) + src.at(1, 2)) / s, s / 4,
							  (src.at(1, 0) - src.at(0, 1)) / s);
		}
	}

	/*  source:
	 *  http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	 */
	Matrix3x3 toMatrix() const {
		Quaternion q(*this);
		q.normalize();

		value_t wx = q.w() * q.x();
		value_t wy = q.w() * q.y();
		value_t wz = q.w() * q.z();

		value_t xx = q.x() * q.x();
		value_t xy = q.x() * q.y();
		value_t xz = q.x() * q.z();

		value_t yy = q.y() * q.y();
		value_t yz = q.y() * q.z();
		value_t zz = q.z() * q.z();

		Matrix3x3 mat;
		mat.set(0, 0, 1.0 - 2.0 * (yy + zz));
		mat.set(0, 1, 2.0 * (xy - wz));
		mat.set(0, 2, 2.0 * (xz + wy));
		mat.set(1, 0, 2.0 * (xy + wz));
		mat.set(1, 1, 1.0 - 2.0 * (xx + zz));
		mat.set(1, 2, 2.0 * (yz - wx));
		mat.set(2, 0, 2.0 * (xz - wy));
		mat.set(2, 1, 2.0 * (yz + wx));
		mat.set(2, 2, 1.0 - 2.0 * (xx + yy));

		return mat;
	}

	/*  source:
	 *  http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
	 *  Works only for EulerXYZ rotation. Other rotationstandards are not supported!
	 */
	vec3_t toEuler() const {
		value_t sqw = values[3] * values[3];
		value_t sqx = values[0] * values[0];
		value_t sqy = values[1] * values[1];
		value_t sqz = values[2] * values[2];
		value_t unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
		value_t test = values[0] * values[1] + values[2] * values[3];
		if (test > 0.499 * unit) { // singularity at north pole
			return vec3_t(2 * std::atan2(values[0], values[3]), M_PI / 2, 0);
		}
		if (test < -0.499 * unit) { // singularity at south pole
			return vec3_t(-2 * std::atan2(values[0], values[3]), -M_PI / 2, 0);
		}
		return vec3_t(std::atan2(2 * values[1] * values[3] - 2 * values[0] * values[2], sqx - sqy - sqz + sqw),
					  std::asin(2 * test / unit),
					  std::atan2(2 * values[0] * values[3] - 2 * values[1] * values[2], -sqx + sqy - sqz + sqw));
	}

	/*
	 * source:
	 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
	 */
	static Quaternion eulerToQuaternion(const vec3_t & euler) {
		value_t c1 = std::cos(euler.x() / 2);
		value_t s1 = std::sin(euler.x() / 2);
		value_t c2 = std::cos(euler.y() / 2);
		value_t s2 = std::sin(euler.y() / 2);
		value_t c3 = std::cos(euler.z() / 2);
		value_t s3 = std::sin(euler.z() / 2);
		value_t c1c2 = c1 * c2;
		value_t s1s2 = s1 * s2;

		return Quaternion(c1c2 * s3 + s1s2 * c3, s1 * c2 * c3 + c1 * s2 * s3, c1 * s2 * c3 - s1 * c2 * s3,
						  c1c2 * c3 - s1s2 * s3);
	}

	vec3_t rotatePoint(const vec3_t & p1) {
		vec3_t p2;
		p2.setX(w() * w() * p1.x() + 2 * y() * w() * p1.z() - 2 * z() * w() * p1.y() + x() * x() * p1.x()
				+ 2 * y() * x() * p1.y() + 2 * z() * x() * p1.z() - z() * z() * p1.x() - y() * y() * p1.x());
		p2.setY(2 * x() * y() * p1.x() + y() * y() * p1.y() + 2 * z() * y() * p1.z() + 2 * w() * z() * p1.x()
				- z() * z() * p1.y() + w() * w() * p1.y() - 2 * x() * w() * p1.z() - x() * x() * p1.y());
		p2.setZ(2 * x() * z() * p1.x() + 2 * y() * z() * p1.y() + z() * z() * p1.z() - 2 * w() * y() * p1.x()
				- y() * y() * p1.z() + 2 * w() * x() * p1.y() - x() * x() * p1.z() + w() * w() * p1.z());
		return p2;
	}

	static Quaternion lerp(const Quaternion & q1, const Quaternion & q2, value_t factor) {
		return Interpolation::linear(q1, q2, factor);
	}

	// source: http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
	static Quaternion slerp(const Quaternion & q1, const Quaternion & q2, value_t factor) {
		// Compute the cosine of the angle between the two
		value_t dot = q1.dot(q2);

		if (dot > 0.9995) {
			// If the inputs are too close for comfort, linearly interpolate
			// and normalize the result.
			Quaternion q = lerp(q1, q2, factor);
			q.normalize();
			return q;
		}

		// Robustness: clamp to stay within domain of acos()
		if (dot < -1) {
			dot = -1;
		} else if (dot > 1) {
			dot = 1;
		}

		value_t theta_0 = std::acos(dot); // theta_0 = angle between input vectors
		value_t theta = theta_0 * factor; // theta = angle between q1 and result

		Quaternion q = q2 - q1 * dot;
		q.normalize();

		return q1 * std::cos(theta) + q * std::sin(theta);
	}

	/*!	@name Rotation */
	//@{
	void makeRotate(const angle_t & angle, value_t _x, value_t _y, value_t _z) {
		const value_t rad = angle.rad();
		// Used to normalize the rotation vector.
		const value_t invLength = 1.0f / std::sqrt(_x * _x + _y * _y + _z * _z);
		const value_t sinHalfAngle = std::sin(0.5 * rad);
		values[0] = sinHalfAngle * invLength * _x;
		values[1] = sinHalfAngle * invLength * _y;
		values[2] = sinHalfAngle * invLength * _z;
		values[3] = std::cos(0.5f * rad);
	}
	inline void makeRotate(const angle_t & angle, const vec3_t & axis) {
		makeRotate(angle, axis.x(), axis.y(), axis.z());
	}
	inline void makeRotate_deg(value_t deg, value_t _x, value_t _y, value_t _z) {
		makeRotate(angle_t::deg(deg), _x, _y, _z);
	}
	inline void makeRotate_deg(value_t deg, const vec3_t & axis) {
		makeRotate(angle_t::deg(deg), axis.x(), axis.y(), axis.z());
	}
	inline void makeRotate_rad(value_t rad, value_t _x, value_t _y, value_t _z) {
		makeRotate(angle_t::rad(rad), _x, _y, _z);
	}
	inline void makeRotate_rad(value_t rad, const vec3_t & axis) {
		makeRotate(angle_t::rad(rad), axis.x(), axis.y(), axis.z());
	}

	Quaternion(const angle_t & rotX, const angle_t & rotY, const angle_t & rotZ) {
		set(0.0f, 0.0f, 0.0f, 1.0f);
		Quaternion quatX, quatY, quatZ, target;
		quatX.makeRotate(rotX, vec3_t(1, 0, 0));
		quatY.makeRotate(rotY, vec3_t(0, 1, 0));
		quatZ.makeRotate(rotZ, vec3_t(0, 0, 1));

		target = quatX * quatY * quatZ;
		set(target[0], target[1], target[2], target[3]);

		normalize();
	}
	//@}

	//! @name Serialization
	//@{
	friend std::ostream & operator<<(std::ostream & out, const Quaternion & q) {
		return out << q.values[0] << ' ' << q.values[1] << ' ' << q.values[2] << ' ' << q.values[3];
	}
	friend std::istream & operator>>(std::istream & in, Quaternion & q) {
		return in >> q.values[0] >> q.values[1] >> q.values[2] >> q.values[3];
	}
	//@}

private:
	value_t values[4];
};
}
#endif /* GEOMETRY_QUATERNION_H */

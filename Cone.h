/*
	This file is part of the Geometry library.
	Copyright (C) 2021 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_CONE_H
#define GEOMETRY_CONE_H

#include "Vec3.h"
#include "Angle.h"
#include <cmath>
#include <istream>
#include <ostream>

namespace Geometry {

/**
 * Representation of a (infinite) cone in three-dimensional space.
 * The cone is stored as the three-dimensional apex, the cone axis and its cosine cutoff (cos(angle/2)).
 *
 * @author Sascha Brandt
 * @date 2021-11-01
 */
template <typename T_>
class _Cone {
public:
	typedef T_ value_t;
	typedef _Vec3<value_t> vec3_t;
	typedef _Angle<value_t> angle_t;

private:
	vec3_t apex;
	vec3_t axis;
	value_t cutoff; // cos(angle/2)

public:
	//! @name Construction
	//@{
	//! Construct a 180° cone centered at the origin in direction of the y-axis
	_Cone() : apex(0, 0, 0), axis(0, 1, 0), cutoff(0) {
	}

	//! Construct a cone with the given apex, axis and cosine cutoff.
	_Cone(vec3_t _apex, vec3_t _axis, value_t _cutoff) : apex(std::move(_apex)), axis(std::move(_axis)), cutoff(std::clamp(_cutoff, static_cast<value_t>(-1), static_cast<value_t>(1))) {
		axis.normalize();
	}

	//! Construct a cone with the given apex, axis and cone angle.
	_Cone(vec3_t _apex, vec3_t _axis, angle_t _angle) : apex(std::move(_apex)), axis(std::move(_axis)), cutoff(std::cos(_angle.rad()/static_cast<value_t>(2.0))) {
		axis.normalize();
	}
	//@}

	//! @name Information
	//@{
	const vec3_t & getApex() const {
		return apex;
	}
	const vec3_t & getAxis() const {
		return axis;
	}
	value_t getCutoff() const {
		return cutoff;
	}
	angle_t getAngle() const {
		return angle_t(static_cast<value_t>(2.0) * std::acos(cutoff), false);
	}
	bool isValid() const {
		return cutoff > static_cast<value_t>(-1);
	}
	bool operator==(const _Cone<value_t> & other) const {
		return apex == other.apex && axis == other.axis && cutoff == other.cutoff;
	}
	//@}

	//! @name Modification
	//@{
	void setApex(const vec3_t & _apex) {
		apex = _apex;
	}
	void setAxis(const vec3_t & _axis) {
		axis = _axis.getNormalized();
	}
	void setCutoff(value_t _cutoff) {
		cutoff = std::clamp(_cutoff, static_cast<value_t>(-1), static_cast<value_t>(1));
	}
	void setAngle(angle_t _angle) {
		cutoff = std::cos(_angle.rad()/static_cast<value_t>(2.0));
	}
	//@}

	//! @name Serialization
	//@{
	friend std::ostream & operator<<(std::ostream & out, const _Cone<value_t> & cone) {
		return out << cone.apex << ' ' cone.axis << ' ' << cone.cutoff;
	}
	friend std::istream & operator>>(std::istream & in, _Cone<value_t> & cone) {
		return in >> cone.apex >> cone.axis >> cone.cutoff;
	}
	//@}
};

typedef _Cone<float> Cone;
typedef _Cone<double> Cone_d;
typedef _Cone<float> Cone_f;
typedef _Cone<int> Cone_i;
}
#endif /* GEOMETRY_CONE_H */

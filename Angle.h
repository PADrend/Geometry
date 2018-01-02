/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_ANGLE_H
#define GEOMETRY_ANGLE_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Geometry {

/*! Wrapper for angles which can be transparently accessed as degree or radians.*/
template <typename T_>
class _Angle {
public:
	using value_t = T_;

private:
	value_t value;
	bool valueIsInDegree;

	_Angle(value_t _value, bool _degree) : value(_value), valueIsInDegree(_degree) {
	}

public:
	static _Angle deg(value_t d) {
		return _Angle(d, true);
	}
	static _Angle rad(value_t r) {
		return _Angle(r, false);
	}

	_Angle() : value(0), valueIsInDegree(false) {
	}
	template <typename otherValue_t>
	_Angle(const _Angle<otherValue_t> & other)
			: value(static_cast<value_t>(other.isInDegree() ? other.deg() : other.rad())),
			  valueIsInDegree(other.isInDegree()) {
	}

	_Angle & operator=(const _Angle & other) {
		value = other.value, valueIsInDegree = other.valueIsInDegree;
		return *this;
	}
	_Angle operator+(const _Angle & other) const {
		return valueIsInDegree ? deg(value + other.deg()) : rad(value + other.rad());
	}
	_Angle operator-() const {
		return _Angle(-value, valueIsInDegree);
	}
	_Angle operator-(const _Angle & other) const {
		return valueIsInDegree ? deg(value - other.deg()) : rad(value - other.rad());
	}
	_Angle operator*(value_t v) const {
		return _Angle(value * v, valueIsInDegree);
	}
	_Angle operator/(value_t v) const {
		return _Angle(value * v, valueIsInDegree);
	}

	_Angle & operator+=(const _Angle & other) {
		value += (valueIsInDegree ? other.deg() : other.rad());
		return *this;
	}
	_Angle & operator-=(const _Angle & other) {
		value -= (valueIsInDegree ? other.deg() : other.rad());
		return *this;
	}
	_Angle & operator*=(value_t v) {
		value *= v;
		return *this;
	}
	_Angle & operator/=(value_t v) {
		value /= v;
		return *this;
	}

	bool operator<(const _Angle & other) const {
		return rad() < other.rad();
	}
	bool operator==(const _Angle & other) const {
		return rad() == other.rad();
	}

	value_t deg() const {
		return valueIsInDegree ? value : value * static_cast<value_t>(180.0 / M_PI);
	}
	value_t rad() const {
		return valueIsInDegree ? value * static_cast<value_t>(M_PI / 180.0) : value;
	}

	bool isInDegree() const {
		return valueIsInDegree;
	}
};

using Angle = _Angle<float>;
}

#endif /* GEOMETRY_ANGLE_H */

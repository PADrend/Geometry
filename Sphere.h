/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include "Vec3.h"
#include <cmath>
#include <istream>
#include <ostream>

namespace Geometry {

/**
 * Representation of a sphere in three-dimensional space.
 * The sphere is stored as the three-dimensional position of its center and its radius.
 *
 * @author Benjamin Eikel
 * @date 2011-10-14
 */
template <typename T_>
class _Sphere {
public:
	typedef T_ value_t;
	typedef _Vec3<value_t> vec3_t;

private:
	vec3_t center;
	value_t radius;

public:
	//! @name Construction
	//@{
	//! Construct a unit sphere centered at the origin.
	_Sphere() : center(0, 0, 0), radius(1) {
	}

	//! Construct a sphere with the given center and radius.
	_Sphere(vec3_t _center, value_t _radius) : center(std::move(_center)), radius(_radius) {
	}
	//@}

	//! @name Information
	//@{
	const vec3_t & getCenter() const {
		return center;
	}
	value_t getRadius() const {
		return radius;
	}
	bool isValid() const {
		return radius >= 0;
	}
	bool operator==(const _Sphere<value_t> & other) const {
		return center == other.center && radius == other.radius;
	}
	/**
	 * Calculate the distance of the given position to the sphere.
	 *
	 * @param position Position vector
	 * @retval >0 The position is outside the sphere.
	 * @retval 0 The position is on the surface of the sphere.
	 * @retval <0 The position is inside the sphere.
	 */
	value_t distance(const vec3_t & position) const {
		return center.distance(position) - radius;
	}
	/**
	 * Check if the given position is outside the sphere.
	 *
	 * @param position Position vector
	 * @retval true The position is outside the sphere.
	 * @retval false The position is inside the sphere, or on the surface of the sphere
	 */
	bool isOutside(const vec3_t & position) const {
		return center.distanceSquared(position) > radius * radius;
	}

	/**
	 * Convert a position on this sphere given in spherical coordinates into Cartesian coordinates.
	 * The function @a calcCartesianCoordinateUnitSphere is called, and center and radius of this sphere are applied to
	 *the result.
	 *
	 * @see calcCartesianCoordinateUnitSphere
	 */
	vec3_t calcCartesianCoordinate(value_t inclination, value_t azimuth) const {
		return center + (calcCartesianCoordinateUnitSphere(inclination, azimuth) * radius);
	}

	/**
	 * Convert a position on the unit sphere given in spherical coordinates into Cartesian coordinates.
	 * The unit sphere is centered at the origin.
	 *
	 * @param inclination Inclination angle from [0, PI]
	 * @param azimuth Azimuth angle from [0, 2 * PI)
	 * @return Three-dimensional vector representing a position on the unit sphere
	 * @note An @a inclination of zero corresponds to the north pole (0, 1, 0), independent of the azimuth.
	 * @note An @a inclination of PI corresponds to the south pole (0, -1, 0), independent of the azimuth.
	 * @note An @a inclination of PI/2 and @a azimuth of zero corresponds to the vector (1, 0, 0).
	 * @note An @a inclination of PI/2 and @a azimuth of 1/2 * PI corresponds to the vector (0, 0, 1).
	 * @note An @a inclination of PI/2 and @a azimuth of PI corresponds to the vector (-1, 0, 0).
	 * @note An @a inclination of PI/2 and @a azimuth of 3/2 * PI corresponds to the vector (0, 0, -1).
	 */
	static vec3_t calcCartesianCoordinateUnitSphere(value_t inclination, value_t azimuth) {
		const value_t sinInclination = std::sin(inclination);
		return _Vec3<value_t>(sinInclination * std::cos(azimuth), std::cos(inclination),
							  sinInclination * std::sin(azimuth));
	}
	//@}

	//! @name Modification
	//@{
	void setCenter(const vec3_t & _center) {
		center = _center;
	}
	void setRadius(value_t _radius) {
		radius = _radius;
	}

	//! Change the sphere such that it contains its old volume and additionally the given position.
	_Sphere & include(const vec3_t & position) {
		// Simply include the point if this sphere is invalid
		if (!isValid()) {
			center = position;
			radius = 0;
			return *this;
		}

		const value_t distCenterPosSquared = center.distanceSquared(position);
		if (distCenterPosSquared <= radius * radius) {
			return *this;
		}
		const value_t distCenterPos = std::sqrt(distCenterPosSquared);

		const value_t distSpherePos = distCenterPos - radius;
		const value_t halfDistSpherePos = distSpherePos / 2;
		// Move center halfDistSpherePos units into normalized (divide by length distCenterPos) direction.
		center += (position - center) / distCenterPos * halfDistSpherePos;
		radius += halfDistSpherePos;

		return *this;
	}

	//! Change the sphere such that it contains its old volume and additionally the given sphere.
	_Sphere & include(const _Sphere & other) {
		// Do not change the sphere if the other sphere is invalid
		if (!other.isValid()) {
			return *this;
		}

		// Simply copy the other sphere if this sphere is invalid
		if (!isValid()) {
			*this = other;
			return *this;
		}

		const value_t centerDistance = center.distance(other.center);
		if (centerDistance + other.radius <= radius) {
			// This sphere already includes the other sphere
			return *this;
		}
		if (centerDistance + radius <= other.radius) {
			// The other sphere already includes this sphere
			*this = other;
			return *this;
		}

		const value_t newRadius = (radius + centerDistance + other.radius) / 2;
		/*
		 * Let n be the normalized vector between the two centers:
		 * n = (other.center - center) / centerDistance
		 * Calculate the outermost intersection t of the line defined by the two centers with this sphere.
		 * t = center - radius * n
		 * The new center can then be calculated as:
		 * center' = t + newRadius * n
		 *         = center + (newRadius - radius) * n
		 *         = center + ((newRadius - radius) / centerDistance) * (other.center - center)
		 */
		const value_t proportion = (newRadius - radius) / centerDistance;
		center += (other.center - center) * proportion;
		radius = newRadius;

		return *this;
	}
	//@}

	//! @name Serialization
	//@{
	friend std::ostream & operator<<(std::ostream & out, const _Sphere<value_t> & sphere) {
		return out << sphere.center << ' ' << sphere.radius;
	}
	friend std::istream & operator>>(std::istream & in, _Sphere<value_t> & sphere) {
		return in >> sphere.center >> sphere.radius;
	}
	//@}
};

typedef _Sphere<double> Sphere_d;
typedef _Sphere<float> Sphere_f;
typedef _Sphere<int> Sphere_i;
}
#endif /* GEOMETRY_SPHERE_H */

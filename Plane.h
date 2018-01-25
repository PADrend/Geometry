/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_PLANE_H
#define GEOMETRY_PLANE_H

#include "Vec3.h"
#include <istream>
#include <ostream>

namespace Geometry {
template <typename vec_t>
class _Line;
template <typename vec_t>
class _Ray;
template <typename vec_t>
class _Segment;

/*! A three dimensional plane.
	The plane is internally represented by its normal and offset from (0,0,0):

	a*nx + b*ny + c*ny = offset

	\note Although the general definition allows a normal of arbitrary length -- internal calculations
		request the normal to always be of length 1.0	*/
template <typename T_>
class _Plane {
public:
	typedef T_ value_t;
	typedef _Vec3<value_t> vec3_t;

private:
	vec3_t normal;
	value_t offset;

	//! @name Creation
	//@{
public:
	/*! (ctor) Create an undefined plane. (isUndefined()==true)*/
	_Plane() : normal(), offset(0.0) {
	}

	/*! (ctor)
		@param _normal normalized normal-vector (length = 1.0)
		@param _offset offset from the origin */
	_Plane(vec3_t _normal, const value_t _offset) : normal(std::move(_normal)), offset(_offset) {
	}

	/*! (ctor)
		@param _pos one point on the plane
		@param _normal normalized normal-vector (length = 1.0)	*/
	_Plane(const vec3_t & _pos, vec3_t _normal) : normal(std::move(_normal)), offset(_pos.dot(normal)) {
	}

	/*! (ctor)
		@param a,b,c Points defining the plane 	*/
	_Plane(const vec3_t & a, const vec3_t & b, const vec3_t & c)
			: normal(((b - a).cross(c - a)).normalize()), offset(a.dot(normal)) {
	}
	//@}

	//! @name Information
	//@{
	/**
	 * Calculate the intersection of the given @p line/ray/segment with the plane.
	 *
	 * @return true iff the line intersects with the plane. The intersection is then stored in @p intersection.
	 */
private:
	template <class _LineBase>
	bool _getIntersection(const _LineBase & line, vec3_t & intersection) const {
		const value_t u = normal.dot(line.getDirection());
		if (u != 0) {
			const value_t t = normal.dot(normal * offset - line.getOrigin()) / u;
			if (t >= line.getMinParam() && t <= line.getMaxParam()) {
				intersection = line.getPoint(t);
				return true;
			}
		}
		return false;
	}

public:
	inline bool getIntersection(const _Line<vec3_t> & line, vec3_t & intersection) const {
		return _getIntersection(line, intersection);
	}
	inline bool getIntersection(const _Ray<vec3_t> & ray, vec3_t & intersection) const {
		return _getIntersection(ray, intersection);
	}
	inline bool getIntersection(const _Segment<vec3_t> & s, vec3_t & intersection) const {
		return _getIntersection(s, intersection);
	}

	bool getIntersection(const _Plane & plane, _Line<vec3_t> & intersection) const;

	inline const vec3_t & getNormal() const {
		return normal;
	}

	inline const value_t & getOffset() const {
		return offset;
	}

	inline vec3_t getProjection(const vec3_t & point) const {
		return point - normal * normal.dot(point - normal * offset);
	}

	//! @return true iff the plane has no normal.
	inline bool isUndefined() const {
		return normal.isZero();
	}

	inline bool operator==(const _Plane & other) const {
		return offset == other.offset && normal == other.normal;
	}

	/**
	 * @return  signed distance of the given point to the plane.
	 *			on plane    = 0
	 *          below plane < 0
	   *          above plane > 0
	 */
	inline float planeTest(const vec3_t & point) const {
		return point.dot(normal) - offset;
	}

	//@}

	//! @name Modification
	//@{
	//! \note it is assumed that the normal has length 1.0
	inline void setNormal(const vec3_t & _normal) {
		normal = _normal;
	}

	inline void setOffset(const value_t _offset) {
		offset = _offset;
	}
	//@}

	//! @name Serialization
	//@{
public:
	friend std::ostream & operator<<(std::ostream & out, const _Plane<value_t> & p) {
		return out << p.getNormal() << ' ' << p.getOffset();
	}
	friend std::istream & operator>>(std::istream & in, _Plane<value_t> & p) {
		return in >> p.normal >> p.offset;
	}
	//@}
};

template <typename T>
bool _Plane<T>::getIntersection(const _Plane<T> & plane, _Line<vec3_t> & intersection) const {
	// compute direction of intersection line
	auto dir = plane.getNormal().cross(plane.getNormal());
	if(dir.lengthSquared() < std::numeric_limits<T>::epsilon())
		return false;
	dir.normalize();
	
	// compute index to the largest component of D
	value_t max = std::fabs(dir[0]);
	uint8_t index = 0;
	{
		value_t b = std::fabs(dir[1]);
		value_t c = std::fabs(dir[2]);
		if (b > max)
			max = b, index = 1;
		if (c > max)
			max = c, index = 2;
	}
	
	vec3_t p;
	T d1 = -offset;
	T d2 = -plane.offset;
	switch (index) {
		case 0:
			p.setValue(0, (d2*normal.z() - d1*plane.normal.z()) / dir.x(), (d1*plane.normal.y() - d2*normal.y()) / dir.x()); break;
		case 1:
			p.setValue((d1*plane.normal.z() - d2*normal.z()) / dir.y(), 0, (d2*normal.x() - d1*plane.normal.x()) / dir.y()); break;
		default:
			p.setValue((d2*normal.y() - d1*plane.normal.y()) / dir.z(), (d1*plane.normal.x() - d2*normal.x()) / dir.z(), 0); 
	}
	intersection.setOrigin(p);
	intersection.setDirection(dir);
	
	return true;
}

typedef _Plane<float> Plane;
typedef _Plane<float> Planef;
typedef _Plane<double> Planed;
}

#endif /* GEOMETRY_PLANE_H */
